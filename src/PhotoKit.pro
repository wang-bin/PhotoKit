
QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoKit
TEMPLATE = app

!include(ezlog/src/libezlog.pri): error(could not find libezlog.pri)
!include(ProgramOptions/src/libProgramOptions.pri): error(could not find libProgramOptions.pri)
!include(NextEffect/src/libNextEffect.pri): error(could not find libNextEffect.pri)
include(../common.pri)

INCLUDEPATH += $$PWD
SOURCES += main.cpp \
	ThumbTask.cpp \
    Config.cpp \
    PhotoKitView.cpp \
    PhotoKitScene.cpp \
    ProgressBarItem.cpp \
    ThumbItem.cpp \
    CategoryItem.cpp \
    ConfigWidget.cpp \
    ButtonItem.cpp \
    WebGet.cpp \
    Updater.cpp \
    OutlineGlowItem.cpp \
    TransformMachine.cpp \
    ItemAnimation.cpp \
    ReflectEffectItem.cpp \
    ShareManager.cpp \
    Button.cpp \
    UiManager.cpp \
    ToolTip.cpp \
    ToolBar.cpp \
    OptionParser.cpp \
    tools/Tools.cpp \
    SlideDisplay.cpp \
    SlidePlayControl.cpp \
    network/weiboapi.cpp \
    network/qput.cpp \
    network/WeiboDialog.cpp

HEADERS  += \
	ThumbTask.h \
    Config.h \
    PhotoKit_Global.h \
    PhotoKitView.h \
    PhotoKitScene.h \
    ProgressBarItem.h \
    ThumbItem.h \
    CategoryItem.h \
    ConfigWidget.h \
    ButtonItem.h \
    WebGet.h \
    Updater.h \
    OutlineGlowItem.h \
    TransformMachine.h \
    ItemAnimation.h \
    ReflectEffectItem.h \
    ShareManager.h \
    Button.h \
    UiManager.h \
    ToolTip.h \
    ToolBar.h \
    OptionParser.h \
    tools/Tools.h \
    SlideDisplay.h \
    SlidePlayControl.h \
    network/weiboapi.h \
    network/qput.h \
    network/WeiboDialog.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    ../res/res.qrc
