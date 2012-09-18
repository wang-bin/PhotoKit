
QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoKit
TEMPLATE = app
LIBS += -lexif
PROJECTROOT = $$PWD/..
isEmpty(BUILD_DIR):BUILD_DIR=$$(BUILD_DIR)
isEmpty(BUILD_DIR):BUILD_DIR=$$[BUILD_DIR]
isEmpty(BUILD_DIR):BUILD_IN_SRC = yes

TRANSLATIONS += $${PROJECTROOT}/i18n/$${TARGET}_zh-cn.ts $${PROJECTROOT}/i18n/$${TARGET}_zh_CN.ts
#common.pri will be included only once and dirs are the values first time included, so include the project's
#common.pri first.
#if BUILD_DIR not set, keep the src structure
!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$PROJECTROOT/out
include($${PROJECTROOT}/common.pri)
!isEmpty(BUILD_IN_SRC):BUILD_DIR=ezlog/out
!include(ezlog/src/libezlog.pri): error(could not find libezlog.pri)
!isEmpty(BUILD_IN_SRC):BUILD_DIR=ProgramOptions/out
!include(ProgramOptions/src/libProgramOptions.pri): error(could not find libProgramOptions.pri)
!isEmpty(BUILD_IN_SRC):BUILD_DIR=NextEffect/out
!include(NextEffect/src/libNextEffect.pri): error(could not find libNextEffect.pri)
win32 {
	!isEmpty(BUILD_IN_SRC):BUILD_DIR=libexif-port/out
	!include(libexif-port/libexif.pri): error(could not find libexif.pri)
}

message(PhotoKit out=$$BUILD_DIR)
unix:!macx {
	QMAKE_RPATHDIR += $$PROJECT_LIBDIR:\'\$\$ORIGIN\':\'\$\$ORIGIN/lib\':.
	QMAKE_LFLAGS += -Wl,-z,origin
}

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
    Guide.cpp \
    DemoItemAnimation.cpp \
    tools/ToolTip.cpp \
    ToolBar.cpp \
    Button.cpp \
    ProgressBarItem.cpp \
    score.cpp \
    BaseItem.cpp \
    FlipAnimation.cpp \
    tools/ExifReader.cpp \
    Dialog.cpp \
    tools/ImageInfoDialog.cpp \
    network/WeiboDialog.cpp \
    TextEdit.cpp \
    ImageProvider.cpp \
    network/GoogleImageSearcher.cpp \
	ThumbRecorder.cpp

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
    Guide.h \
    DemoItemAnimation.h \
    tools/ToolTip.h \
    ToolBar.h \
    Button.h \
    ProgressBarItem.h \
    score.h \
    BaseItem.h \
    FlipAnimation.h \
    tools/ExifReader.h \
    Dialog.h \
    tools/ImageInfoDialog.h \
    Dialog_p.h \
    network/WeiboDialog.h \
    TextEdit.h \
    ImageProvider.h \
    ImageProvider_p.h \
    network/GoogleImageSearcher.h \
    ImageBaseInfo.h \
	ThumbRecorder.h

#CONFIG += mobility
MOBILITY = 

RESOURCES += \
    ../res/res.qrc


contains(MEEGO_EDITION,harmattan) {
	DEFINES += CACHE_APPDIR
	target.path = /opt/PhotoKit/bin
	images.files = $$PROJECTROOT/res/images
	images.path = /opt/PhotoKit/bin
	desktopfile.files = $$PROJECTROOT/qtc_packaging/harmattan/PhotoKit.desktop
    desktopfile.path = /usr/share/applications
	icon.files = $$PROJECTROOT/qtc_packaging/PhotoKit.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
	INSTALLS += target desktopfile icon images
}

maemo5 {
	target.path = /opt/PhotoKit/bin
	images.files = $$PROJECTROOT/res/images
	images.path = /opt/PhotoKit/bin
	desktopfile.files = $$PROJECTROOT/qtc_packaging/fremantle/PhotoKit.desktop
    desktopfile.path = /usr/share/applications/hildon
	icon.files = $$PROJECTROOT/qtc_packaging/PhotoKit.png
	icon.path = /usr/share/icons/hicolor/64x64/apps
	INSTALLS += target desktopfile icon images
}



maemo5 {
    desktopfile.files = PhotoKit.desktop
    desktopfile.path = /usr/share/applications/hildon
    INSTALLS += desktopfile
}

contains(MEEGO_EDITION,harmattan) {
    desktopfile.files = PhotoKit.desktop
    desktopfile.path = /usr/share/applications
    INSTALLS += desktopfile
}


message($$_PRO_FILE_PWD_)
#message($$_PRO_FILE_)
message($$OUT_PWD)
message($$PWD)
message($$IN_PWD)
message($$_FILE_)
message($$_LINE_)
message($$QMAKE_HOST.version)
message($$QMAKE_HOST.name)
message($$QMAKE_HOST.os)
message($$QMAKE_HOST.arch)
message($$BUILD_NAME)

