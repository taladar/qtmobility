/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qorganizercollectionremoverequest.h"
#include "qorganizeritemrequests_p.h"

QTM_BEGIN_NAMESPACE

/*!
  \class QOrganizerCollectionRemoveRequest
  \brief The QOrganizerCollectionRemoveRequest class allows a client to asynchronously
    request organizeritems from a organizeritems store manager.

  \inmodule QtOrganizer


  For a QOrganizerCollectionRemoveRequest, the resultsAvailable() signal will be emitted when the resultant
  organizeritems (which may be retrieved by calling organizeritems()), are updated, as well as if
  the overall operation error (which may be retrieved by calling error()) is updated.

  \ingroup organizeritems-requests
 */

/*! Constructs a new organizeritem fetch request whose parent is the specified \a parent */
QOrganizerCollectionRemoveRequest::QOrganizerCollectionRemoveRequest(QObject* parent)
    : QOrganizerAbstractRequest(new QOrganizerCollectionRemoveRequestPrivate, parent)
{
}

/*! Frees memory in use by this request */
QOrganizerCollectionRemoveRequest::~QOrganizerCollectionRemoveRequest()
{
    QOrganizerAbstractRequestPrivate::notifyEngine(this);
}

/*! Sets the list of ids of collections which will be removed by this request to a list containing the single element \a collectionId */
void QOrganizerCollectionRemoveRequest::setCollectionId(const QOrganizerCollectionId& collectionId)
{
    Q_D(QOrganizerCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_collectionIds.clear();
    d->m_collectionIds.append(collectionId);
}

/*! Sets the list of ids of collections which will be removed by this request to \a collectionIds */
void QOrganizerCollectionRemoveRequest::setCollectionIds(const QList<QOrganizerCollectionId>& collectionIds)
{
    Q_D(QOrganizerCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_collectionIds = collectionIds;
}

/*! Returns the list of ids of collections which will be removed by this request if possible */
QList<QOrganizerCollectionId> QOrganizerCollectionRemoveRequest::collectionIds() const
{
    Q_D(const QOrganizerCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_collectionIds;
}

/*! Returns any errors which occurred during the request */
QMap<int, QOrganizerManager::Error> QOrganizerCollectionRemoveRequest::errorMap() const
{
    Q_D(const QOrganizerCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

#include "moc_qorganizercollectionremoverequest.cpp"

QTM_END_NAMESPACE
