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

#include "dialog.h"
#include <QMessageBox>
#include <QTimer>
#include <math.h>
#include <QDateTime>

#ifndef Q_CC_MINGW
#ifdef Q_OS_WIN
// silly MS
inline float round(float x) {
      return floor(x+0.5);
   }
#endif
#endif
Dialog::Dialog() :
    QWidget(),
    saver(NULL), systemInfo(NULL), di(NULL), ni(NULL),sti(NULL),bi(NULL),dis(NULL),
    alt1(0),alt2(0),alt3(0)
{
    setupUi(this);
    setupGeneral();

    connect(comboBox,SIGNAL(activated(int)),this,SLOT(tabChanged(int)));
    connect(versionComboBox,SIGNAL(activated(int)), this,SLOT(getVersion(int)));
    connect(featureComboBox,SIGNAL(activated(int)), this,SLOT(getFeature(int)));
}

Dialog::~Dialog()
{
    delete systemInfo;
    delete di;
    delete saver;
    delete dis;
}

void Dialog::parseArguments()
{
    bool ok;
    int tab = qApp->arguments().at(1).toInt(&ok);
    if (ok) {
        comboBox->setCurrentIndex(tab);
        stackedWidget->setCurrentIndex(tab);
        tabChanged(tab);
    }
}


void Dialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
#if !defined(Q_WS_MAEMO_6)
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
#endif
}

void Dialog::tabChanged(int index)
{
    if (lastTab == 4 && index !=4) {
        disconnect(bi,SIGNAL(currentFlowChanged(int)),0,0);
    }
    switch(index) {
    case 0:
        setupGeneral();
        break;
    case 1:
        setupGeneral();
        break;
    case 2:
        setupDevice();
        break;
    case 3:
        setupDevice();
        break;
    case 4:
        setupBattery();
        break;
    case 5:
        setupDisplay();
        break;
    case 6:
        setupStorage();
        break;
    case 7:
        setupNetwork();
        break;
    case 8:
        setupNetwork();
        break;
    case 9:
        setupNetwork();
        break;
    case 10:
        setupSaver();
        break;
    case 11:
        setupAlignedTimers();
        break;
    };

    lastTab = index;
}

void Dialog::setupGeneral()
{
    delete systemInfo;
//! [lang]
    systemInfo = new QSystemInfo(this);
    curLanguageLineEdit->setText( systemInfo->currentLanguage());
//! [lang]
    languagesListWidget->clear();
    languagesListWidget->insertItems(0,systemInfo->availableLanguages());
    countryCodeLabel->setText(systemInfo->currentCountryCode());
}

void Dialog::setupDevice()
{
    delete di;
//! [createdi]
    di = new QSystemDeviceInfo(this);
//! [createdi]


    ImeiLabel->setText(di->imei());
    imsiLabel->setText(di->imsi());
//! [manuf-id]
    manufacturerLabel->setText(di->manufacturer());
//! [manuf-id]
    modelLabel->setText(di->model());
//! [productName]
    productLabel->setText(di->productName());
//! [productName]

    deviceLockPushButton->setChecked(di->isDeviceLocked());

    updateSimStatus();
    updateThermalState();
    updateProfile();

    connect(di, SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)),
        this, SLOT(updateProfile(QSystemDeviceInfo::Profile)));


//! [inputMethod flags]
    QSystemDeviceInfo::InputMethodFlags methods = di->inputMethodType();
    QStringList inputs;
    if ((methods & QSystemDeviceInfo::Keys)=QSystemDeviceInfo::Keypad){
        inputs << "Keys";
    }
    if ((methods & QSystemDeviceInfo::Keypad)=QSystemDeviceInfo::Keypad) {
        inputs << "Keypad";
    }
//! [inputMethod flags]
    if ((methods & QSystemDeviceInfo::Keyboard)=QSystemDeviceInfo::Keyboard) {
        inputs << "Keyboard";
    }
    if ((methods & QSystemDeviceInfo::SingleTouch)=QSystemDeviceInfo::SingleTouch) {
        inputs << "Touch Screen";
    }
    if ((methods & QSystemDeviceInfo::MultiTouch)=QSystemDeviceInfo::MultiTouch) {
        inputs << "Multi touch";
    }
    if ((methods & QSystemDeviceInfo::Mouse)=QSystemDeviceInfo::Mouse){
        inputs << "Mouse";
    }

    inputMethodLabel->setText(inputs.join(" "));

    bluetoothPowerLabel->setText((di->currentBluetoothPowerState() ? "On" : "Off"));
    connect(di,SIGNAL(bluetoothStateChanged(bool)), this,SLOT(bluetoothChanged(bool)));

    uniqueIDLabel->setText(di->uniqueDeviceID());

    updateKeyboard(di->keyboardTypes());

    keyboardFlipRadioButton->setChecked(di->isKeyboardFlippedOpen());
    connect(di,SIGNAL(keyboardFlipped(bool)),this,SLOT(keyboardFlipped(bool)));

    wirelessKeyboardConnectedRadioButton->setChecked(di->isWirelessKeyboardConnected());

    QSystemDeviceInfo::LockTypeFlags locktype = di->lockStatus();
    lockStateLabel->setText(lockStateToString(locktype));

    oldLockStatus = QSystemDeviceInfo::UnknownLock;
    lockStateLabel_2->setText(lockStateToString(oldLockStatus));
    oldLockStatus = locktype;

    connect(di,SIGNAL(lockStatusChanged(QSystemDeviceInfo::LockTypeFlags)),
            this,SLOT(lockStatusChanged(QSystemDeviceInfo::LockTypeFlags)),Qt::UniqueConnection);
    updateDeviceLockedState();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDeviceLockedState()));
    timer->start(1000);
}

