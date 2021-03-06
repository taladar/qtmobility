/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "contactlistpage.h"
#ifdef BUILD_VERSIT
#include "qversitreader.h"
#include "qversitcontactimporter.h"
#include "qversitwriter.h"
#include "qversitcontactexporter.h"
#include <QDesktopServices>
#endif

#include <QtGui>

ContactListPage::ContactListPage(QMainWindow *mainWindow, QWidget *parent)
        : QWidget(parent), m_mainWindow(mainWindow)
{
    m_manager = 0;
    m_currentFilter = QContactFilter();
    m_needsFresh = true;

    m_backendsCombo = new QComboBox(this);
    QStringList availableManagers = QContactManager::availableManagers();
    availableManagers.removeAll("invalid");
    foreach(QString managerName, availableManagers) {

        QMap<QString, QString> params;
        QString managerUri = QContactManager::buildUri(managerName, params);

        // Add some parameters to SIM backend so that we can use
        // all the stores.
        if (managerName == "symbiansim") {
            availableManagers.removeAll("symbiansim");

            availableManagers.append("symbiansim:adn");
            params.insert("store", "ADN");
            managerUri = QContactManager::buildUri(managerName, params);
            m_availableManagers.insert(availableManagers.last(), managerUri);

            availableManagers.append("symbiansim:fdn");
            params.clear();
            params.insert("store", "FDN");
            managerUri = QContactManager::buildUri(managerName, params);
            m_availableManagers.insert(availableManagers.last(), managerUri);

            availableManagers.append("symbiansim:sdn");
            params.clear();
            params.insert("store", "SDN");
            managerUri = QContactManager::buildUri(managerName, params);
            m_availableManagers.insert(availableManagers.last(), managerUri);
        }
        else {
            m_availableManagers.insert(managerName, managerUri);
        }
    }
    m_backendsCombo->addItems(availableManagers);
    connect(m_backendsCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(backendSelected()));
    m_filterActiveLabel = new QLabel(tr("Filter active"));
    m_filterActiveLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    QVBoxLayout *bookLayout = new QVBoxLayout;
    QFormLayout *backendLayout = new QFormLayout;
    backendLayout->addRow(tr("Store:"), m_backendsCombo);
    backendLayout->addRow(m_filterActiveLabel);
    bookLayout->addLayout(backendLayout);

    m_contactsList = new QListWidget(this);
    bookLayout->addWidget(m_contactsList);

    // Action buttons at the bottom
    QHBoxLayout *btnLayout1 = new QHBoxLayout;

    QPushButton* addBtn = new QPushButton(tr("&Add"), this);
    connect(addBtn, SIGNAL(clicked()), this, SLOT(addClicked()));
    btnLayout1->addWidget(addBtn);

    QPushButton* editBtn = new QPushButton(tr("&Edit"), this);
    connect(editBtn, SIGNAL(clicked()), this, SLOT(editClicked()));
    btnLayout1->addWidget(editBtn);

    QPushButton* deleteBtn = new QPushButton(tr("&Delete"), this);
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(deleteClicked()));
    btnLayout1->addWidget(deleteBtn);

    bookLayout->addLayout(btnLayout1);

    setLayout(bookLayout);

    // Add items to the menu
    if (m_mainWindow) {
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_OS_WINCE)
        // These platforms need their menu items added directly to the menu bar.
        QMenuBar *optionsMenu = m_mainWindow->menuBar();
#else
        QMenu *optionsMenu = new QMenu(tr("&Contacts"), this);
        m_mainWindow->menuBar()->addMenu(optionsMenu);
#endif
        QAction* filterAction = new QAction(tr("Apply &Filter..."), this);
        connect(filterAction, SIGNAL(triggered()), this, SLOT(filterClicked()));
        optionsMenu->addAction(filterAction);
        QAction* clearFilterAction = new QAction(tr("&Clear Filter"), this);
        connect(clearFilterAction, SIGNAL(triggered()), this, SIGNAL(clearFilter()));
        optionsMenu->addAction(clearFilterAction);
        optionsMenu->addSeparator();

#ifdef BUILD_VERSIT
        QAction* importAction = new QAction(tr("&Import contacts..."), this);
        connect(importAction, SIGNAL(triggered()), this, SLOT(importClicked()));
        optionsMenu->addAction(importAction);
        QAction* exportAction = new QAction(tr("Ex&port contacts..."), this);
        connect(exportAction, SIGNAL(triggered()), this, SLOT(exportClicked()));
        optionsMenu->addAction(exportAction);
        optionsMenu->addSeparator();
#endif
#if !(defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6))
        // Maemo applications don't have an Exit button in the menu.
        QAction* exitAction = new QAction(tr("E&xit"), this);
        connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
        optionsMenu->addAction(exitAction);
