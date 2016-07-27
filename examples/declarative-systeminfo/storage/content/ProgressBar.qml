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

import Qt 4.7

Item {
    id: progressbar

    property int minimum: 0
    property int maximum: 100
    property int value: 0
    property int maxval: 0
    property alias color: gradient1.color
    property alias secondColor: gradient2.color

    width: 30; height: 250
    clip: true

    BorderImage {
        source: "background.png"
        width: parent.width; height: parent.height
        border { left: 4; top: 4; right: 4; bottom: 4 }
    }

    Rectangle {
        id: highlight

        property int widthDest: ((progressbar.height * (value - minimum)) / (maximum - minimum) )

        height: highlight.widthDest
        Behavior on height { SmoothedAnimation { velocity: 1200 } }

        anchors { left: parent.left; right: parent.right; bottom: parent.bottom; leftMargin: 2; rightMargin: 2; bottomMargin: 2 }
//        anchors { left: parent.left; top: parent.top; bottom: parent.bottom; leftMargin: 3; topMargin: 3; bottomMargin: 3 }
        radius: 1
        gradient: Gradient {
            GradientStop { id: gradient1; position: 0.0 }
            GradientStop { id: gradient2; position: 1.0 }
        }

    }
    Text {
        anchors { /*right: highlight.right; rightMargin: 6; */horizontalCenter: parent.horizontalCenter }
        color: "black"
        font.bold: true
        text: maxval + '%'//Math.floor((value - minimum) / (maximum - minimum) * 100) + '%'
    }
}