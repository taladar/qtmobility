TEMPLATE = app
CONFIG+=testcase
TARGET=tst_positionplugin

include (../../../common.pri)

INCLUDEPATH += ../../../src/location \
               ../../../src/location/maps \
               ../../../src/location/maps/tiled

# Input 
HEADERS += ../qgeocoordinate/qlocationtestutils_p.h

SOURCES += tst_positionplugin.cpp \
           ../qgeocoordinate/qlocationtestutils.cpp

CONFIG += mobility
MOBILITY = location

symbian {
    INCLUDEPATH += $${EPOCROOT}epoc32/include/osextensions
    TARGET.CAPABILITY = ALL -TCB
}
