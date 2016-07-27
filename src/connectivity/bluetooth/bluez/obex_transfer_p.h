/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -p obex_transfer_p.h:obex_transfer.cpp org.openobex.transfer.xml org.openobex.Transfer
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef OBEX_TRANSFER_P_H_1291853849
#define OBEX_TRANSFER_P_H_1291853849

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.openobex.Transfer
 */
class OrgOpenobexTransferInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.openobex.Transfer"; }

public:
    OrgOpenobexTransferInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgOpenobexTransferInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Cancel()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Cancel"), argumentList);
    }

    inline QDBusPendingReply<QVariantMap> GetProperties()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetProperties"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace org {
  namespace openobex {
    typedef ::OrgOpenobexTransferInterface Transfer;
  }
}
#endif
