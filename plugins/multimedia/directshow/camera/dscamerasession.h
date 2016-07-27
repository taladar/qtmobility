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

#ifndef DSCAMERASESSION_H
#define DSCAMERASESSION_H

#include <QtCore/qobject.h>
#include <QTime>
#include <QUrl>
#include <QMutex>

#include <qcamera.h>
#include <QtMultimedia/qvideoframe.h>
#include <QtMultimedia/qabstractvideosurface.h>
#include <QtMultimedia/qvideosurfaceformat.h>

#include <tchar.h>
#include <dshow.h>
#include <objbase.h>
#include <initguid.h>
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ole32.lib")
#include <windows.h>

#pragma include_alias("dxtrans.h","qedit.h")
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include <qedit.h>

struct ICaptureGraphBuilder2;
struct ISampleGrabber;

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class DSVideoRenderer;
class SampleGrabberCallbackPrivate;


struct video_buffer {
    unsigned char* buffer;
    int            length;
    qint64         time;
};

typedef QMap<unsigned int, QList<QSize> > FormatResolutionMap;

class DSCameraSession : public QObject
{
    Q_OBJECT
public:
    DSCameraSession(QObject *parent = 0);
    ~DSCameraSession();

    bool deviceReady();
    bool pictureInProgress();

    // camera controls

    int framerate() const;
    void setFrameRate(int rate);
    int brightness() const;
    void setBrightness(int b);
    int contrast() const;
    void setContrast(int c);
    int saturation() const;
    void setSaturation(int s);
    int hue() const;
    void setHue(int h);
    int sharpness() const;
    void setSharpness(int s);
    int zoom() const;
    void setZoom(int z);
    bool backlightCompensation() const;
    void setBacklightCompensation(bool);
    int whitelevel() const;
    void setWhitelevel(int w);
    int rotation() const;
    void setRotation(int r);
    bool flash() const;
    void setFlash(bool f);
    bool autofocus() const;
    void setAutofocus(bool f);

    QSize frameSize() const;
    void setFrameSize(const QSize& s);
    void setDevice(const QString &device);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats();
    QVideoFrame::PixelFormat pixelFormat() const;
    void setPixelFormat(QVideoFrame::PixelFormat fmt);
    QList<QSize> supportedResolutions(QVideoFrame::PixelFormat format);

    // media control

    bool setOutputLocation(const QUrl &sink);
    QUrl outputLocation() const;
    qint64 position() const;
    int state() const;
    void record();
    void pause();
    void stop();

    void setSurface(QAbstractVideoSurface* surface);

    int captureImage(const QString &fileName);

    AM_MEDIA_TYPE StillMediaType;
    QList<video_buffer*> frames;
    SampleGrabberCallbackPrivate* StillCapCB;

    QMutex mutex;

Q_SIGNALS:
    void stateChanged(QCamera::State);
    void imageCaptured(int id, const QImage &preview);
    void imageSaved(int id, const QString &fileName);
    void readyForCaptureChanged(bool);

private Q_SLOTS:
    void captureFrame();

private:
    QVideoSurfaceFormat actualFormat;
    QList<QVideoFrame::PixelFormat> types;

    QTime timeStamp;
    bool graph;
    bool active;
    bool opened;
    bool available;
    QCamera::State m_state;
    QByteArray m_device;
    QUrl m_sink;
    DSVideoRenderer*   m_output;
    QAbstractVideoSurface* m_surface;
    QVideoFrame::PixelFormat pixelF;
    QSize m_windowSize;
    FormatResolutionMap resolutions;

    ICaptureGraphBuilder2* pBuild;
    IGraphBuilder* pGraph;
    IBaseFilter* pCap;
    IBaseFilter* pSG_Filter;
    ISampleGrabber *pSG;


    QString m_snapshot;
    int m_currentImageId;
protected:
    HRESULT getPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);
    bool createFilterGraph();
    void updateProperties();
    bool setProperties();
    bool openStream();
    void closeStream();
    bool startStream();
    void stopStream();
    void suspendStream();
    void resumeStream();
};

QT_END_NAMESPACE

QT_END_HEADER


#endif
