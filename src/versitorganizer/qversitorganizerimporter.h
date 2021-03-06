/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QVERSITORGANIZERIMPORTER_H
#define QVERSITORGANIZERIMPORTER_H

#include "qmobilityglobal.h"

#include <qorganizeritem.h>

#include <QList>

QTM_BEGIN_NAMESPACE

class QVersitDocument;
class QVersitOrganizerImporterPrivate;
class QVersitProperty;

class Q_VERSIT_ORGANIZER_EXPORT QVersitOrganizerImporterPropertyHandler
{
public:
    virtual ~QVersitOrganizerImporterPropertyHandler() {}
    virtual void propertyProcessed(const QVersitDocument& document,
                                   const QVersitProperty& property,
                                   const QOrganizerItem& item,
                                   bool *alreadyProcessed,
                                   QList<QOrganizerItemDetail>* updatedDetails) = 0;
    virtual void subDocumentProcessed(const QVersitDocument& topLevel,
                                      const QVersitDocument& subDocument,
                                      QOrganizerItem* item) = 0;
};

class Q_VERSIT_ORGANIZER_EXPORT QVersitOrganizerImporter
{
public:
    enum Error {
        NoError = 0,
        InvalidDocumentError,
        EmptyDocumentError
    };

    QVersitOrganizerImporter();
    explicit QVersitOrganizerImporter(const QString& profile);
    ~QVersitOrganizerImporter();

    bool importDocument(const QVersitDocument& document);
    QList<QOrganizerItem> items() const;
    QMap<int, Error> errorMap() const;

    void setPropertyHandler(QVersitOrganizerImporterPropertyHandler* handler);

private:
    QVersitOrganizerImporterPrivate* d;
};

QTM_END_NAMESPACE

#endif // QVERSITORGANIZERIMPORTER_H