void Dialog::updateKeyboard(QSystemDeviceInfo::KeyboardTypeFlags type)
{

    if ((type & QSystemDeviceInfo::SoftwareKeyboard)) {
        softkeysRadioButton->setChecked(true);
    } else if ((type & QSystemDeviceInfo::ITUKeypad)) {
        ituRadioButton->setChecked(true);
    } else if ((type & QSystemDeviceInfo::HalfQwertyKeyboard)) {
        halfKeysRadioButton->setChecked(true);
    } else if ((type & QSystemDeviceInfo::FullQwertyKeyboard)) {
        qwertyKeysRadioButton->setChecked(true);
    } else if ((type & QSystemDeviceInfo::WirelessKeyboard)) {
        wirelessRadioButton->setChecked(true);
    } else {
        uknownKeysRadioButton->setChecked(true);
    }

    keyboardLightCheckBox->setChecked(di->keypadLightOn(QSystemDeviceInfo::PrimaryKeypad));
}

void Dialog::updateDeviceLockedState()
{
    if (di)
        deviceLockPushButton->setChecked(di->isDeviceLocked());
}

void Dialog::updateProfile(QSystemDeviceInfo::Profile /*profile*/)
{
   updateProfile();
}


void Dialog::setupDisplay()
{
    if (!dis) {
        dis = new QSystemDisplayInfo(this);
        connect(dis,SIGNAL(orientationChanged(QSystemDisplayInfo::DisplayOrientation)),
                this,SLOT(orientationChanged(QSystemDisplayInfo::DisplayOrientation )));

    }
    brightnessLabel->setText(QString::number(dis->displayBrightness(0)));
    colorDepthLabel->setText(QString::number(dis->colorDepth((0))));

    orientationChanged(dis->orientation(0));

    contrastLabel->setText(QString::number(dis->contrast((0))));

    dpiWidthLabel->setText(QString::number(dis->getDPIWidth(0)));
    dpiHeightLabel->setText(QString::number(dis->getDPIHeight((0))));

    physicalHeightLabel->setText(QString::number(dis->physicalHeight(0)));
    physicalWidthLabel->setText(QString::number(dis->physicalWidth((0))));

    backlightTotext(dis->backlightStatus(0));
}

void Dialog::setupStorage()
{
    if (!sti) {
    sti = new QSystemStorageInfo(this);
    storageTreeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

    connect(sti,SIGNAL(logicalDriveChanged(bool,const QString &)),
            this,SLOT(storageChanged(bool ,const QString &)));
    }
    connect(sti,SIGNAL(storageStateChanged(const QString &,QSystemStorageInfo::StorageState)),
            this,SLOT(storageStateChanged(const QString &, QSystemStorageInfo::StorageState)));

    updateStorage();
}

void Dialog::updateStorage()
{
    storageTreeWidget->clear();

    QStringList vols = sti->logicalDrives();
    foreach(QString volName, vols) {
        QString type;
        QSystemStorageInfo::DriveType volType;
        volType = sti->typeForDrive(volName);
        if (volType == QSystemStorageInfo::InternalDrive) {
            type =  "Internal";
        }
        if (volType == QSystemStorageInfo::RemovableDrive) {
            type = "Removable";
        }
        if (volType == QSystemStorageInfo::CdromDrive) {
            type =  "CDRom";
        }
        if (volType == QSystemStorageInfo::RemoteDrive) {
            type =  "Network";
        }
        if (volType == QSystemStorageInfo::InternalFlashDrive) {
            type =  "Flash";
        }
        if (volType == QSystemStorageInfo::RamDrive) {
            type =  "Ram";
        }
        QStringList items;
        items << volName;
        items << type;
        items << sizeToString(sti->totalDiskSpace(volName));
        items << sizeToString(sti->availableDiskSpace(volName));
        items << sti->uriForDrive(volName);
        items << storageStateToString(sti->getStorageState(volName));

        QTreeWidgetItem *item = new QTreeWidgetItem(items);

        for (int i = 0; i < 5; i++) {
            item->setBackground( i ,brushForStorageState( sti->getStorageState(volName)));
        }
        storageTreeWidget->addTopLevelItem(item);
    }
}

QBrush Dialog::brushForStorageState(QSystemStorageInfo::StorageState state)
{
    if (state == QSystemStorageInfo::CriticalStorageState) {
        return  QBrush(Qt::red);
    }
    if (state== QSystemStorageInfo::VeryLowStorageState) {
        return  QBrush(Qt::magenta);
    }
    if (state == QSystemStorageInfo::LowStorageState) {
        return  QBrush(Qt::yellow);
    }
    return  QBrush();
}

QString Dialog:: sizeToString(qlonglong size)
{
    float fSize = size;
    int i = 0;
    const char* units[] = {"B", "kB", "MB", "GB", "TB"};
    while (fSize > 1024) {
        fSize /= 1024.0;
        i++;
    }
    fSize = round((fSize)*100)/100;
    return QString::number(fSize)+" "+ units[i];
}

void Dialog::setupNetwork()
{
    delete ni;
//! [networkInfo]
    ni = new QSystemNetworkInfo(this);
//! [networkInfo]

    connect(netStatusComboBox,SIGNAL(activated(int)),
            this, SLOT(netStatusComboActivated(int)));

//! [sig strength]
    connect(ni,SIGNAL(networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode, int)),
            this,SLOT(networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode,int)));
