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

#ifndef S60CAMERALOCKSCONTROL_H
#define S60CAMERALOCKSCONTROL_H

#include <QtCore/qobject.h>
#include "qcameralockscontrol.h"

QT_USE_NAMESPACE

class S60CameraService;
class S60ImageCaptureSession;
class S60CameraSettings;
class S60CameraFocusControl;

/*
 * Control for searching and locking 3A algorithms (AutoFocus, AutoExposure
 * and AutoWhitebalance).
 */
class S60CameraLocksControl : public QCameraLocksControl
{
    Q_OBJECT

public: // Contructors & Destrcutor

    S60CameraLocksControl(QObject *parent = 0);
    S60CameraLocksControl(S60CameraService *service,
                          S60ImageCaptureSession *session,
                          QObject *parent = 0);
    ~S60CameraLocksControl();

public: // QCameraLocksControl

    QCamera::LockTypes supportedLocks() const;

    QCamera::LockStatus lockStatus(QCamera::LockType lock) const;

    void searchAndLock(QCamera::LockTypes locks);
    void unlock(QCamera::LockTypes locks);

/*
Q_SIGNALS: // QCameraLocksControl

    void lockStatusChanged(QCamera::LockType type,
                           QCamera::LockStatus status,
                           QCamera::LockChangeReason reason);
*/

private slots: // Internal Slots

    void exposureStatusChanged(QCamera::LockStatus status, QCamera::LockChangeReason reason);
    void focusStatusChanged(QCamera::LockStatus status, QCamera::LockChangeReason reason);
    void resetAdvancedSetting();

private: // Internal

    // Focus
    void startFocusing();
    void cancelFocusing();

    // Exposure
    void startExposureLocking();
    void cancelExposureLocking();

private: // Data

    S60ImageCaptureSession  *m_session;
    S60CameraService        *m_service;
    S60CameraSettings       *m_advancedSettings;
    S60CameraFocusControl   *m_focusControl;
    QCamera::LockStatus     m_focusStatus;
    QCamera::LockStatus     m_exposureStatus;
    QCamera::LockStatus     m_whiteBalanceStatus;
};

#endif // S60CAMERALOCKSCONTROL_H
