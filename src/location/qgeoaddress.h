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

#ifndef QGEOADDRESS_H
#define QGEOADDRESS_H

#include "qmobilityglobal.h"
#include <QSharedDataPointer>

class QString;

QTM_BEGIN_NAMESPACE

class QGeoAddressPrivate;
class Q_LOCATION_EXPORT QGeoAddress
{
public:
    QGeoAddress();
    QGeoAddress(const QGeoAddress &other);
    ~QGeoAddress();

    QGeoAddress &operator=(const QGeoAddress &other);
    bool operator==(const QGeoAddress &other) const;
    bool operator!=(const QGeoAddress &other) const {
        return !(other == *this);
    }

    QString country() const;
    void setCountry(const QString &country);

    QString countryCode() const;
    void setCountryCode(const QString &countryCode);

    QString state() const;
    void setState(const QString &state);

    QString county() const;
    void setCounty(const QString &county);

    QString city() const;
    void setCity(const QString &city);

    QString district() const;
    void setDistrict(const QString &district);

    QString street() const;
    void setStreet(const QString &street);

    QString postcode() const;
    void setPostcode(const QString &postcode);

    bool isEmpty() const;
    void clear();

private:
    QSharedDataPointer<QGeoAddressPrivate> d;
};

QTM_END_NAMESPACE

#endif