//! [sig strength]

    connect(ni,SIGNAL(networkNameChanged(QSystemNetworkInfo::NetworkMode,QString)),
            this,SLOT(networkNameChanged(QSystemNetworkInfo::NetworkMode,QString)));

    connect(ni,SIGNAL(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)),
            this,SLOT(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)));

    connect(ni,SIGNAL(networkModeChanged(QSystemNetworkInfo::NetworkMode)),
            this,SLOT(networkModeChanged(QSystemNetworkInfo::NetworkMode)));

    connect(ni,SIGNAL(cellDataTechnologyChanged(QSystemNetworkInfo::CellDataTechnology)),
            this,SLOT(dataTechnologyChanged(QSystemNetworkInfo::CellDataTechnology)));

    networkModeChanged(ni->currentMode());
    netStatusComboBox->setCurrentIndex((int)ni->currentMode());
    netStatusComboActivated((int)ni->currentMode());

    cellIdLabel->setText(QString::number(ni->cellId()));
    connect(ni,SIGNAL(cellIdChanged(int)),this,SLOT(cellIdChanged(int)));

    locationAreaCodeLabel->setText(QString::number(ni->locationAreaCode()));
    currentMCCLabel->setText(ni->currentMobileCountryCode());
    currentMNCLabel->setText(ni->currentMobileNetworkCode());

    homeMCCLabel->setText(ni->homeMobileCountryCode());

    homeMNCLabel->setText(ni->homeMobileNetworkCode());

    dataTechnologyChanged(ni->cellDataTechnology());
}
void Dialog::netStatusComboActivated(int index)
{
    QString status;
    int reIndex = index;

    displayNetworkStatus(ni->networkStatus((QSystemNetworkInfo::NetworkMode)reIndex));

    macAddressLabel->setText(ni->macAddress((QSystemNetworkInfo::NetworkMode)reIndex));

    int strength = ni->networkSignalStrength((QSystemNetworkInfo::NetworkMode)reIndex);
    if (strength < 0)
        strength = 0;
    signalLevelProgressBar->setValue(strength);

    InterfaceLabel->setText(ni->interfaceForMode((QSystemNetworkInfo::NetworkMode)reIndex).humanReadableName());

    operatorNameLabel->setText(ni->networkName((QSystemNetworkInfo::NetworkMode)reIndex));

    if ((index == 1 || index == 2 || index == 3)
        && ni->networkStatus((QSystemNetworkInfo::NetworkMode)reIndex)
                             != QSystemNetworkInfo::UndefinedStatus) {

        cellIdLabel->setText(QString::number(ni->cellId()));
        locationAreaCodeLabel->setText(QString::number(ni->locationAreaCode()));
        currentMCCLabel->setText(ni->currentMobileCountryCode());
        currentMNCLabel->setText(ni->currentMobileNetworkCode());

        homeMCCLabel->setText(ni->homeMobileCountryCode());
        homeMNCLabel->setText(ni->homeMobileNetworkCode());
    } else {
        cellIdLabel->setText("");
        locationAreaCodeLabel->setText("");
        currentMCCLabel->setText("");
        currentMNCLabel->setText("");
        homeMCCLabel->setText("");
        homeMNCLabel->setText("");
    }
}

void Dialog::getVersion(int index)
{
    QSystemInfo::Version version;
    switch(index) {
    case 0:
        versionLineEdit->setText("");
        break;
    case 1:
//! [OS ver]
        version = QSystemInfo::Os;
//! [OS ver]
        break;
    case 2:
//! [Qt ver]
        version = QSystemInfo::QtCore;
//! [Qt ver]
        break;
    case 3:
//! [Firm ver]
        version = QSystemInfo::Firmware;
//! [Firm ver]
        break;
    case 4:
        version = QSystemInfo::QtMobility;
        break;
    };

    QSystemInfo si;
    versionLineEdit->setText(si.version(version));
}

void Dialog::getFeature(int index)
{
//! [feature]
    QSystemInfo::Feature feature;
    switch(index) {
//! [feature]
    case 0:
        return;
        break;
//! [feature-bluetooth]
    case 1:
        feature = QSystemInfo::BluetoothFeature;
//! [feature-bluetooth]
        break;
    case 2:
        feature = QSystemInfo::CameraFeature;
        break;
    case 3:
        feature = QSystemInfo::FmradioFeature;
        break;
    case 4:
        feature = QSystemInfo::IrFeature;
        break;
    case 5:
        feature = QSystemInfo::LedFeature;
        break;
    case 6:
        feature = QSystemInfo::MemcardFeature;
        break;
    case 7:
        feature = QSystemInfo::UsbFeature;
        break;
    case 8:
        feature = QSystemInfo::VibFeature;
        break;
    case 9:
        feature = QSystemInfo::WlanFeature;
        break;
    case 10:
        feature = QSystemInfo::SimFeature;
        break;
    case 11:
        feature = QSystemInfo::LocationFeature;
        break;
    case 12:
        feature = QSystemInfo::VideoOutFeature;
        break;
    case 13:
        feature = QSystemInfo::HapticsFeature;
        break;
    case 14:
        feature = QSystemInfo::FmTransmitterFeature;
        break;
    };
//! [feature test]
    QSystemInfo si;
    featuresLineEdit->setText((si.hasFeatureSupported(feature) ? "true":"false" ));
//! [feature test]
}

void Dialog::setupSaver()
{
    if (!saver) {
        saver = new QSystemScreenSaver(this);
    }

    bool saverEnabled = saver->screenSaverInhibited();


    connect( saverInhibitedCheckBox, SIGNAL(clicked(bool)),
             this,SLOT(setSaverEnabled(bool)));

    saverInhibitedCheckBox->setChecked(saverEnabled);
}


