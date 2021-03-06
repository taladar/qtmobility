/****************************************************************************
**
** Copyright (C) 2009-2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef WLANINFO_S60_H
#define WLANINFO_S60_H

#include <QObject>
#include <QTimer>

#include <wlanmgmtclient.h>
#include <wlanmgmtinterface.h>
#include <wlanmgmtcommon.h>

class CWlanInfo : public QObject, public MWlanMgmtNotifications
{
    Q_OBJECT
public:
    CWlanInfo(QObject *parent = 0);
    ~CWlanInfo();

    QString wlanNetworkName() const;
    int wlanNetworkSignalStrength() const;
    bool wlanNetworkConnectionStatus() const;
    void FreeResources();

private:
    void stopPolling();

protected: // MWlanMgmtNotifications
    void ConnectionStateChanged(TWlanConnectionMode aNewState);
    void BssidChanged(TWlanBssid& aNewBSSID) {};
    void BssLost() {};
    void BssRegained() {};
    void NewNetworksDetected() {};
    void OldNetworksLost() {};
    void TransmitPowerChanged(TUint) {};
    void RssChanged(TWlanRssClass , TUint strength) {};

private slots:
    void checkWlanInfo();

signals:
    void wlanNetworkNameChanged();
    void wlanNetworkSignalStrengthChanged();
    void wlanNetworkStatusChanged();

private:
    CWlanMgmtClient *m_wlanMgmtClient;
    bool m_wlanStatus;
    QString m_wlanSsid;
    int m_wlanSignalStrength;
    QTimer *m_timer;
};

#endif //WLANINFO_S60_H
