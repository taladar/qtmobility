/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QNDEFRECORD_H
#define QNDEFRECORD_H

#include <qmobilityglobal.h>

#include <QtCore/QSharedDataPointer>
#include <QtCore/QByteArray>

QT_BEGIN_HEADER

QTM_BEGIN_NAMESPACE

class QNdefRecordPrivate;

class Q_CONNECTIVITY_EXPORT QNdefRecord
{
public:
    enum TypeNameFormat {
        Empty = 0x00,
        NfcRtd = 0x01,
        Mime = 0x02,
        Uri = 0x03,
        ExternalRtd = 0x04,
        Unknown = 0x05
    };

    QNdefRecord();
    ~QNdefRecord();

    QNdefRecord(const QNdefRecord &other);
    QNdefRecord &operator=(const QNdefRecord &other);

    void setTypeNameFormat(TypeNameFormat typeNameFormat);
    TypeNameFormat typeNameFormat() const;

    void setType(const QByteArray &type);
    QByteArray type() const;

    void setId(const QByteArray &id);
    QByteArray id() const;

    void setPayload(const QByteArray &payload);
    QByteArray payload() const;

    bool isEmpty() const;

    template <typename T>
    inline bool isRecordType() const
    {
        T dummy;
        return (typeNameFormat() == dummy.typeNameFormat() && type() == dummy.type());
    }

    bool operator==(const QNdefRecord &other) const;
    inline bool operator!=(const QNdefRecord &other) const { return !operator==(other); }

protected:
    QNdefRecord(const QNdefRecord &other, TypeNameFormat typeNameFormat, const QByteArray &type);
    QNdefRecord(TypeNameFormat typeNameFormat, const QByteArray &type);

private:
    QSharedDataPointer<QNdefRecordPrivate> d;
};

#define Q_DECLARE_NDEF_RECORD(className, typeNameFormat, type, initialPayload) \
    className() : QNdefRecord(typeNameFormat, type) { setPayload(initialPayload); } \
    className(const QNdefRecord &other) : QNdefRecord(other, typeNameFormat, type) { }

#define Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(className, typeNameFormat_, type_) \
    QTM_BEGIN_NAMESPACE \
    template<> inline bool QNdefRecord::isRecordType<className>() const\
    { \
        return (typeNameFormat() == typeNameFormat_ && type() == type_); \
    } \
    QTM_END_NAMESPACE

uint qHash(const QNdefRecord &key);

QTM_END_NAMESPACE

QT_END_HEADER

#endif // QNDEFRECORD_H