void Dialog::setSaverEnabled(bool b)
{
    saver->setScreenSaverInhibited(b);
}


void Dialog::updateBatteryStatus(int level)
{
    batteryLevelBar->setValue(level);
    lcdNumber->display(level);
}

void Dialog::updatePowerState(QSystemDeviceInfo::PowerState /*newState*/)
{
//    currentPowerState = newState;
//    switch (newState) {
//    case QSystemDeviceInfo::BatteryPower:
//        {
//            radioButton_2->setChecked(true);
//        }
//        break;
//    case QSystemDeviceInfo::WallPower:
//        {
//            radioButton_3->setChecked(true);
//        }
//        break;
//    case QSystemDeviceInfo::WallPowerChargingBattery:
//        {
//            radioButton_4->setChecked(true);
//        }
//        break;
//    case QSystemDeviceInfo::NoBatteryLevel:
//        {
//            radioButton->setChecked(true);
//        }
//        break;
//    };
}

void Dialog::displayBatteryStatus(QSystemBatteryInfo::BatteryStatus status)
{
    if (currentBatStat == status)
        return;
    QString msg;
        switch(status) {
        case QSystemBatteryInfo::BatteryEmpty:
            {
                msg = "Battery is Empty (0%), shutting down.";
                QMessageBox::critical(this,"QSystemInfo",msg);
            }
            break;
        case QSystemBatteryInfo::BatteryCritical:
            {
                msg = "Battery is Critical (4% or less), please save your work or plug in the charger.";
                QMessageBox::critical(this,"QSystemInfo",msg);
            }
            break;
        case QSystemBatteryInfo::BatteryVeryLow:
            {
                msg = "Battery is Very Low (10%), please plug in the charger soon";
                QMessageBox::warning(this,"QSystemInfo",msg);
            }
            break;
        case QSystemBatteryInfo::BatteryLow:
            {
                msg = "Battery is Low (40% or less)";
                QMessageBox::information(this,"QSystemInfo",msg);
            }
            break;
        case QSystemBatteryInfo::BatteryOk:
            {
                msg = "Battery is Normal (greater than 40%)";
                QMessageBox::information(this,"QSystemInfo",msg);
            }
            break;
        case QSystemBatteryInfo::BatteryFull:
            {
                msg = "Battery is Full (100%)";
                QMessageBox::information(this,"QSystemInfo",msg);
            }
            break;
        case QSystemBatteryInfo::BatteryUnknown:
            {

            }
            break;
        };
        currentBatStat = status;
}

void Dialog::networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode mode , int strength)
{
    if (strength < 0) {
        strength = 0;
    }

    if (mode == QSystemNetworkInfo::UnknownMode) {
        if (netStatusComboBox->currentText() == "Unknown") {
            signalLevelProgressBar->setValue(strength);
        }
    }

    if (mode == QSystemNetworkInfo::GsmMode) {
        if (netStatusComboBox->currentText() == "Gsm") {
            signalLevelProgressBar->setValue(strength);
        }
    }

    if (mode == QSystemNetworkInfo::CdmaMode) {
        if (netStatusComboBox->currentText() == "Cdma") {
            signalLevelProgressBar->setValue(strength);
        }
    }

    if (mode == QSystemNetworkInfo::WcdmaMode) {
        if (netStatusComboBox->currentText() == "Wcdma") {
            signalLevelProgressBar->setValue(strength);
        }
    }

    if (mode == QSystemNetworkInfo::WlanMode) {
        if (netStatusComboBox->currentText() == "Wlan") {
            signalLevelProgressBar->setValue(strength);
        }
    }

    if (mode == QSystemNetworkInfo::EthernetMode) {
        if (netStatusComboBox->currentText() == "Ethernet") {
            signalLevelProgressBar->setValue(strength);
        }
    }

    if (mode == QSystemNetworkInfo::BluetoothMode) {
        if (netStatusComboBox->currentText() == "Bluetooth") {
            signalLevelProgressBar->setValue(strength);
        }
    }

    if (mode == QSystemNetworkInfo::WimaxMode) {
        if (netStatusComboBox->currentText() == "Wimax") {
            signalLevelProgressBar->setValue(strength);
        }
    }

    if (mode == QSystemNetworkInfo::LteMode) {
        if (netStatusComboBox->currentText() == "Lte") {
            signalLevelProgressBar->setValue(strength);
        }
    }

}

void Dialog::networkNameChanged(QSystemNetworkInfo::NetworkMode mode,const QString &text)
{
    if (mode == QSystemNetworkInfo::UnknownMode) {
        if (netStatusComboBox->currentText() == "Unknown") {
            operatorNameLabel->setText(text);
        }
    }

    if (mode == QSystemNetworkInfo::GsmMode) {
        if (netStatusComboBox->currentText() == "Gsm") {
            operatorNameLabel->setText(text);
        }
    }

    if (mode == QSystemNetworkInfo::CdmaMode) {
        if (netStatusComboBox->currentText() == "Cdma") {
            operatorNameLabel->setText(text);
        }
    }

    if (mode == QSystemNetworkInfo::WcdmaMode) {
        if (netStatusComboBox->currentText() == "Wcdma") {
            operatorNameLabel->setText(text);
        }
    }

    if (mode == QSystemNetworkInfo::WlanMode) {
        if (netStatusComboBox->currentText() == "Wlan") {
            operatorNameLabel->setText(text);
        }
    }

    if (mode == QSystemNetworkInfo::EthernetMode) {
        if (netStatusComboBox->currentText() == "Ethernet") {
            operatorNameLabel->setText(text);
        }
    }

    if (mode == QSystemNetworkInfo::BluetoothMode) {
        if (netStatusComboBox->currentText() == "Bluetooth") {
            operatorNameLabel->setText(text);
        }
    }

    if (mode == QSystemNetworkInfo::WimaxMode) {
        if (netStatusComboBox->currentText() == "Wimax") {
            operatorNameLabel->setText(text);
        }
    }

    if (mode == QSystemNetworkInfo::LteMode) {
        if (netStatusComboBox->currentText() == "Lte") {
            operatorNameLabel->setText(text);
        }
    }
}

