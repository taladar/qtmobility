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

#ifndef QSERVICE_INSTANCE_MANAGER
#define QSERVICE_INSTANCE_MANAGER

#include <qmobilityglobal.h>
#include "qremoteserviceregister.h"
#include "qremoteserviceregisterentry_p.h"
#include <QHash>
#include <QMutexLocker>
#include <QMetaObject>
#include <QMetaClassInfo>
#include <QUuid>
#include <QDebug>

QTM_BEGIN_NAMESPACE

struct ServiceIdentDescriptor
{
    ServiceIdentDescriptor() : globalInstance(0), globalRefCount(0)
    {
    }

    QExplicitlySharedDataPointer<QRemoteServiceRegisterEntryPrivate> entryData;

    QHash<QUuid, QObject*> individualInstances;
    QObject* globalInstance;
    QUuid globalId;
    int globalRefCount;
};

class QM_AUTOTEST_EXPORT InstanceManager : public QObject
{
    Q_OBJECT
public:
    InstanceManager(QObject *parent = 0);
    ~InstanceManager();

    bool addType(const QRemoteServiceRegister::Entry& entry);

    const QMetaObject* metaObject(const QRemoteServiceRegister::Entry& ident) const;
    QList<QRemoteServiceRegister::Entry> allEntries() const;

    int totalInstances() const;

    QObject* createObjectInstance(const QRemoteServiceRegister::Entry& entry, QUuid& instanceId);
    void removeObjectInstance(const QRemoteServiceRegister::Entry& entry, const QUuid& instanceId);

    static InstanceManager* instance();

Q_SIGNALS:
    void allInstancesClosed();
    void instanceClosed(const QRemoteServiceRegister::Entry&);
    void instanceClosed(const QRemoteServiceRegister::Entry&, const QUuid&);

private:
    mutable QMutex lock;
    QHash<QRemoteServiceRegister::Entry, ServiceIdentDescriptor> metaMap;
};



QTM_END_NAMESPACE


#endif //QSERVICE_INSTANCE_MANAGER
