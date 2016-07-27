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


#ifndef QORGANIZERITEMLOCATION_H
#define QORGANIZERITEMLOCATION_H

#include <QString>

#include "qtorganizerglobal.h"
#include "qorganizeritemdetail.h"
#include "qorganizeritem.h"
#include "qorganizeritemfilter.h"

QTM_BEGIN_NAMESPACE

/* Leaf class */
class Q_ORGANIZER_EXPORT QOrganizerItemLocation : public QOrganizerItemDetail
{
public:
#ifdef Q_QDOC
    static const QLatin1Constant DefinitionName;
    static const QLatin1Constant FieldLatitude;
    static const QLatin1Constant FieldLongitude;
    static const QLatin1Constant FieldLabel;
#else
    Q_DECLARE_CUSTOM_ORGANIZER_DETAIL(QOrganizerItemLocation, "Location")
    Q_DECLARE_LATIN1_CONSTANT(FieldLatitude, "Latitude");
    Q_DECLARE_LATIN1_CONSTANT(FieldLongitude, "Longitude");
    Q_DECLARE_LATIN1_CONSTANT(FieldLabel, "Label");
#endif

    void setLatitude(double latitude) {setValue(FieldLatitude, latitude);}
    double latitude() const {return value<double>(FieldLatitude);}
    void setLongitude(double longitude) {setValue(FieldLongitude, longitude);}
    double longitude() const {return value<double>(FieldLongitude);}
    void setLabel(const QString& label) {setValue(FieldLabel, label);}
    QString label() const {return value(FieldLabel);}

    // Convenience filter
    static QOrganizerItemFilter match(const QString& substring);
};

QTM_END_NAMESPACE

#endif

