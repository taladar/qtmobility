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

#include "meegoproximitysensor.h"

char const * const meegoproximitysensor::id("meego.proximitysensor");
bool meegoproximitysensor::m_initDone = false;

meegoproximitysensor::meegoproximitysensor(QSensor *sensor)
    : meegosensorbase(sensor), m_exClose(false)
{
    initSensor<ProximitySensorChannelInterface>(m_initDone);
    setReading<QProximityReading>(&m_reading);
    addDataRate(10,10); //TODO: fix this when we know better
}

void meegoproximitysensor::start(){
    Unsigned data(((ProximitySensorChannelInterface*)m_sensorInterface)->proximity());
    m_reading.setClose(data.x()? true: false);
    m_reading.setTimestamp(data.UnsignedData().timestamp_);
    newReadingAvailable();
    meegosensorbase::start();
}


void meegoproximitysensor::slotDataAvailable(const Unsigned& data)
{
    bool close = data.x()? true: false;
    if (close == m_exClose) return;
    m_reading.setClose(data.x()? true: false);
    m_reading.setTimestamp(data.UnsignedData().timestamp_);
    newReadingAvailable();
    m_exClose = close;
}

bool meegoproximitysensor::doConnect(){
    return (QObject::connect(m_sensorInterface, SIGNAL(dataAvailable(const Unsigned&)),
                           this, SLOT(slotDataAvailable(const Unsigned&))));
}


QString meegoproximitysensor::sensorName() const{
    return "proximitysensor";
}
