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
#ifndef QSIGNALINTERCEPTER_H
#define QSIGNALINTERCEPTER_H

#include "qmobilityglobal.h"
#include <qobject.h>
#include <qvariant.h>
#include <qlist.h>

QT_BEGIN_HEADER

QTM_BEGIN_NAMESPACE

class QSignalIntercepterPrivate;
class QSignalIntercepter : public QObject
{
    // Note: Do not put Q_OBJECT here.
    friend class QSlotInvoker;
    friend class QCopProxy;
public:
    QSignalIntercepter( QObject *sender, const QByteArray& signal,
                        QObject *parent=0 );
    ~QSignalIntercepter();

    QObject *sender() const;
    QByteArray signal() const;

    bool isValid() const;

    static const int QVariantId = -243;

    static int *connectionTypes( const QByteArray& member, int& nargs );

protected:
    int qt_metacall( QMetaObject::Call c, int id, void **a );
    virtual void activated( const QList<QVariant>& args ) = 0;

private:
    QSignalIntercepterPrivate *d;

    static int typeFromName( const QByteArray& name );
};

QTM_END_NAMESPACE

QT_END_HEADER

#endif
