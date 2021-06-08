#-------------------------------------------------
#
# Project created by QtCreator 2020-08-03T16:42:17
#
#-------------------------------------------------

QT       += widgets network

TARGET = TmctlPlugin
TEMPLATE = lib

DEFINES += TMCTLPLUGIN_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TmctlPlugin.cpp \
    LTmctlTrans.cpp \
    LTmctlToolWidget.cpp \
    LTmctlSettingsWidget.cpp \
    LDataProcessing.cpp \
    LTmctlSettingsRelier.cpp \
    common/LCanDataManager.cpp \
    common/LDbcManagerDllWrapper.cpp \
    common/LUniqueResource.cpp \
    common/LUsbAdcDataManager.cpp \
    common/LUsbCanDllWrapper.cpp \
    common/LYavUsbAdcDllWrapper.cpp \
    common/LTmctlWrapper.cpp \
    common/LTmctlWrapper.cpp

HEADERS += \
    TmctlPlugin.h \
    tmctlplugin_global.h \
    LTmctlTrans.h \
    LTmctlToolWidget.h \
    LTmctlSettingsWidget.h \
    LTmctlData.h \
    LDataProcessing.h \
    LTmctlSettingsRelier.h \
    common/LAttributes.h \
    common/LCanDataManager.h \
    common/LCanDataReceiver.h \
    common/LCommon.h \
    common/LCommonError.h \
    common/LDataValue.h \
    common/LDbcManagerDllWrapper.h \
    common/LObixObj.h \
    common/LObixObjTree.h \
    common/LPath.h \
    common/LSettingsRelier.h \
    common/LTransmission.h \
    common/LTransPluginFactory.h \
    common/LTree.h \
    common/LUniqueResource.h \
    common/LUsbAdcDataManager.h \
    common/LUsbAdcDataReceiver.h \
    common/LUsbCanDllWrapper.h \
    common/LYavUsbAdcDllWrapper.h \
    common/LTmctlWrapper.h \
    common/LTmctlWrapper.h

INCLUDEPATH += \
    common \

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    LTmctlToolWidget.ui \
    LTmctlSettingsWidget.ui
