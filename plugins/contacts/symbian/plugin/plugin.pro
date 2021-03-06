TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(qtcontacts_symbian)
PLUGIN_TYPE=contacts

include(../../../../common.pri)
include(symbian_defines.pri)
symbian: { 
    load(data_caging_paths)

    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x2002AC7B
  
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
	
    INCLUDEPATH += inc
    INCLUDEPATH += rss
    INCLUDEPATH += $$SOURCE_DIR/contacts
    INCLUDEPATH += $$SOURCE_DIR/contacts/details
    INCLUDEPATH += $$SOURCE_DIR/contacts/filters
    INCLUDEPATH += $$SOURCE_DIR/contacts/requests

    HEADERS += \
        $$PUBLIC_HEADERS \
        inc/cntsymbianengine.h \
        inc/cntabstractrelationship.h \
        inc/cntrelationshipgroup.h \
        inc/cntsymbiantransformerror.h \
        inc/cntsymbiandatabase.h \
        inc/cntdisplaylabel.h \
        inc/cntrelationship.h \
        inc/transform/cnttransformcontact.h \
        inc/transform/cnttransformcontactdata.h \
        inc/transform/cnttransformname.h \
        inc/transform/cnttransformnickname.h \
        inc/transform/cnttransformphonenumber.h \
        inc/transform/cnttransformemail.h \
        inc/transform/cnttransformaddress.h \
        inc/transform/cnttransformurl.h \
        inc/transform/cnttransformbirthday.h \
        inc/transform/cnttransformonlineaccount.h \
        inc/transform/cnttransformonlineaccountsimple.h \
        inc/transform/cnttransformorganisation.h \
        inc/transform/cnttransformpresence.h \
        inc/transform/cnttransformringtone.h \
        inc/transform/cnttransformthumbnail.h \
        inc/transform/cnttransformavatar.h \
        inc/transform/cntthumbnailcreator.h \
        inc/transform/cnttransformsynctarget.h \
        inc/transform/cnttransformgender.h \
        inc/transform/cnttransformanniversary.h \
        inc/transform/cnttransformanniversarysimple.h \
        inc/transform/cnttransformgeolocation.h \
        inc/transform/cnttransformnote.h \
        inc/transform/cnttransformfamily.h \
        inc/transform/cnttransformempty.h \
        inc/transform/cnttransformfavorite.h \
        inc/filtering/cntfilterrelationship.h \
        inc/filtering/cntfilterdetail.h \
        inc/filtering/cntdbinfo.h \
        inc/filtering/cntfilterdefault.h \
        inc/filtering/cntfilterintersection.h \
        inc/filtering/cntfilterunion.h \
        inc/filtering/cntabstractcontactsorter.h \
        inc/filtering/cntabstractcontactfilter.h \
        inc/filtering/cntsymbianfilterdbms.h \
        inc/filtering/cntsymbianfiltersql.h \
        inc/filtering/cntsymbiansorterdbms.h \
        inc/filtering/cntsymbiansrvconnection.h \
                
    SOURCES += \
        src/transform/cnttransformcontact.cpp \
        src/transform/cnttransformcontactdata.cpp \
        src/transform/cnttransformname.cpp \
        src/transform/cnttransformnickname.cpp \
        src/transform/cnttransformphonenumber.cpp \
        src/transform/cnttransformemail.cpp \
        src/transform/cnttransformaddress.cpp \
        src/transform/cnttransformurl.cpp \
        src/transform/cnttransformbirthday.cpp \
        src/transform/cnttransformonlineaccount.cpp \
        src/transform/cnttransformonlineaccountsimple.cpp \
        src/transform/cnttransformorganisation.cpp \
        src/transform/cnttransformpresence.cpp \
        src/transform/cnttransformringtone.cpp \
        src/transform/cnttransformthumbnail.cpp \
        src/transform/cnttransformavatar.cpp \
        src/transform/cntthumbnailcreator.cpp\
        src/transform/cnttransformsynctarget.cpp \
        src/transform/cnttransformgender.cpp \
        src/transform/cnttransformanniversary.cpp \
        src/transform/cnttransformanniversarysimple.cpp \
        src/transform/cnttransformgeolocation.cpp \
        src/transform/cnttransformnote.cpp \
        src/transform/cnttransformfamily.cpp \
        src/transform/cnttransformempty.cpp \
        src/transform/cnttransformfavorite.cpp \
        src/filtering/cntfilterrelationship.cpp \
        src/filtering/cntfilterdetail.cpp \
        src/filtering/cntdbinfo.cpp \
        src/filtering/cntfilterdefault.cpp \
        src/filtering/cntfilterintersection.cpp \
        src/filtering/cntfilterunion.cpp \
        src/filtering/cntsymbianfilterdbms.cpp \
        src/filtering/cntsymbianfiltersql.cpp \
        src/filtering/cntsymbiansorterdbms.cpp \
        src/filtering/cntsymbiansrvconnection.cpp \
        src/cntsymbianengine.cpp \
        src/cntabstractrelationship.cpp \
        src/cntrelationshipgroup.cpp \
        src/cntsymbiantransformerror.cpp \
        src/cntsymbiandatabase.cpp \
        src/cntdisplaylabel.cpp \
        src/cntrelationship.cpp 

    CONFIG += mobility
    MOBILITY = contacts

    LIBS += \
        -lcntmodel \
        -lcentralrepository \
        -lestor \
        -lflogger \
        -lefsrv \
        -lfbscli \
        -limageconversion \
        -lbitmaptransforms \
        -lbafl \
        -ltzclient

    target.path = /sys/bin
    INSTALLS += target

    contains(DEFINES, SYMBIAN_BACKEND_USE_CNTMODEL_V2) {
        cntmodelResourceFile = \
            "START RESOURCE ../../rss/customisedcntmodel.rss" \
            "TARGETPATH $${CONTACTS_RESOURCE_DIR}" \
            "LANGUAGE_IDS" \
            "END"
        MMP_RULES += cntmodelResourceFile
    }
    
    symbianplugin.sources = $${TARGET}.dll
    symbianplugin.path = $${QT_PLUGINS_BASE_DIR}/$${PLUGIN_TYPE}
    DEPLOYMENT += symbianplugin
    
    # Public header
    headers.sources = inc/cntbackendsdefs.h
    for(header, headers.sources):BLD_INF_RULES.prj_exports += "$$header $$basename(header)"
}
