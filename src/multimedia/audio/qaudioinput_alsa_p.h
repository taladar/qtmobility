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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//


#ifndef QAUDIOINPUTALSA_H
#define QAUDIOINPUTALSA_H

#include <alsa/asoundlib.h>

#include <QtCore/qfile.h>
#include <QtCore/qdebug.h>
#include <QtCore/qtimer.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qdatetime.h>

#include "qaudio.h"
#include "qaudiodeviceinfo.h"
#include "qaudiosystem.h"

QT_BEGIN_NAMESPACE

class InputPrivate;

class RingBuffer
{
public:
    RingBuffer();

    void resize(int size);

    int bytesOfDataInBuffer() const;
    int freeBytes() const;

    const char *availableData() const;
    int availableDataBlockSize() const;
    void readBytes(int bytes);

    void write(char *data, int len);

private:
    int m_head;
    int m_tail;

    QByteArray m_data;
};

class QAudioInputPrivate : public QAbstractAudioInput
{
    Q_OBJECT
public:
    QAudioInputPrivate(const QByteArray &device);
    ~QAudioInputPrivate();

    qint64 read(char* data, qint64 len);

    void start(QIODevice* device);
    QIODevice* start();
    void stop();
    void reset();
    void suspend();
    void resume();
    int bytesReady() const;
    int periodSize() const;
    void setBufferSize(int value);
    int bufferSize() const;
    void setNotifyInterval(int milliSeconds);
    int notifyInterval() const;
    qint64 processedUSecs() const;
    qint64 elapsedUSecs() const;
    QAudio::Error error() const;
    QAudio::State state() const;
    void setFormat(const QAudioFormat& fmt);
    QAudioFormat format() const;
    bool resuming;
    snd_pcm_t* handle;
    qint64 totalTimeValue;
    QIODevice* audioSource;
    QAudioFormat settings;
    QAudio::Error errorState;
    QAudio::State deviceState;

private slots:
    void userFeed();
    bool deviceReady();

private:
    int checkBytesReady();
    int xrun_recovery(int err);
    int setFormat();
    bool open();
    void close();
    void drain();

    QTimer* timer;
    QTime timeStamp;
    QTime clockStamp;
    qint64 elapsedTimeOffset;
    int intervalTime;
    RingBuffer ringBuffer;
    int bytesAvailable;
    QByteArray m_device;
    bool pullMode;
    int buffer_size;
    int period_size;
    unsigned int buffer_time;
    unsigned int period_time;
    snd_pcm_uframes_t buffer_frames;
    snd_pcm_uframes_t period_frames;
    snd_async_handler_t* ahandler;
    snd_pcm_access_t access;
    snd_pcm_format_t pcmformat;
    snd_timestamp_t* timestamp;
    snd_pcm_hw_params_t *hwparams;
};

class InputPrivate : public QIODevice
{
    Q_OBJECT
public:
    InputPrivate(QAudioInputPrivate* audio);
    ~InputPrivate();

    qint64 readData( char* data, qint64 len);
    qint64 writeData(const char* data, qint64 len);

    void trigger();
private:
    QAudioInputPrivate *audioDevice;
};

QT_END_NAMESPACE

#endif
