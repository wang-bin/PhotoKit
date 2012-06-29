
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoKit
TEMPLATE = app

!include(ProgramOptions/src/libProgramOptions.pri): error(could not find libProgramOptions.pri)
include(../common.pri)

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
    ToolBar.cpp

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
    ToolBar.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    ../res/res.qrc
