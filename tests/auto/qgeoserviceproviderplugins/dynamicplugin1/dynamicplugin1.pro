TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(qtgeoservices_dynamicplugin1)
PLUGIN_TYPE=geoservices

include(../../../../common.pri)

CONFIG += mobility
MOBILITY = location

HEADERS += qgeoserviceproviderplugin_test.h

SOURCES += qgeoserviceproviderplugin_test.cpp

INCLUDEPATH += ../../../../src/location \
               ../../../../src/location/maps

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
    pluginDep.sources = $${TARGET}.dll
    pluginDep.path = $${QT_PLUGINS_BASE_DIR}/$${PLUGIN_TYPE}
    DEPLOYMENT += pluginDep
}
