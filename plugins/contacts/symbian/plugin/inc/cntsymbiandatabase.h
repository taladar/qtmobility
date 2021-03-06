/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
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
#ifndef CNTSYMBIANDATABASE_H
#define CNTSYMBIANDATABASE_H

// System includes
#include <QList>
#include <QObject>
#include <e32std.h>
#include <cntdb.h>
#include <cntdbobs.h>

// User includes
#include "qcontactmanager.h"

// Forward declarations
QTM_BEGIN_NAMESPACE
class QContactManagerEngine;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE
// External data types

// Constants

class CntSymbianDatabase : public QObject, public MContactDbObserver
#ifdef SYMBIAN_BACKEND_USE_CNTMODEL_V2  
, public MContactDbObserverV2
#endif
{
Q_OBJECT

public:
    CntSymbianDatabase(QContactManagerEngine *engine, QContactManager::Error* error);
    ~CntSymbianDatabase();

public:
    CContactDatabase* contactDatabase();
    void appendContactsEmitted(const QList<QContactLocalId>& contactList);
    void appendContactEmitted(QContactLocalId id);

public:
    // From MContactDbObserver
    void HandleDatabaseEventL(TContactDbObserverEvent aEvent);

#ifdef SYMBIAN_BACKEND_USE_CNTMODEL_V2    
    // From MContactDbObserverV2
    void HandleDatabaseEventV2L(TContactDbObserverEventV2 aEvent);
#endif    
    
private:
    void initializeL();
    void updateGroupMembershipsL();
    void updateGroupMembershipsL(QContactLocalId groupId, QSet<QContactLocalId> &added, QSet<QContactLocalId> &removed);
    QSet<QContactLocalId> groupMembersL(QContactLocalId groupId);

private:
#ifndef SYMBIAN_BACKEND_USE_CNTMODEL_V2
    CContactChangeNotifier* m_contactChangeNotifier;
#endif
    QContactManagerEngine *m_engine;
    CContactDatabase* m_contactDatabase;
    QList<QContactLocalId> m_contactsEmitted;
    QContactLocalId m_currentOwnCardId;
    QMap<QContactLocalId, QSet<QContactLocalId> > m_groupContents;
    bool m_restoreStarted;
#ifdef CNTSYMBIANDATABASE_UNIT_TEST
    friend class TestCntSymbianDatabase;
#endif  //CNTSYMBIANDATABASE_UNIT_TEST
};

#endif // CNTSYMBIANDATABASE_H