void Dialog::networkStatusChanged(QSystemNetworkInfo::NetworkMode mode , QSystemNetworkInfo::NetworkStatus status)
{
    if (mode == QSystemNetworkInfo::UnknownMode) {
        if (netStatusComboBox->currentText() == "Unknown") {
            displayNetworkStatus(status);
        }
    }

    if (mode == QSystemNetworkInfo::GsmMode) {
        if (netStatusComboBox->currentText() == "Gsm") {
            displayNetworkStatus(status);
        }
    }

    if (mode == QSystemNetworkInfo::CdmaMode) {
        if (netStatusComboBox->currentText() == "Cdma") {
            displayNetworkStatus(status);
        }
    }

    if (mode == QSystemNetworkInfo::WcdmaMode) {
        if (netStatusComboBox->currentText() == "Wcdma") {
            displayNetworkStatus(status);
        }
    }

    if (mode == QSystemNetworkInfo::WlanMode) {
        if (netStatusComboBox->currentText() == "Wlan") {
           displayNetworkStatus(status);
        }
    }

    if (mode == QSystemNetworkInfo::EthernetMode) {
        if (netStatusComboBox->currentText() == "Ethernet") {
           displayNetworkStatus(status);
        }
    }

    if (mode == QSystemNetworkInfo::BluetoothMode) {
        if (netStatusComboBox->currentText() == "Bluetooth") {
            displayNetworkStatus(status);
        }
    }

    if (mode == QSystemNetworkInfo::WimaxMode) {
        if (netStatusComboBox->currentText() == "Wimax") {
            displayNetworkStatus(status);
        }
    }

    if (mode == QSystemNetworkInfo::LteMode) {
        if (netStatusComboBox->currentText() == "Lte") {
            displayNetworkStatus(status);
        }
    }
}

void Dialog::networkModeChanged(QSystemNetworkInfo::NetworkMode mode)
{
    if (mode == QSystemNetworkInfo::UnknownMode) {
        primaryModeLabel->setText("None");
    }

    if (mode == QSystemNetworkInfo::GsmMode) {
        primaryModeLabel->setText("Gsm");
    }

    if (mode == QSystemNetworkInfo::CdmaMode) {
        primaryModeLabel->setText("Cdma");
    }

    if (mode == QSystemNetworkInfo::WcdmaMode) {
        primaryModeLabel->setText("Wcdma");
    }
    if (mode == QSystemNetworkInfo::WlanMode) {
        primaryModeLabel->setText("Wlan");
    }

    if (mode == QSystemNetworkInfo::EthernetMode) {
        primaryModeLabel->setText("Ethernet");
    }

    if (mode == QSystemNetworkInfo::BluetoothMode) {
        primaryModeLabel->setText("Bluetooth");
    }

    if (mode == QSystemNetworkInfo::WimaxMode) {
        primaryModeLabel->setText("Wimax");
    }

    if (mode == QSystemNetworkInfo::LteMode) {
        primaryModeLabel->setText("Lte");
    }
}


void Dialog::displayNetworkStatus(QSystemNetworkInfo::NetworkStatus status)
{
    QString stat;
    switch(status) {
    case QSystemNetworkInfo::UndefinedStatus:
        stat = "Undefined";
        break;
    case QSystemNetworkInfo::NoNetworkAvailable:
        stat = "No Network Available";
        break;
    case QSystemNetworkInfo::EmergencyOnly:
        stat = "Emergency Only";
        break;
    case QSystemNetworkInfo::Searching:
        stat = "Searching or Connecting";
        break;
    case QSystemNetworkInfo::Busy:
        stat = "Busy";
        break;
    case QSystemNetworkInfo::Connected:
        stat = "Connected";
        break;
    case QSystemNetworkInfo::HomeNetwork:
        stat = "Home Network";
        break;
    case QSystemNetworkInfo::Denied:
        stat = "Denied";
        break;
    case QSystemNetworkInfo::Roaming:
        stat = "Roaming";
        break;
    };
    cellNetworkStatusLabel->setText(stat);
}

void Dialog::updateProfile()
{

    if (di) {
        QString profilestring;
        switch(di->currentProfile()) {
            case QSystemDeviceInfo::SilentProfile:
            {
                profilestring = "Silent";
            }
            break;
            case QSystemDeviceInfo::BeepProfile:
            {
                profilestring = "Beep";
            }
            break;
            case QSystemDeviceInfo::NormalProfile:
            {
                profilestring = "Normal";
            }
            break;
            case QSystemDeviceInfo::LoudProfile:
            {
                profilestring = "Loud";
            }
            break;
            case QSystemDeviceInfo::VibProfile:
            {
                profilestring = "Vibrate";
            }
            break;
            case QSystemDeviceInfo::OfflineProfile:
            {
                profilestring = "Offline";
            }
            break;
            case QSystemDeviceInfo::PowersaveProfile:
            {
                profilestring = "Powersave";
            }
            break;
            case QSystemDeviceInfo::CustomProfile:
            {
                profilestring = "custom";
            }
            break;
            default:
            {
                profilestring = "Unknown";
            }
        };
        profileLabel->setText(profilestring);

        QSystemDeviceInfo::ProfileDetails pDetails = di->activeProfileDetails();
        messageRingtonVolumeLcdNumber->display(pDetails.messageRingtoneVolume());
        voiceRingtoneVolumeLcdNumber->display(pDetails.voiceRingtoneVolume());
        vibrationActiveRadioButton->setChecked(pDetails.vibrationActive());
    }
}


