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

#ifndef QRFCOMMSERVER_P_H
#define QRFCOMMSERVER_P_H

#include <QtGlobal>
#include <QList>
#include <qbluetoothsocket.h>

#ifdef QTM_SYMBIAN_BLUETOOTH
#include <es_sock.h>
#include <bt_sock.h>
#endif

#ifndef QT_NO_DBUS
QT_FORWARD_DECLARE_CLASS(QSocketNotifier)
#endif

QT_BEGIN_HEADER

QTM_BEGIN_NAMESPACE

class QBluetoothAddress;
class QBluetoothSocket;

#ifdef QTM_SYMBIAN_BLUETOOTH
class QBluetoothSocketPrivate;
#endif

class QRfcommServer;

class QRfcommServerPrivate
{
    Q_DECLARE_PUBLIC(QRfcommServer)

public:
    QRfcommServerPrivate();
    ~QRfcommServerPrivate();

#ifdef QTM_SYMBIAN_BLUETOOTH
    // private slots
    void _q_connected();
    void _q_socketError(QBluetoothSocket::SocketError err);
    void _q_disconnected();
#endif //QTM_SYMBIAN_BLUETOOTH

#ifndef QT_NO_DBUS
    void _q_newConnection();
#endif

public:
    QBluetoothSocket *socket;

#ifdef QTM_SYMBIAN_BLUETOOTH
    mutable QList<QBluetoothSocket *> activeSockets;
    QBluetoothSocketPrivate *ds;
#endif

    int maxPendingConnections;
    QBluetooth::SecurityFlags securityFlags;

protected:
    QRfcommServer *q_ptr;

private:
#ifndef QT_NO_DBUS
    QSocketNotifier *socketNotifier;
#endif
};

QTM_END_NAMESPACE

QT_END_HEADER

#endif