#endif
    }

    // force update to backend.
    QTimer::singleShot(0, this, SLOT(backendSelected()));
}

ContactListPage::~ContactListPage()
{
    QList<QContactManager*> initialisedManagers = m_initialisedManagers.values();
    while (!initialisedManagers.isEmpty()) {
        QContactManager *deleteMe = initialisedManagers.takeFirst();
        delete deleteMe;
    }
}

void ContactListPage::backendSelected()
{
    QString managerUri = m_availableManagers.value(m_backendsCombo->currentText());

    // first, check to see if they reselected the same backend.
    if (m_manager && m_manager->managerUri() == managerUri)
        return;

    if (m_manager)
        disconnect(m_manager, 0, this, 0); // we no longer want updates from the old manager

    // the change is real.  update.
    if (m_initialisedManagers.contains(managerUri)) {
        m_manager = m_initialisedManagers.value(managerUri);
    } else {
        m_manager = QContactManager::fromUri(managerUri);
        if (m_manager->error()) {
            QMessageBox::information(this, tr("Failed!"), QString("Failed to open store!\n(error code %1)").arg(m_manager->error()));
            delete m_manager;
            m_manager = 0;
            return;
        }
        m_initialisedManagers.insert(managerUri, m_manager);
    }

    connect(m_manager, SIGNAL(contactsAdded(const QList<QContactLocalId>&)),
            this, SLOT(contactsAdded(const QList<QContactLocalId>&)));
    connect(m_manager, SIGNAL(contactsRemoved(const QList<QContactLocalId>&)),
            this, SLOT(contactsRemoved(const QList<QContactLocalId>&)));
    connect(m_manager, SIGNAL(contactsChanged(const QList<QContactLocalId>&)),
            this, SLOT(contactsChanged(const QList<QContactLocalId>&)));
    connect(m_manager, SIGNAL(dataChanged()),
            this, SLOT(rebuildList()));
    // signal that the manager has changed.
    emit managerChanged(m_manager);

    // and... rebuild the list.
    rebuildList(m_currentFilter);
}

void ContactListPage::rebuildList(const QContactFilter& filter)
{
    //updating is going on
    m_needsFresh = false;

    m_currentFilter = QContactManagerEngine::canonicalizedFilter(filter);

    m_filterActiveLabel->setVisible(m_currentFilter != QContactFilter());

    m_contactsList->clear();
    m_contacts = m_manager->contacts(m_currentFilter);
    //set the flag so new request would not be lost during view list updating
    m_needsFresh = true;
    foreach (QContact contact, m_contacts) {
        QListWidgetItem *currItem = new QListWidgetItem;
        currItem->setData(Qt::DisplayRole, contact.displayLabel());
        currItem->setData(Qt::UserRole, contact.localId()); // also store the id of the contact.
        m_contactsList->addItem(currItem);
    }
}

//contactsAdded is called when new contact datas are added into database
void ContactListPage::contactsAdded(const QList<QContactLocalId>& ids)
{
    Q_UNUSED(ids);
    //skip update this time if updating is going on,
    if (m_needsFresh)
    {
        //updating is going on
        m_needsFresh = false;
        //set timer to update the view list and avoid multi times fetch
        QTimer::singleShot(0, this, SLOT(rebuildList()));
    }
}

//ContactsAdd() singleShot timer expired function, it gets contacts list and updates the view list
void ContactListPage::rebuildList ()
{
    m_contactsList->clear();
    m_contacts = m_manager->contacts(m_currentFilter);
    //set the flag so new request would not be lost during view list updating
    m_needsFresh = true;
    //update the whole view list
    foreach (QContact contact, m_contacts) {
        QListWidgetItem *currItem = new QListWidgetItem;
        currItem->setData(Qt::DisplayRole, contact.displayLabel());
        currItem->setData(Qt::UserRole, contact.localId()); // also store the id of the contact.
        m_contactsList->addItem(currItem);
    }
}

void ContactListPage::contactsRemoved(const QList<QContactLocalId>& ids)
{
    int i = 0;
    while (i < m_contactsList->count()) {
        if (ids.contains(m_contactsList->item(i)->data(Qt::UserRole).toUInt())) {
            delete m_contactsList->takeItem(i);
        } else {
            i++;
        }
    }
    i = 0;
    while (i < m_contacts.count()) {
        if (ids.contains(m_contacts[i].localId())) {
            m_contacts.removeAt(i);
        } else {
            i++;
        }
    }
}

