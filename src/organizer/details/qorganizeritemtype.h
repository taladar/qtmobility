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

#ifndef QORGANIZERITEMTYPE_H
#define QORGANIZERITEMTYPE_H

#include <QString>

#include "qtorganizerglobal.h"
#include "qorganizeritemdetail.h"
#include "qorganizeritem.h"

QTM_BEGIN_NAMESPACE

/* Leaf class */
class Q_ORGANIZER_EXPORT QOrganizerItemType : public QOrganizerItemDetail
{
public:
#ifdef Q_QDOC
    static const QLatin1Constant DefinitionName;
    static const QLatin1Constant FieldType;
    static const QLatin1Constant TypeEvent;
    static const QLatin1Constant TypeTodo;
    static const QLatin1Constant TypeJournal;
    static const QLatin1Constant TypeNote;
    static const QLatin1Constant TypeEventOccurrence;
    static const QLatin1Constant TypeTodoOccurrence;
#else
    Q_DECLARE_CUSTOM_ORGANIZER_DETAIL(QOrganizerItemType, "Type")
    Q_DECLARE_LATIN1_CONSTANT(FieldType, "Type");
    Q_DECLARE_LATIN1_CONSTANT(TypeEvent, "Event");
    Q_DECLARE_LATIN1_CONSTANT(TypeEventOccurrence, "EventOccurrence");
    Q_DECLARE_LATIN1_CONSTANT(TypeTodo, "Todo");
    Q_DECLARE_LATIN1_CONSTANT(TypeTodoOccurrence, "TodoOccurrence");
    Q_DECLARE_LATIN1_CONSTANT(TypeJournal, "Journal");
    Q_DECLARE_LATIN1_CONSTANT(TypeNote, "Note");
#endif

    void setType(const QString& type) {setValue(FieldType, type);}
    QString type() const {return value(FieldType);}
};

QTM_END_NAMESPACE

#endif

