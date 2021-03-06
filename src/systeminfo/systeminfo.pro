TEMPLATE = lib
TARGET = QtSystemInfo
QT += network gui

include(../../common.pri)

# Input
PUBLIC_HEADERS += \
    qsysteminfo.h \
    qsystemgeneralinfo.h \
    qsystemdeviceinfo.h \
    qsystemdisplayinfo.h \
    qsystemnetworkinfo.h \
    qsystemscreensaver.h \
    qsystemstorageinfo.h  \
    qsystembatteryinfo.h \
    qsystemalignedtimer.h

SOURCES += \
    qsystemgeneralinfo.cpp \
    qsystemdeviceinfo.cpp \
    qsystemdisplayinfo.cpp \
    qsystemnetworkinfo.cpp \
    qsystemscreensaver.cpp \
    qsystemstorageinfo.cpp \
    qsystembatteryinfo.cpp \
    qsystemalignedtimer.cpp

PRIVATE_HEADERS += qsysteminfocommon_p.h

DEFINES += QT_MAKEDLL QT_BUILD_SYSINFO_LIB

simulator {
    SOURCES += qsystemalignedtimer_stub.cpp
    HEADERS += qsystemalignedtimer_stub_p.h
}

win32:!simulator {
    contains(CONFIG,release) {
       CONFIG -= console
    }

    SOURCES += qsysteminfo_win.cpp qsystemalignedtimer_stub.cpp
    HEADERS += qsysteminfo_win_p.h qsystemalignedtimer_stub_p.h

    win32-msvc*: {
        SOURCES += windows/qwmihelper_win.cpp
        HEADERS += windows/qwmihelper_win_p.h

        LIBS += \
            -lOle32 \
            -lUser32 \
            -lGdi32 \
            -lIphlpapi \
            -lOleaut32 \
            -lDxva2 \
            -lPowrProf \
            -lSetupapi

    }

    win32-g++: {
        LIBS += -luser32 -lgdi32
    }

    wince*: {
        LIBS += \
            -Laygshell \
            -lcellcore \
            -lCoredll
    }
}

