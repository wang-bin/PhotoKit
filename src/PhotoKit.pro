
QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoKit
TEMPLATE = app
PROJECTROOT = $$PWD/..
!include(ezlog/src/libezlog.pri): error(could not find libezlog.pri)
!include(ProgramOptions/src/libProgramOptions.pri): error(could not find libProgramOptions.pri)
!include(NextEffect/src/libNextEffect.pri): error(could not find libNextEffect.pri)
include($${PROJECTROOT}/common.pri)

INCLUDEPATH += $$PWD
SOURCES += main.cpp \
	ThumbTask.cpp \
    Config.cpp \
    PhotoKitView.cpp \
    PhotoKitScene.cpp \
    ThumbItem.cpp \
    OutlineGlowItem.cpp \
    TransformMachine.cpp \
    ItemAnimation.cpp \
    ReflectEffectItem.cpp \
    ShareManager.cpp \
    UiManager.cpp \
    OptionParser.cpp \
    tools/Tools.cpp \
    SlideDisplay.cpp \
    SlidePlayControl.cpp \
    network/weiboapi.cpp \
    network/qput.cpp \
    network/WeiboDialog.cpp \
    tools/ConfigDialog.cpp

HEADERS  += \
	ThumbTask.h \
    Config.h \
    PhotoKit_Global.h \
    PhotoKitView.h \
    PhotoKitScene.h \
    ThumbItem.h \
    OutlineGlowItem.h \
    TransformMachine.h \
    ItemAnimation.h \
    ReflectEffectItem.h \
    ShareManager.h \
    UiManager.h \
    OptionParser.h \
    tools/Tools.h \
    SlideDisplay.h \
    SlidePlayControl.h \
    network/weiboapi.h \
    network/qput.h \
    network/WeiboDialog.h \
    tools/ConfigDialog.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    ../res/res.qrc

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/PhotoKit/bin
    INSTALLS += target
}

maemo5 {
    target.path = /opt/PhotoKit/bin
    INSTALLS += target
}