void Dialog::updateSimStatus()
{
    if (di) {
        QString simstring;
        switch(di->simStatus()) {
        case QSystemDeviceInfo::SimLocked:
            {
                simstring = "Sim Locked";
            }
            break;
        case QSystemDeviceInfo::SimNotAvailable:
            {
                simstring = "Sim not available";
            }
            break;
        case QSystemDeviceInfo::SingleSimAvailable:
            {
                simstring = "Single Sim Available";
            }
            break;
        case QSystemDeviceInfo::DualSimAvailable:
            {
                simstring = "Dual Sim available";
            }
            break;

        };
        simStatusLabel->setText(simstring);
    }
}

void Dialog::updateThermalState()
{
    if(di) {
        QString thermalState;
        switch (di->currentThermalState()) {
        case QSystemDeviceInfo::UnknownThermal:
            {
                thermalState = "Unknown";
            }
            break;
        case QSystemDeviceInfo::NormalThermal:
            {
                thermalState = "Normal";
            }
            break;
        case QSystemDeviceInfo::WarningThermal:
            {
                thermalState = "Warning";
            }
            break;
        case QSystemDeviceInfo::AlertThermal:
            {
                thermalState = "Alert";
            }
            break;
        case QSystemDeviceInfo::ErrorThermal:
            {
                thermalState = "Error";
            }
            break;
        };
        thermalStateLabel->setText(thermalState);
    }
}

void Dialog::storageChanged(bool added,const QString &volName)
{
    if (added) {
        updateStorage();
    } else {
        storageTreeWidget->takeTopLevelItem( storageTreeWidget->indexOfTopLevelItem(storageTreeWidget->findItems(volName,Qt::MatchExactly).at(0)));
    }
}

void Dialog::bluetoothChanged(bool b)
{
    bluetoothPowerLabel->setText((b ? "On" : "Off"));
}

void Dialog::setupBattery()
{
    delete bi;
    //! [batterystatus1]
    bi = new QSystemBatteryInfo(this);
    //! [batterystatus1]

    connect(bi,SIGNAL(remainingCapacityPercentChanged(int)),
            this,SLOT(updateBatteryStatus(int)));

    connect(bi,SIGNAL(batteryStatusChanged(QSystemBatteryInfo::BatteryStatus)),
            this,SLOT(displayBatteryStatus(QSystemBatteryInfo::BatteryStatus)));

    connect(bi,SIGNAL(chargingStateChanged(QSystemBatteryInfo::ChargingState)),
             this,SLOT(chargingStateChanged(QSystemBatteryInfo::ChargingState)));

    connect(bi,SIGNAL(chargerTypeChanged(QSystemBatteryInfo::ChargerType)),
            this,SLOT(chargerTypeChanged(QSystemBatteryInfo::ChargerType)));

    connect(bi,SIGNAL(nominalCapacityChanged(int)),
            NominalCaplcdNumber,SLOT(display(int)));
    connect(bi,SIGNAL(remainingCapacityChanged(int)),
            remainCaplcdNumber,SLOT(display(int)));
    connect(bi,SIGNAL(currentFlowChanged(int)),
            currentFLowlcdNumber,SLOT(display(int)));
    connect(bi,SIGNAL(remainingCapacityBarsChanged(int)),
            remainingCapBarslcdNumber,SLOT(display(int)));
    connect(bi,SIGNAL(remainingChargingTimeChanged(int)),
            chargeTimelcdNumber,SLOT(display(int)));

    //! [batterystatus2]
    chargerTypeChanged(bi->chargerType());

    currentBatStat = bi->batteryStatus();
    //! [batterystatus2]

    chargingStateChanged(bi->chargingState());

    NominalCaplcdNumber->display(bi->nominalCapacity());

    int level = bi->remainingCapacityPercent();
    batteryLevelBar->setValue(level);
    lcdNumber->display(level);

    remainCaplcdNumber->display(bi->remainingCapacity());

    voltagelcdNumber->display(bi->voltage());
    chargeTimelcdNumber->display(bi->remainingChargingTime());
    currentFLowlcdNumber->display(bi->currentFlow());
    remainingCapBarslcdNumber->display(bi->remainingCapacityBars());
    maxBarslcdNumber->display(bi->maxBars());

    if (bi->energyMeasurementUnit() == QSystemBatteryInfo::UnitmAh) {
        energyMeasurementUnit->setText("mAh");
    } else if (bi->energyMeasurementUnit() == QSystemBatteryInfo::UnitmWh) {
        energyMeasurementUnit->setText("mWh");
    } else {
        energyMeasurementUnit->setText("Unknown");
    }

}

void Dialog::chargingStateChanged(QSystemBatteryInfo::ChargingState chargingState)
{
    if (chargingState == QSystemBatteryInfo::Charging) {
        chargingCheckBox->setChecked(true);
    } else {
        chargingCheckBox->setChecked(false);
    }

    currentChargingState = chargingState;
}

