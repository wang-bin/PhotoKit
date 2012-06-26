#-------------------------------------------------
#
# Project created by QtCreator 2012-06-23T21:41:01
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoKit
TEMPLATE = app


SOURCES += src/main.cpp \
	src/ThumbTask.cpp \
    src/Config.cpp \
    src/PhotoKitView.cpp \
    src/PhotoKitScene.cpp \
    src/ProgressBarItem.cpp \
    src/ThumbItem.cpp \
    src/CategoryItem.cpp \
    src/ConfigWidget.cpp \
    src/ButtonItem.cpp \
    src/WebGet.cpp \
    src/Updater.cpp \
    src/OutlineGlowItem.cpp \
    src/TransformMachine.cpp \
    src/ItemAnimation.cpp \
    src/ReflectEffectItem.cpp

HEADERS  += \
	src/ThumbTask.h \
    src/Config.h \
    src/PhotoKit_Global.h \
    src/PhotoKitView.h \
    src/PhotoKitScene.h \
    src/ProgressBarItem.h \
    src/ThumbItem.h \
    src/CategoryItem.h \
    src/ConfigWidget.h \
    src/ButtonItem.h \
    src/WebGet.h \
    src/Updater.h \
    src/OutlineGlowItem.h \
    src/TransformMachine.h \
    src/ItemAnimation.h \
    src/ReflectEffectItem.h

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xea6f847b
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
