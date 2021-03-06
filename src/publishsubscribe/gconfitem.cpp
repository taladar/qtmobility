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

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QVariant>
#include <QtDebug>

#include "gconfitem_p.h"

#include <glib.h>
#include <gconf/gconf-value.h>
#include <gconf/gconf-client.h>

struct GConfItemPrivate {
    QString key;
    QVariant value;
    bool monitor;
    guint notify_id;

    static void notify_trampoline(GConfClient*, guint, GConfEntry *, gpointer);
};

#define withClient(c) for(GConfClient *c = (g_type_init(), gconf_client_get_default()); c; g_object_unref(c), c=NULL)

static QByteArray convertKey (QString key)
{
    if (key.startsWith('/'))
        return key.toUtf8();
    else
    {
        qWarning() << "Using dot-separated key names with GConfItem is deprecated.";
        qWarning() << "Please use" << '/' + key.replace('.', '/') << "instead of" << key;
        return '/' + key.replace('.', '/').toUtf8();
    }
}

static QString convertKey(const char *key)
{
    return QString::fromUtf8(key);
}

static QVariant convertValue(GConfValue *src)
{
    if (!src) {
        return QVariant();
    } else {
        switch (src->type) {
        case GCONF_VALUE_INVALID:
            return QVariant(QVariant::Invalid);
        case GCONF_VALUE_BOOL:
            return QVariant((bool)gconf_value_get_bool(src));
        case GCONF_VALUE_INT:
            return QVariant(gconf_value_get_int(src));
        case GCONF_VALUE_FLOAT:
            return QVariant(gconf_value_get_float(src));
        case GCONF_VALUE_STRING:
            return QVariant(QString::fromUtf8(gconf_value_get_string(src)));
        case GCONF_VALUE_LIST:
            switch (gconf_value_get_list_type(src)) {
            case GCONF_VALUE_STRING:
                {
                    QStringList result;
                    for (GSList *elts = gconf_value_get_list(src); elts; elts = elts->next)
                        result.append(QString::fromUtf8(gconf_value_get_string((GConfValue *)elts->data)));
                    return QVariant(result);
                }
            default:
                {
                    QList<QVariant> result;
                    for (GSList *elts = gconf_value_get_list(src); elts; elts = elts->next)
                        result.append(convertValue((GConfValue *)elts->data));
                    return QVariant(result);
                }
            }
        case GCONF_VALUE_SCHEMA:
        default:
            return QVariant();
        }
    }
}

static GConfValue *convertString(const QString &str)
{
    GConfValue *v = gconf_value_new (GCONF_VALUE_STRING);
    gconf_value_set_string (v, str.toUtf8().data());
    return v;
}

static GConfValueType primitiveType (const QVariant &elt)
{
    switch(elt.type()) {
    case QVariant::String:
        return GCONF_VALUE_STRING;
    case QVariant::Int:
        return GCONF_VALUE_INT;
    case QVariant::Double:
        return GCONF_VALUE_FLOAT;
    case QVariant::Bool:
        return GCONF_VALUE_BOOL;
    default:
        return GCONF_VALUE_INVALID;
    }
}

static GConfValueType uniformType(const QList<QVariant> &list)
{
    GConfValueType result = GCONF_VALUE_INVALID;

    foreach (const QVariant &elt, list) {
        GConfValueType elt_type = primitiveType (elt);

        if (elt_type == GCONF_VALUE_INVALID)
            return GCONF_VALUE_INVALID;

        if (result == GCONF_VALUE_INVALID)
            result = elt_type;
        else if (result != elt_type)
            return GCONF_VALUE_INVALID;
    }

    if (result == GCONF_VALUE_INVALID)
        return GCONF_VALUE_STRING;  // empty list.
    else
        return result;
}

static int convertValue(const QVariant &src, GConfValue **valp)
{
    GConfValue *v;

    switch(src.type()) {
    case QVariant::Invalid:
        v = NULL;
        break;
    case QVariant::Bool:
        v = gconf_value_new (GCONF_VALUE_BOOL);
        gconf_value_set_bool (v, src.toBool());
        break;
    case QVariant::Int:
        v = gconf_value_new (GCONF_VALUE_INT);
        gconf_value_set_int (v, src.toInt());
        break;
    case QVariant::Double:
        v = gconf_value_new (GCONF_VALUE_FLOAT);
        gconf_value_set_float (v, src.toDouble());
        break;
    case QVariant::String:
        v = convertString(src.toString());
        break;
    case QVariant::StringList:
        {
            GSList *elts = NULL;
            v = gconf_value_new(GCONF_VALUE_LIST);
            gconf_value_set_list_type(v, GCONF_VALUE_STRING);
            foreach (const QString &str, src.toStringList())
                elts = g_slist_prepend(elts, convertString(str));
            gconf_value_set_list_nocopy(v, g_slist_reverse(elts));
            break;
        }
    case QVariant::List:
        {
            GConfValueType elt_type = uniformType(src.toList());
            if (elt_type == GCONF_VALUE_INVALID)
                v = NULL;
            else
            {
                GSList *elts = NULL;
                v = gconf_value_new(GCONF_VALUE_LIST);
                gconf_value_set_list_type(v, elt_type);
                foreach (const QVariant &elt, src.toList())
                {
                    GConfValue *val = NULL;
                    convertValue(elt, &val);  // guaranteed to succeed.
                    elts = g_slist_prepend(elts, val);
                }
                gconf_value_set_list_nocopy(v, g_slist_reverse(elts));
            }
            break;
        }
    default:
        return 0;
    }

    *valp = v;
    return 1;
}

