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

#ifndef CNTVERSITLANDLINEPLUGIN_H
#define CNTVERSITLANDLINEPLUGIN_H

#include "qversitcontacthandler.h"

QTM_USE_NAMESPACE

/* QtMobility Versit plugin for VCard VOICE handling */
class CntVersitLandlinePlugin : public QVersitContactHandler
{
private:
    void exportLandLineToVoice(QList<QVersitProperty>* toBeAdded, QString number);
public:
    /* From QVersitContactImporterPropertyHandlerV2 */
    void propertyProcessed(const QVersitDocument& document,
        const QVersitProperty& property,
        const QContact& contact,
        bool* alreadyProcessed,
        QList<QContactDetail>* updatedDetails);
    void documentProcessed(const QVersitDocument& document, QContact* contact);
    
    /* From QVersitContactExporterDetailHandlerV2 */
    void detailProcessed(const QContact& contact,
        const QContactDetail& detail,
        const QVersitDocument& document,
        QSet<QString>* processedFields,
        QList<QVersitProperty>* toBeRemoved,
        QList<QVersitProperty>* toBeAdded);
    void contactProcessed(const QContact& contact, QVersitDocument* document);
};

#endif
