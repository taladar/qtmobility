/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "thumbnailmodel.h"

#include <QtCore/qurl.h>
#include <QtGui/qpixmap.h>

#if defined(Q_WS_MAEMO_5)
const QSize ThumbnailModel::thumbnailSize(124, 124);
#else
const QSize ThumbnailModel::thumbnailSize(128, 128);
#endif

ThumbnailModel::ThumbnailModel(QAbstractGallery *gallery, QObject *parent)
    : QGalleryQueryModel(gallery, parent)
{
    connect(&cache, SIGNAL(thumbnailReady()), this, SLOT(thumbnailLoaded()));
}

ThumbnailModel::~ThumbnailModel()
{
}

QVariant ThumbnailModel::data(const QModelIndex &index, int role) const
{
    return role == Qt::DecorationRole && index.isValid()
            ? cache.thumbnail(imageUrl(index))
            : QGalleryQueryModel::data(index, role);
}

QUrl ThumbnailModel::imageUrl(const QModelIndex &index) const
{
    return itemUrl(index);
}

void ThumbnailModel::thumbnailLoaded()
{
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() -1));
}
