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

#ifndef QGALLERYTRACKERMETADATAEDIT_P_H
#define QGALLERYTRACKERMETADATAEDIT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qmobilityglobal.h>

#include "qgallerydbusinterface_p.h"

#include <QtCore/qstringlist.h>

QTM_BEGIN_NAMESPACE

class QGalleryTrackerMetaDataEdit : public QObject
{
    Q_OBJECT
public:
    QGalleryTrackerMetaDataEdit(
            const QGalleryDBusInterfacePointer &metaDataInterface,
            const QString &uri,
            const QString &service,
            QObject *parent = 0);
    ~QGalleryTrackerMetaDataEdit();

    int index() const { return m_index; }
    void setIndex(int index) { m_index = index; }

    QString uri() const { return m_uri; }
    QString service() const { return m_service; }

    QStringList fields() const { return m_values.keys(); }

    QString value(const QString &field) const { return m_values.value(field); }
    void setValue(const QString &field, const QString &value, const QString &oldValue) {
        m_values[field] = value; m_oldValues[field] = oldValue;}

    QMap<QString, QString> values() const { return m_values; }

    void commit();

Q_SIGNALS:
    void finished(QGalleryTrackerMetaDataEdit *edit);

public Q_SLOTS:
    void itemsInserted(int index, int count);
    void itemsRemoved(int index, int count);

private:
    int m_index;
    QGalleryDBusInterfacePointer m_metaDataInterface;
    QString m_uri;
    QString m_service;
    QMap<QString, QString> m_values;
    QMap<QString, QString> m_oldValues;
};

QTM_END_NAMESPACE

#endif