void GConfItemPrivate::notify_trampoline (GConfClient*,
                                             guint,
                                             GConfEntry *entry,
                                             gpointer data)
{
    GConfItem *item = (GConfItem *)data;


    item->update_value (true, entry->key, convertValue(entry->value));
}

void GConfItem::update_value (bool emit_signal, const QString& key, const QVariant& value)
{
    QVariant new_value;

    if (emit_signal) {
        emit subtreeChanged(key, value);
    }

    withClient(client) {
        GError *error = NULL;
        QByteArray k = convertKey(priv->key);
        GConfValue *v = gconf_client_get(client, k.data(), &error);

        if (error) {
            qWarning() << error->message;
            g_error_free (error);
            new_value = priv->value;
        } else {
            new_value = convertValue(v);
            if (v)
                gconf_value_free(v);
        }
    }

    if (new_value != priv->value) {
        priv->value = new_value;
        if (emit_signal)
            emit valueChanged();
    }
}

QString GConfItem::key() const
{
    return priv->key;
}

QVariant GConfItem::value() const
{
    return priv->value;
}

QVariant GConfItem::value(const QVariant &def) const
{
    if (priv->value.isNull())
        return def;
    else
        return priv->value;
}

void GConfItem::set(const QVariant &val)
{
    withClient(client) {
        QByteArray k = convertKey(priv->key);
        GConfValue *v;
        if (convertValue(val, &v)) {
            GError *error = NULL;

            if (v) {
                gconf_client_set(client, k.data(), v, &error);
                gconf_value_free(v);
            } else {
                gconf_client_unset(client, k.data(), &error);
            }

            if (error) {
                qWarning() << error->message;
                g_error_free(error);
            } else if (priv->value != val) {
                priv->value = val;
                emit valueChanged();
            }

        } else
            qWarning() << "Can't store a" << val.typeName();
    }
}

void GConfItem::unset() {
    set(QVariant());
}

void GConfItem::recursiveUnset() {
    withClient(client) {
        QByteArray k = convertKey(priv->key);
        GError *error = NULL;

        GConfUnsetFlags flags;
        gconf_client_recursive_unset(client, k.data(), flags, &error);

        if (error) {
            qWarning() << error->message;
            g_error_free(error);
        } else {
            priv->value = QVariant();
            emit valueChanged();
        }
    }
}
QList<QString> GConfItem::listDirs() const
{
    QList<QString> children;

    withClient(client) {
        QByteArray k = convertKey(priv->key);
        GSList *dirs = gconf_client_all_dirs(client, k.data(), NULL);
        for (GSList *d = dirs; d; d = d->next) {
            children.append(convertKey((char *)d->data));
            g_free (d->data);
        }
        g_slist_free (dirs);
    }

    return children;
}

QList<QString> GConfItem::listEntries() const
{
    QList<QString> children;

    withClient(client) {
        QByteArray k = convertKey(priv->key);
        GSList *entries = gconf_client_all_entries(client, k.data(), NULL);
        for (GSList *e = entries; e; e = e->next) {
            children.append(convertKey(((GConfEntry *)e->data)->key));
            gconf_entry_free ((GConfEntry *)e->data);
        }
        g_slist_free (entries);
    }

    return children;
}

GConfItem::GConfItem(const QString &key, bool monitor, QObject *parent)
    : QObject (parent)
{
    priv = new GConfItemPrivate;
    priv->key = key;
    priv->monitor = monitor;
    withClient(client) {
        update_value (false, "", QVariant());
        if (priv->monitor) {
            QByteArray k = convertKey(priv->key);
            gconf_client_add_dir (client, k.data(), GCONF_CLIENT_PRELOAD_ONELEVEL, NULL);
            priv->notify_id = gconf_client_notify_add (client, k.data(),
                                                       GConfItemPrivate::notify_trampoline, this,
                                                       NULL, NULL);
        }
    }
}

GConfItem::~GConfItem()
{
    withClient(client) {
        QByteArray k = convertKey(priv->key);
        if (priv->monitor) {
            gconf_client_notify_remove (client, priv->notify_id);
            gconf_client_remove_dir (client, k.data(), NULL);
        }
    }
    delete priv;
}
