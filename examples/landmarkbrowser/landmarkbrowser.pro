# #####################################################################
# Automatically generated by qmake (2.01a) Mon Jun 21 13:06:14 2010
# #####################################################################
TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += ../../src/location \
                ../../src/location/landmarks
include(../mobility_examples.pri)

# Input
SOURCES += main.cpp \
    landmarkbrowser.cpp \
    landmarkadddialog.cpp \
    categoryadddialog.cpp \
    landmarkfilterdialog.cpp

HEADERS += landmarkbrowser.h \
    landmarkadddialog.h \
    categoryadddialog.h \
    landmarkfilterdialog.h
    
CONFIG += mobility

FORMS += landmarkbrowser.ui \
    landmarkadddialog.ui \
    categoryadddialog.ui \
    landmarkfilterdialog.ui

MOBILITY += location

symbian|wince* {
    symbian {
        addFiles.sources = places.gpx
        addFiles.sources += moreplaces.lmx
        DEPLOYMENT += addFiles

        TARGET.CAPABILITY = LocalServices ReadUserData WriteUserData ReadDeviceData WriteDeviceData NetworkServices
    }
    wince* {
        addFiles.sources = ./places.gpx
        addFiles.sources += ./moreplaces.lmx
        addFiles.path = .
        DEPLOYMENT += addFiles
    }
} else {
    samplegpx.path = $$QT_MOBILITY_EXAMPLES
    samplegpx.files = places.gpx
    INSTALLS += samplegpx


    samplelmx.path = $$QT_MOBILITY_EXAMPLES
    samplelmx.files = moreplaces.lmx
    INSTALLS += samplelmx
}
