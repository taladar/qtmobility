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

/*
 * This file was generated by qdbusxml2cpp version 0.7 and then hand edited
 * Command line was: qdbusxml2cpp -p target_interface_p.h:target_interface.cpp com.nokia.nfc.Target.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 */

#ifndef TARGET_INTERFACE_P_H_1294360578
#define TARGET_INTERFACE_P_H_1294360578

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

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.nokia.nfc.Target
 */
class ComNokiaNfcTargetInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.nokia.nfc.Target"; }

public:
    ComNokiaNfcTargetInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~ComNokiaNfcTargetInterface();

    Q_PROPERTY(QString State READ state)
    inline QString state()
    {
        QDBusReply<QVariantMap> reply = GetProperties();
        if (!reply.isValid())
            return QString();

        return reply.value().value(QLatin1String("State")).toString();
    }

    Q_PROPERTY(QString Type READ type)
    inline QString type()
    {
        QDBusReply<QVariantMap> reply = GetProperties();
        if (!reply.isValid())
            return QString();

        return reply.value().value(QLatin1String("Type")).toString();
    }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QVariantMap> GetProperties()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetProperties"), argumentList);
    }

    inline QDBusPendingReply<> SetProperty(const QString &in0, const QDBusVariant &in1)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(in0) << qVariantFromValue(in1);
        return asyncCallWithArgumentList(QLatin1String("SetProperty"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void PropertyChanged(const QString &in0, const QDBusVariant &in1);
};

namespace com {
  namespace nokia {
    namespace nfc {
      typedef ::ComNokiaNfcTargetInterface Target;
    }
  }
}
#endif
