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

#ifndef QGALLERYDBUSINTERFACE_P_H
#define QGALLERYDBUSINTERFACE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qmobilityglobal.h>

#include <QtCore/qshareddata.h>
#include <QtDBus/qdbusabstractinterface.h>

QTM_BEGIN_NAMESPACE

class QM_AUTOTEST_EXPORT QGalleryDBusInterface : public QDBusAbstractInterface, public QSharedData
{
    Q_OBJECT
public:
    QGalleryDBusInterface(
            const QString &service,
            const QString &path,
            const char *interface,
            const QDBusConnection &connection = QDBusConnection::sessionBus(),
            QObject *parent = 0)
        : QDBusAbstractInterface(service, path, interface, connection, parent) {}
};

typedef QExplicitlySharedDataPointer<QGalleryDBusInterface> QGalleryDBusInterfacePointer;

class QGalleryTrackerDaemonDBusInterface : public QGalleryDBusInterface
{
    Q_OBJECT
public:
    QGalleryTrackerDaemonDBusInterface(
            const QString &service,
            const QString &path,
            const char *interface,
            const QDBusConnection &connection = QDBusConnection::sessionBus(),
            QObject *parent = 0)
        : QGalleryDBusInterface(service, path, interface, connection, parent) {}

Q_SIGNALS:
    void IndexFinished(double elapsed);
    void ServiceStatisticsUpdated(const QVector<QStringList> &statistics);
};

class QGalleryDBusInterfaceFactory
{
public:
    virtual ~QGalleryDBusInterfaceFactory() {}

    virtual QGalleryDBusInterfacePointer daemonInterface() = 0;
    virtual QGalleryDBusInterfacePointer metaDataInterface() = 0;
    virtual QGalleryDBusInterfacePointer searchInterface() = 0;
    virtual QGalleryDBusInterfacePointer fileInterface() = 0;
};

QTM_END_NAMESPACE

#endif