void Dialog::chargerTypeChanged(QSystemBatteryInfo::ChargerType chargerType)
{
    if (chargerType == QSystemBatteryInfo::NoCharger) {
        radioButton_2->setChecked(true);
    } else if (chargerType == QSystemBatteryInfo::WallCharger) {
        radioButton_3->setChecked(true);
    } else if (chargerType == QSystemBatteryInfo::USBCharger) {
        radioButton_4->setChecked(true);
    } else if (chargerType == QSystemBatteryInfo::USB_500mACharger) {
        radioButton_5->setChecked(true);
    } else if (chargerType == QSystemBatteryInfo::USB_100mACharger) {
        radioButton_6->setChecked(true);
    } else if (chargerType == QSystemBatteryInfo::VariableCurrentCharger) {
        radioButton_7->setChecked(true);
    } else {
        radioButton->setChecked(true);
    }
    currentChargerType = chargerType;
}

void Dialog::orientationChanged(QSystemDisplayInfo::DisplayOrientation orientation)
{
    QString orientStr;
    switch(orientation) {
    case QSystemDisplayInfo::Landscape:
        orientStr="Landscape";
        break;
    case QSystemDisplayInfo::Portrait:
        orientStr="Portrait";
        break;
    case QSystemDisplayInfo::InvertedLandscape:
        orientStr="Inverted Landscape";
        break;
    case QSystemDisplayInfo::InvertedPortrait:
        orientStr="Inverted Portrait";
        break;
    default:
        orientStr="Orientation unknown";
        break;
    }

    orientationLabel->setText(orientStr);
}


void Dialog::keyboardFlipped(bool on)
{
    keyboardFlipRadioButton->setChecked(on);
}

void Dialog::storageStateChanged(const QString &vol, QSystemStorageInfo::StorageState state)
{
    QList<QTreeWidgetItem *>item = storageTreeWidget->findItems(vol,Qt::MatchExactly,0);
    item.at(0)->setText(3,sizeToString(sti->availableDiskSpace(item.at(0)->text(0))));
    item.at(0)->setText(5,storageStateToString(state));
}

QString Dialog::storageStateToString(QSystemStorageInfo::StorageState state)
{
    QString str;
    if (state == QSystemStorageInfo::CriticalStorageState) {
        str = "Critical";
    } else if (state == QSystemStorageInfo::VeryLowStorageState) {
        str = "Very Low";
    } else if (state == QSystemStorageInfo::LowStorageState) {
        str = "Low";
    } else {
        str = "Normal";
    }
    return str;
}


void Dialog::backlightTotext(QSystemDisplayInfo::BacklightState state)
{
    QString blState;

    switch(state) {
    case QSystemDisplayInfo::BacklightStateUnknown:
        blState = "Unknown";
        break;
    case QSystemDisplayInfo::BacklightStateOff:
        blState = "Off";
        break;
    case QSystemDisplayInfo::BacklightStateDimmed:
        blState = "Dimmed";
        break;
    case QSystemDisplayInfo::BacklightStateOn:
        blState = "On";
        break;
    };
    backlightStatusLabel->setText(blState);
}


void Dialog::dataTechnologyChanged(QSystemNetworkInfo::CellDataTechnology tech)
{
    QString techString;
    switch(tech) {
    case QSystemNetworkInfo::UnknownDataTechnology:
        techString = "Unknown";
        break;
    case QSystemNetworkInfo::GprsDataTechnology:
        techString = "Gprs";
        break;
    case QSystemNetworkInfo::EdgeDataTechnology:
        techString = "Edge";
        break;
    case QSystemNetworkInfo::UmtsDataTechnology:
        techString = "Umts";
        break;
    case QSystemNetworkInfo::HspaDataTechnology:
        techString = "Hspa";
        break;
    };
    dataTechnologyLabel->setText(techString);
}

QString Dialog::lockStateToString(QSystemDeviceInfo::LockTypeFlags lock)
{
    if ((lock & QSystemDeviceInfo::PinLocked)){
        return "Pin/Password Locked";
    } else if ((lock & QSystemDeviceInfo::TouchAndKeyboardLocked)){
        return "Touch and keyboard locked";
    }
    return "Unknown";
}

void Dialog::lockStatusChanged(QSystemDeviceInfo::LockTypeFlags locktype)
{
    if (locktype != oldLockStatus) {
        lockStateLabel_2->setText(lockStateToString(oldLockStatus));
        oldLockStatus = locktype;
        lockStateLabel->setText(lockStateToString(locktype));
    }
}

void Dialog::cellIdChanged(int id)
{
    cellIdLabel->setText(QString::number(id));
}

void  Dialog::setupAlignedTimer()
{
    connect(startButton,SIGNAL(clicked()),this,SLOT(startAlignedTimers()));
}

