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
    id: toolbar
    property string button1Label
    property string button2Label
    property string button3Label
    signal button1Clicked
    signal button2Clicked
    signal button3Clicked
    Rectangle {
        id: button1
        border.color: 'black'
        border.width: 1
        anchors.left: parent.left; y: 3; width: parent.width/3; height: 32
        Text {id: button1Text; text:  button1Label; color: "black"}
        MouseArea {
            anchors.fill: parent
            onClicked: toolbar.button1Clicked()
        }
    }
    Rectangle {
        id: button2
        border.color: 'black'
        border.width: 1
        anchors.left: button1.right; y: 3; anchors.leftMargin: -1; width: parent.width/3 - anchors.leftMargin; height: 32
        Text {id: button2Text; text:  button2Label; color: "black"}
        MouseArea {
            anchors.fill: parent
            onClicked: toolbar.button2Clicked()
        }
    }
    Rectangle {
        id: button3
        border.color: 'black'
        border.width: 1
        anchors.left: button2.right; anchors.leftMargin: -1; y: 3; width: parent.width/3 - anchors.leftMargin; height: 32
        Text {id: button3Text; text:  button3Label; color: "black"}
        MouseArea {
            anchors.fill: parent
            onClicked: toolbar.button3Clicked()
        }
    }
}
