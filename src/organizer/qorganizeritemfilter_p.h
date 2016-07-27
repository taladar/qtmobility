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

#ifndef QORGANIZERITEMFILTER_P_H
#define QORGANIZERITEMFILTER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qorganizeritemfilter.h"
#include <QSharedData>
#include <QDataStream>

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#endif

/* Boiler plate code */
#define Q_IMPLEMENT_ORGANIZERITEMFILTER_PRIVATE(Class) \
    Class##Private* Class::d_func() { return reinterpret_cast<Class##Private *>(d_ptr.data()); } \
    const Class##Private* Class::d_func() const { return reinterpret_cast<const Class##Private *>(d_ptr.constData()); } \
    Class::Class(const QOrganizerItemFilter& other) : QOrganizerItemFilter() { Class##Private::copyIfPossible(d_ptr, other); }

#define Q_IMPLEMENT_ORGANIZERITEMFILTER_VIRTUALCTORS(Class, Type) \
    QOrganizerItemFilterPrivate* clone() const { return new Class##Private(*this); } \
    virtual QOrganizerItemFilter::FilterType type() const {return Type;} \
    static void copyIfPossible(QSharedDataPointer<QOrganizerItemFilterPrivate>& d_ptr, const QOrganizerItemFilter& other) \
    { \
        if (other.type() == Type) \
            d_ptr = extract_d(other); \
        else \
            d_ptr = new Class##Private; \
    }

QTM_BEGIN_NAMESPACE
class QOrganizerItemFilterPrivate : public QSharedData
{
public:
    QOrganizerItemFilterPrivate()
    {
    }

    virtual ~QOrganizerItemFilterPrivate()
    {
    }

    virtual bool compare(const QOrganizerItemFilterPrivate* other) const = 0;
    virtual QDataStream& outputToStream(QDataStream& stream, quint8 formatVersion) const = 0;
    virtual QDataStream& inputFromStream(QDataStream& stream, quint8 formatVersion) = 0;
#ifndef QT_NO_DEBUG_STREAM
    // NOTE: on platforms where Qt is built without debug streams enabled, vtable will differ!
    virtual QDebug& debugStreamOut(QDebug& dbg) const = 0;
#endif
    virtual QOrganizerItemFilterPrivate* clone() const = 0;
    virtual QOrganizerItemFilter::FilterType type() const = 0;

    /* Helper functions for C++ protection rules */
    static const QSharedDataPointer<QOrganizerItemFilterPrivate>& extract_d(const QOrganizerItemFilter& other) {return other.d_ptr;}
};
QTM_END_NAMESPACE

QT_BEGIN_NAMESPACE
#if defined(Q_CC_MWERKS)
// This results in multiple symbol definition errors on all other compilers
// but not having a definition here results in an attempt to use the unspecialized
// clone (which fails because of the pure virtuals above)
template<> QTM_PREPEND_NAMESPACE(QOrganizerItemFilterPrivate) *QSharedDataPointer<QTM_PREPEND_NAMESPACE(QOrganizerItemFilterPrivate)>::clone()
{
    return d->clone();
}
#else
template<> QTM_PREPEND_NAMESPACE(QOrganizerItemFilterPrivate) *QSharedDataPointer<QTM_PREPEND_NAMESPACE(QOrganizerItemFilterPrivate)>::clone();
#endif
QT_END_NAMESPACE


#endif