void ContactListPage::contactsChanged(const QList<QContactLocalId>& ids)
{
    int i = 0;
    QMap<QContactLocalId, QContact> updatedContacts;
    foreach (QContactLocalId id, ids) {
        QContact contact = m_manager->contact(id);
        updatedContacts.insert(id, contact);
    }

    for (i = 0; i < m_contactsList->count(); i++) {
        QListWidgetItem* currItem = m_contactsList->item(i);
        QContactLocalId lid = currItem->data(Qt::UserRole).toUInt();
        if (updatedContacts.contains(lid)) {
            currItem->setData(Qt::DisplayRole, updatedContacts.value(lid).displayLabel());
        }
    }

    for (i = 0; i < m_contacts.count(); i++) {
        QContactLocalId lid = m_contacts[i].localId();
        if (updatedContacts.contains(lid)) {
            m_contacts[i] = updatedContacts.value(lid);
        }
    }
}

void ContactListPage::dataChanged()
{
    rebuildList(m_currentFilter);
}

void ContactListPage::addClicked()
{
    if (m_manager)
        emit showEditorPage(QContactLocalId(0));
}

void ContactListPage::editClicked()
{
    if (m_contactsList->currentItem())
        emit showEditorPage(QContactLocalId(m_contactsList->currentItem()->data(Qt::UserRole).toUInt()));
    // else, nothing selected; ignore.
}

void ContactListPage::filterClicked()
{
    if (m_manager)
        emit showFilterPage(m_currentFilter);
}

void ContactListPage::deleteClicked()
{
    if (!m_manager) {
        qWarning() << "No manager selected; cannot delete.";
        return;
    }

    if (!m_contactsList->currentItem()) {
        qWarning() << "Nothing to delete.";
        return;
    }

    QContactLocalId contactId = QContactLocalId(m_contactsList->currentItem()->data(Qt::UserRole).toUInt());
    bool success = m_manager->removeContact(contactId);
    if (success) {
        // no need to do this here - we can wait for the signal to get through to contactsRemoved()
        // delete m_contactsList->takeItem(m_contactsList->currentRow());
    }
    else
        QMessageBox::information(this, "Failed!", "Failed to delete contact!");
}

void ContactListPage::importClicked()
{
#ifdef BUILD_VERSIT
    if (!m_manager) {
        qWarning() << "No manager selected; cannot import";
        return;
    }
    QString docPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    if (docPath.isEmpty())
        docPath = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
    if (docPath.isEmpty())
        docPath = ".";
    QString fileName = QFileDialog::getOpenFileName(this,
       tr("Select vCard file"), docPath, tr("vCard files (*.vcf)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        bool success = false;
        if (file.isReadable()) {
            QVersitReader reader;
            reader.setDevice(&file);
            if (reader.startReading() && reader.waitForFinished()) {
                QVersitContactImporter importer;
                if (importer.importDocuments(reader.results())) {
                    QList<QContact> contacts = importer.contacts();
                    qDebug() << "Contacts imported: " << contacts;
                    QMap<int, QContactManager::Error> errorMap;
                    QList<QContact>::iterator it = contacts.begin();
                    while (it != contacts.end()) {
                        *it = m_manager->compatibleContact(*it);
                        it++;
                    }
                    if (contacts.count() > 0)
                        success = m_manager->saveContacts(&contacts, &errorMap);
                    //rebuildList(m_currentFilter);
                }
            }
        }

        // We could be more specific, probably
        if (!success)
            QMessageBox::warning(this, tr("Error importing vCard"), tr("Unable to import vCard file.  Check that the file exists and is valid."));
    }
#endif
}

void ContactListPage::exportClicked()
{
#ifdef BUILD_VERSIT
    if (!m_manager) {
        qWarning() << "No manager selected; cannot export";
        return;
    }
    qDebug() << "Exporting contacts: " << m_contacts;
    QString docPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    if (docPath.isEmpty())
        docPath = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
    if (docPath.isEmpty())
        docPath = ".";
    docPath.append("/contacts.vcf");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save vCard"),
                                                    docPath,
                                                    tr("vCards (*.vcf)"));

    if (!fileName.isEmpty()) {
        bool success = false;
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        if (file.isWritable()) {
            QVersitContactExporter exporter;
            if(exporter.exportContacts(m_contacts, QVersitDocument::VCard30Type)) {
                QList<QVersitDocument> documents = exporter.documents();
                QVersitWriter writer;
                writer.setDevice(&file);
                writer.startWriting(documents);
                writer.waitForFinished();
                success = (writer.error() == QVersitWriter::NoError);
            }
        }

        if (!success)
            QMessageBox::warning(this, tr("Error saving vCard"), tr("Unable to save vCard file.  Check that the path specified is writable."));
    }
#endif
}
