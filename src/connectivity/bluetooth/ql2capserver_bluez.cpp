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

#include "ql2capserver.h"
#include "ql2capserver_p.h"
#include "qbluetoothsocket.h"

#include <QtCore/QSocketNotifier>

#include <QtCore/QDebug>

#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

#include <errno.h>

QTM_BEGIN_NAMESPACE

static inline void convertAddress(quint64 from, quint8 (&to)[6])
{
    to[0] = (from >> 0) & 0xff;
    to[1] = (from >> 8) & 0xff;
    to[2] = (from >> 16) & 0xff;
    to[3] = (from >> 24) & 0xff;
    to[4] = (from >> 32) & 0xff;
    to[5] = (from >> 40) & 0xff;
}

QL2capServerPrivate::QL2capServerPrivate()
:  pending(false), maxPendingConnections(1), socketNotifier(0)
{
    socket = new QBluetoothSocket(QBluetoothSocket::L2capSocket);
}

QL2capServerPrivate::~QL2capServerPrivate()
{
    qDebug() << "Deleted";
    delete socketNotifier;

    delete socket;
}

void QL2capServerPrivate::_q_newConnection()
{
    // disable socket notifier until application calls nextPendingConnection().
    socketNotifier->setEnabled(false);

    emit q_ptr->newConnection();
}

void QL2capServer::close()
{
    Q_D(QL2capServer);

    d->socket->close();
}

bool QL2capServer::listen(const QBluetoothAddress &address, quint16 port)
{
    Q_D(QL2capServer);

    int sock = d->socket->socketDescriptor();
    if (sock < 0)
        return false;

    sockaddr_l2 addr;

    memset(&addr, 0, sizeof(sockaddr_l2));
    addr.l2_family = AF_BLUETOOTH;
    addr.l2_psm = port;

    if (!address.isNull())
        convertAddress(address.toUInt64(), addr.l2_bdaddr.b);
    else
        convertAddress(Q_UINT64_C(0), addr.l2_bdaddr.b);

    if (::bind(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(sockaddr_l2)) < 0)
        return false;

    if (::listen(sock, d->maxPendingConnections) < 0)
        return false;

    d->socket->setSocketState(QBluetoothSocket::ListeningState);

    delete d->socketNotifier;

    d->socketNotifier = new QSocketNotifier(d->socket->socketDescriptor(),
                                            QSocketNotifier::Read);
    connect(d->socketNotifier, SIGNAL(activated(int)), this, SLOT(_q_newConnection()));

    return true;
}

void QL2capServer::setMaxPendingConnections(int numConnections)
{
    Q_D(QL2capServer);

    if (d->socket->state() == QBluetoothSocket::UnconnectedState)
        d->maxPendingConnections = numConnections;
}

bool QL2capServer::hasPendingConnections() const
{
    Q_D(const QL2capServer);

    if (!d || !d->socketNotifier)
        return false;

    // if the socket notifier is disable there is a pending connection waiting for us to accept.
    return !d->socketNotifier->isEnabled();
}

QBluetoothSocket *QL2capServer::nextPendingConnection()
{
    Q_D(QL2capServer);

    if (!hasPendingConnections())
        return 0;

    sockaddr_l2 addr;
    socklen_t length = sizeof(sockaddr_l2);

    int pending = ::accept(d->socket->socketDescriptor(),
                           reinterpret_cast<sockaddr *>(&addr), &length);
    if (pending >= 0) {
        QBluetoothSocket *newSocket = new QBluetoothSocket;
        newSocket->setSocketDescriptor(pending, QBluetoothSocket::L2capSocket);

        d->socketNotifier->setEnabled(true);

        return newSocket;
    } else {
//        qDebug() << "Hmm, could have sworn there was a connection waiting to be accepted!" << errno;
        d->socketNotifier->setEnabled(true);
    }

    return 0;
}

QBluetoothAddress QL2capServer::serverAddress() const
{
    Q_D(const QL2capServer);

    return d->socket->localAddress();
}

quint16 QL2capServer::serverPort() const
{
    Q_D(const QL2capServer);

    return d->socket->localPort();
}

void QL2capServer::setSecurityFlags(QBluetooth::SecurityFlags security)
{
    Q_D(QL2capServer);

    int lm = 0;
    if(security == QBluetooth::NoSecurity){
        lm = 0;
    }
    if(security.testFlag(QBluetooth::Authorization)){
        lm |= L2CAP_LM_AUTH;
    }
    if(security.testFlag(QBluetooth::Authentication)){
        lm |= L2CAP_LM_TRUSTED;
    }
    if(security.testFlag(QBluetooth::Encryption)){
        lm |= L2CAP_LM_ENCRYPT;
    }
    if(security.testFlag(QBluetooth::Secure)){
        lm |= L2CAP_LM_SECURE;
    }

    if(setsockopt(d->socket->socketDescriptor(), SOL_L2CAP, L2CAP_LM, &lm, sizeof(lm)) < 0){
        qWarning() << "Failed to set socket option, closing socket for safety" << errno;
        qWarning() << "Error: " << strerror(errno);
        d->socket->close();
    }
}

QBluetooth::SecurityFlags QL2capServer::securityFlags() const
{
    Q_D(const QL2capServer);

    int lm = 0;
    int len = sizeof(lm);
    int security = QBluetooth::NoSecurity;

    if(getsockopt(d->socket->socketDescriptor(), SOL_L2CAP, L2CAP_LM, &lm, (socklen_t *)&len) < 0) {
        qWarning() << "Failed to get security flags" << strerror(errno);
        return QBluetooth::NoSecurity;
    }

    if(lm & L2CAP_LM_SECURE)
        security |= QBluetooth::Secure;

    if(lm & L2CAP_LM_ENCRYPT)
        security |= QBluetooth::Encryption;

    if(lm & L2CAP_LM_TRUSTED)
        security |= QBluetooth::Authentication;

    if(lm & L2CAP_LM_AUTH)
        security |= QBluetooth::Authorization;

    return static_cast<QBluetooth::SecurityFlags>(security);
}


QTM_END_NAMESPACE