void Dialog::setupAlignedTimers()
{
    QSystemAlignedTimer alignedtimer;
    if(alignedtimer.lastError() == QSystemAlignedTimer::AlignedTimerNotSupported) {
        min1spinBox->setEnabled(false);
        max1spinBox->setEnabled(false);

        min2spinBox->setEnabled(false);
        max2spinBox->setEnabled(false);

        min3spinBox->setEnabled(false);
        max3spinBox->setEnabled(false);

        startButton->setEnabled(false);

        checkBox_1->setEnabled(false);
        checkBox_2->setEnabled(false);
        checkBox_3->setEnabled(false);

        timeLabel->setText("Aligned Timer is not supported on this platform");
        return;
    }

    if (!alt1) {
        alt1 = new QSystemAlignedTimer(this);

        connect(alt1,SIGNAL(error(QSystemAlignedTimer::AlignedTimerError)),
                this,SLOT(timerError(QSystemAlignedTimer::AlignedTimerError)));
        connect(alt1,SIGNAL(timeout()),this,SLOT(timeout1()));
    }
    if (!alt2) {
        alt2 = new QSystemAlignedTimer(this);

        connect(alt2,SIGNAL(error(QSystemAlignedTimer::AlignedTimerError)),
                this,SLOT(timerError(QSystemAlignedTimer::AlignedTimerError)));
        connect(alt2,SIGNAL(timeout()),this,SLOT(timeout2()));
    }
    if (!alt3) {
        alt3 = new QSystemAlignedTimer(this);

        connect(alt3,SIGNAL(error(QSystemAlignedTimer::AlignedTimerError)),
                this,SLOT(timerError(QSystemAlignedTimer::AlignedTimerError)));
        connect(alt3,SIGNAL(timeout()),this,SLOT(timeout3()));
    }

    connect(startButton,SIGNAL(clicked()),this,SLOT(startAlignedTimers()),Qt::UniqueConnection);
    connect(stopButton,SIGNAL(clicked()),this,SLOT(stopAlignedTimers()),Qt::UniqueConnection);

    connect(startButton_2,SIGNAL(clicked()),this,SLOT(startAlignedTimers()),Qt::UniqueConnection);
    connect(stopButton_2,SIGNAL(clicked()),this,SLOT(stopAlignedTimers()),Qt::UniqueConnection);

    connect(startButton_3,SIGNAL(clicked()),this,SLOT(startAlignedTimers()),Qt::UniqueConnection);
    connect(stopButton_3,SIGNAL(clicked()),this,SLOT(stopAlignedTimers()),Qt::UniqueConnection);
}

void Dialog::startAlignedTimers()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    if (button == startButton) {
        if (checkBox_1->isChecked()) {
            QSystemAlignedTimer::singleShot(min1spinBox->value(),max1spinBox->value(),
                                            this,SLOT(timeout1()));
        } else {
            alt1->setMinimumInterval(min1spinBox->value());
            alt1->setMaximumInterval(max1spinBox->value());
            alt1->start();
        }
        if(alt1->lastError() == QSystemAlignedTimer::NoError)
            textEdit->append("<b>Timer 1:  Start: </b>" + QDateTime::currentDateTime().toString("yyyy MMM ddd hh:mm:ss"));
    } else if (button == startButton_2) {
        if (checkBox_2->isChecked()) {
            QSystemAlignedTimer::singleShot(min2spinBox->value(),max2spinBox->value(),
                                            this,SLOT(timeout2()));
        } else {
            alt2->start(min2spinBox->value(),max2spinBox->value());
        }
        if(alt2->lastError() == QSystemAlignedTimer::NoError)
            textEdit->append("<b>Timer 2: Start: </b>" + QDateTime::currentDateTime().toString("yyyy MMM ddd hh:mm:ss"));
    } else if (button == startButton_3) {
        if (checkBox_3->isChecked()) {
            QSystemAlignedTimer::singleShot(min3spinBox->value(),max3spinBox->value(),
                                            this,SLOT(timeout3()));
        } else {
            if(alt3->lastError() == QSystemAlignedTimer::NoError)
                alt3->start(min3spinBox->value(),max3spinBox->value());
        }
        textEdit->append("<b>Timer 3:  Start: </b>" + QDateTime::currentDateTime().toString("yyyy MMM ddd hh:mm:ss"));
    }
}

void Dialog::timeout1()
{
    textEdit->append("<b>Timer 1: </b>" + QDateTime::currentDateTime().toString("yyyy MMM ddd hh:mm:ss"));
}

void Dialog::timeout2()
{
    textEdit->append("<b>Timer 2: </b>" + QDateTime::currentDateTime().toString("yyyy MMM ddd hh:mm:ss"));
}

void Dialog::timeout3()
{
    textEdit->append("<b>Timer 3: </b>" + QDateTime::currentDateTime().toString("yyyy MMM ddd hh:mm:ss"));
}

void Dialog::stopAlignedTimers()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(button->objectName() == stopButton->objectName()) {
        alt1->stop();
        textEdit->append("<b>Timer 1:  Stop: </b>" + QDateTime::currentDateTime().toString("yyyy MMM ddd hh:mm:ss"));
    } else if(button == stopButton_2) {
        alt2->stop();
        textEdit->append("<b>Timer 2:  Stop: </b>" + QDateTime::currentDateTime().toString("yyyy MMM ddd hh:mm:ss"));
    } else if(button == stopButton_3) {
        alt3->stop();
        textEdit->append("<b>Timer 3:  Stop: </b>" + QDateTime::currentDateTime().toString("yyyy MMM ddd hh:mm:ss"));
    }
}

void Dialog::timerError(QSystemAlignedTimer::AlignedTimerError error)
{
    QString errorStr;
    QSystemAlignedTimer *timer = qobject_cast<QSystemAlignedTimer*>(sender());
    if (timer == alt1) {
        errorStr = "<b>Timer 1: </b> ";
    } if (timer == alt2) {
        errorStr = "<b>Timer 2: </b> ";
    } if (timer == alt3) {
        errorStr = "<b>Timer 3: </b> ";
    }
    switch( error) {
    case QSystemAlignedTimer::AlignedTimerNotSupported:
        errorStr += "Timer Not Supported";
        break;
    case QSystemAlignedTimer::InvalidArgument:
        errorStr += "Invalid Argument";
        break;
    case QSystemAlignedTimer::TimerFailed:
        errorStr += "Timer Failed";
        break;
    case QSystemAlignedTimer::InternalError:
        errorStr += "Internal Error";
        break;
    };
    textEdit->append(errorStr);
}
