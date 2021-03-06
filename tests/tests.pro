include(../staticconfig.pri)

TEMPLATE = subdirs
SUBDIRS += auto benchmarks manual

contains(mobility_modules,serviceframework): SUBDIRS += testservice2 sampleserviceplugin sampleserviceplugin2
contains(mobility_modules,contacts): SUBDIRS += contacts-tester
contains(mobility_modules,bearer) {
    symbian:SUBDIRS += bearerex
}

contains(mobility_modules,location) SUBDIRS += location-testing-tools

contains(mobility_modules,systeminfo): SUBDIRS += sysinfo-tester

contains(mobility_modules,connectivity) {
    SUBDIRS += nfctestserver

    linux*:!linux-armcc: SUBDIRS += btclient
}

symbian {
    contains(mobility_modules,messaging): SUBDIRS += messagingex
    contains(mobility_modules,publishsubscribe): SUBDIRS += publishsubscribeex
    contains(mobility_modules,connectivity): SUBDIRS += nfcsymbianbackend
}