unix:!simulator {
    QT += gui
    PRIVATE_HEADERS += linux/qsysteminfo_dbus_p.h

        contains(build_unit_tests, yes):contains(test_use_sim, yes) {

            ## for using simulator backend to test frontend signals
            ## configure with -test-sim -tests

            SOURCES += qsysteminfo_simulator.cpp qsysteminfodata_simulator.cpp
            HEADERS += qsysteminfo_simulator_p.h qsysteminfodata_simulator_p.h
            DEFINES += TESTR QT_SIMULATOR
            SOURCES += qsystemalignedtimer_stub.cpp
            HEADERS += qsystemalignedtimer_stub_p.h

        } else {


        linux-*: {
        contains(bluez_enabled, yes):DEFINES += BLUEZ_SUPPORTED


        SOURCES += linux/qsysteminfo_linux_common.cpp
        HEADERS += linux/qsysteminfo_linux_common_p.h

        contains(blkid_enabled, yes): {
            DEFINES += BLKID_SUPPORTED
            LIBS += -lblkid
        }

        !embedded:!contains(QT_CONFIG,qpa): {
            LIBS += -lX11 -lXrandr
        }

        # alignedtimer on Linux/MeeGo
        contains(iphb_enabled, yes): {
            SOURCES += qsystemalignedtimer_meego.cpp
            HEADERS += qsystemalignedtimer_meego_p.h
            PKGCONFIG += libiphb
            DEFINES += ALIGNEDTIMER_MEEGO
            LIBS += -liphb
        } else {
            SOURCES += qsystemalignedtimer_stub.cpp
            HEADERS += qsystemalignedtimer_stub_p.h
        }
    }

    !maemo5:!maemo6:linux-*: {

        SOURCES += linux/qsysteminfo_linux.cpp
        HEADERS += linux/qsysteminfo_linux_p.h
        contains(QT_CONFIG, dbus): {
            QT += dbus
            SOURCES += \
                linux/qhalservice_linux.cpp \
                linux/qsysteminfodbushelper.cpp \
                linux/qdevicekitservice_linux.cpp

            HEADERS += \
                linux/qhalservice_linux_p.h \
                linux/qsysteminfodbushelper_p.h \
                linux/qdevicekitservice_linux_p.h \
                linux/qsysteminfo_dbus_p.h

            # udev should not be enabled on maemo5 and maemo6
            contains(udev_enabled, yes): {
                DEFINES += UDEV_SUPPORTED
                LIBS += -ludev
                SOURCES += linux/qudevservice_linux.cpp
                HEADERS += linux/qudevservice_linux_p.h
            }

            contains(networkmanager_enabled, yes): {
                SOURCES += linux/qnetworkmanagerservice_linux.cpp linux/qnmdbushelper.cpp
                HEADERS += linux/qnetworkmanagerservice_linux_p.h linux/qnmdbushelper_p.h
            } else {
                DEFINES += QT_NO_NETWORKMANAGER
            }

            contains(CONFIG,meego): {
                #for now... udisks
            } else {
                DEFINES += QT_NO_UDISKS
                !embedded:!contains(QT_CONFIG,qpa): LIBS += -lX11 -lXrandr
            }

            contains(connman_enabled, yes): {
                SOURCES+= linux/qconnmanservice_linux.cpp linux/qofonoservice_linux.cpp
                HEADERS+= linux/qconnmanservice_linux_p.h linux/qofonoservice_linux_p.h
            } else {
                DEFINES += QT_NO_CONNMAN
            }
        } else {
            DEFINES += QT_NO_NETWORKMANAGER QT_NO_UDISKS QT_NO_CONNMAN
            !embedded:!contains(QT_CONFIG,qpa): LIBS += -lX11 -lXrandr
        }
     }

    maemo5|maemo6: {

        #Qt GConf wrapper added here until a proper place is found for it.
        CONFIG += link_pkgconfig
        SOURCES += qsysteminfo_maemo.cpp linux/gconfitem.cpp
        HEADERS += qsysteminfo_maemo_p.h linux/gconfitem_p.h
        DEFINES += QT_NO_CONNMAN QT_NO_UDISKS  QT_NO_NETWORKMANAGER

          contains(bme_enabled, yes): {
              LIBS += -lbmeipc
              DEFINES += Q_USE_BME
          }

        contains(QT_CONFIG,dbus): {
            QT += dbus
            SOURCES += linux/qhalservice_linux.cpp
            HEADERS += linux/qhalservice_linux_p.h
        }

        PKGCONFIG += glib-2.0 gconf-2.0
        CONFIG += create_pc create_prl
        QMAKE_PKGCONFIG_REQUIRES = glib-2.0 gconf-2.0
        pkgconfig.path = $$QT_MOBILITY_LIB/pkgconfig
        pkgconfig.files = QtSystemInfo.pc
    }

    mac: {
        OBJECTIVE_SOURCES += qsysteminfo_mac.mm
        SOURCES += qsystemalignedtimer_stub.cpp

        HEADERS += qsysteminfo_mac_p.h qsystemalignedtimer_stub_p.h
        LIBS += -framework SystemConfiguration -framework CoreFoundation \
            -framework IOKit -framework ApplicationServices -framework Foundation \
            -framework CoreServices -framework ScreenSaver -framework QTKit \
            -framework DiskArbitration -framework IOBluetooth

        contains(corewlan_enabled, yes) {
            isEmpty(QMAKE_MAC_SDK) {
                SDK6="yes"
            } else {
                contains(QMAKE_MAC_SDK, "/Developer/SDKs/MacOSX10.6.sdk") {
                    SDK6="yes"
                }
            }

            !isEmpty(SDK6) {
                LIBS += -framework CoreWLAN  -framework CoreLocation
            }
        } else {
            DEFINES += MAC_SDK_10_5
            CONFIG += no_keywords
        }

        TEMPLATE = lib
    }
  }

    symbian: {
        contains(S60_VERSION, 3.1) {
            DEFINES += SYMBIAN_3_1
        }

        contains(S60_VERSION, 5.2) {
            DEFINES += SYMBIAN_3_PLATFORM
        }

        contains(LockandFlipPSkeys_enabled, yes) {
            message("LockandFlipPSKeys available")
            DEFINES += LOCKANDFLIP_SUPPORTED
            SOURCES += lockandflipstatus_s60.cpp
            HEADERS += lockandflipstatus_s60.h
        }

        contains(FmTxClient_enabled, yes) {
            message("FmTxClient available")
            DEFINES += FMTXCLIENT_SUPPORTED
            LIBS += -lhwrmfmtxclient
        }

        contains(DiskNotifyClient_enabled, yes) {
            message("DiskNotiferClient available")
            DEFINES += DISKNOTIFY_SUPPORTED
            LIBS += -ldisknotifyhandler
            SOURCES += storagedisknotifier_s60.cpp
            HEADERS += storagedisknotifier_s60.h
        }

        contains(hb_symbian_enabled, yes) {
            ## for symbian ^4
            CONFIG += qt hb
            DEFINES += HB_SUPPORTED
            message("s60_HbKeymap enabled")
            LIBS += -lhbcore
        } else {
            LIBS += -lptiengine
        }

        contains(symbianflextimer_tenabled, yes) { #disabled until test crash is fixed
            message("SymbianFlexTimer enabled")
            SOURCES += qsystemalignedtimer_symbian.cpp heartbeattimer_s60.cpp
            HEADERS += qsystemalignedtimer_symbian_p.h heartbeattimer_s60.h
            DEFINES += ALIGNEDTIMER_SYMBIAN
            LIBS += -lflextimerclient
        } else {
            SOURCES += qsystemalignedtimer_stub.cpp
            HEADERS += qsystemalignedtimer_stub_p.h
        }

        INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
        DEPENDPATH += symbian

        SOURCES += \
            qsysteminfo_s60.cpp \
            telephonyinfo_s60.cpp \
            chargingstatus_s60.cpp \
            wlaninfo_s60.cpp \
            storagestatus_s60.cpp \
            pubandsubkey_s60.cpp \
            batterystatus_s60.cpp \
            networkinfo_s60.cpp

        HEADERS += \
            qsysteminfo_s60_p.h \
            telephonyinfo_s60.h \
            chargingstatus_s60.h \
            wlaninfo_s60.h \
            storagestatus_s60.h \
            pubandsubkey_s60.h \
            batterystatus_s60.h \
            networkinfo_s60.h

        LIBS += \
            -lprofileengine \
            -letel3rdparty \
            -lsysutil \
            -lcentralrepository \
            -lcenrepnotifhandler \
            -lefsrv \
            -lfeatdiscovery \
            -lhwrmvibraclient \
            -lavkon \    #Used by AknLayoutUtils::PenEnabled(). Try to remove this dependency.
            -lcone \
            -lws32 \
            -lcentralrepository \
            -lbluetooth \
            -lgdi \
            -lecom \
            -lplatformenv \
            -lhwrmlightclient \
            -letel

        contains(S60_VERSION, 5.1) | contains(S60_VERSION, 5.2) {
            LIBS += -lhwrmpowerclient -lusbman
        }

        contains(symbiancntsim_enabled, yes) {
            LIBS += -letelmm
            DEFINES += ETELMM_SUPPORTED
            message("ETELMM enabled")
        }

        contains(etelpacketservice_symbian_enabled, yes) {
            message("etel packet service enabled")
            LIBS += -letelpckt
            DEFINES += ETELPACKETSERVICE_SUPPORTED
        }

        contains(thermalstatus_symbian_enabled, yes) {
            DEFINES += THERMALSTATUS_SUPPORTED
            SOURCES += thermalstatus_s60.cpp
            HEADERS += thermalstatus_s60.h
            message("Thermalstatus enabled")
        }

        TARGET.CAPABILITY = ALL -TCB
#        TARGET.CAPABILITY = LocalServices NetworkServices ReadUserData UserEnvironment Location ReadDeviceData TrustedUI

        TARGET.EPOCALLOWDLLDATA = 1
        TARGET.UID3 = 0x2002ac7d

        QtSystemInfoDeployment.sources = QtSystemInfo.dll
        QtSystemInfoDeployment.path = /sys/bin
        DEPLOYMENT += QtSystemInfoDeployment
    }
}

simulator {
    SOURCES += qsysteminfo_simulator.cpp qsysteminfodata_simulator.cpp
    HEADERS += qsysteminfo_simulator_p.h qsysteminfodata_simulator_p.h
    SOURCES += qsystemalignedtimer_stub.cpp
    HEADERS += qsystemalignedtimer_stub_p.h

#   contains(build_unit_tests, yes) {
 #           DEFINES += TESTR QT_SIMULATOR
  # } else {
    INCLUDEPATH += ../mobilitysimulator
    qtAddLibrary(QtMobilitySimulator)
 # }

}

HEADERS += $$PUBLIC_HEADERS
CONFIG += middleware
include (../../features/deploy.pri)
