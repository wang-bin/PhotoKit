
QT	   += opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

TARGET = PhotoKit
TEMPLATE = app
PROJECTROOT = $$PWD/..
include($$PROJECTROOT/deploy.pri)
isEmpty(BUILD_DIR):BUILD_DIR=$$(BUILD_DIR)
isEmpty(BUILD_DIR):BUILD_DIR=$$[BUILD_DIR]
isEmpty(BUILD_DIR):BUILD_IN_SRC = yes
#TODO: lupdate doing wrong
TRANSLATIONS += $${PROJECTROOT}/i18n/$${TARGET}_zh-cn.ts $${PROJECTROOT}/i18n/$${TARGET}_zh_CN.ts
#common.pri will be included only once and dirs are the values first time included, so include the project's
#common.pri first.
#if BUILD_DIR not set, keep the src structure
!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$OUT_PWD/../out
include($${PROJECTROOT}/common.pri)
!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$OUT_PWD/ezlog/out
!include(ezlog/src/libezlog.pri): error(could not find libezlog.pri)
!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$OUT_PWD/ProgramOptions/out
!include(ProgramOptions/src/libProgramOptions.pri): error(could not find libProgramOptions.pri)
!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$OUT_PWD/NextEffect/out
!include(NextEffect/src/libNextEffect.pri): error(could not find libNextEffect.pri)
win32|mac {
	!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$OUT_PWD/libexif-port/out
	!include(libexif-port/libexif.pri): error(could not find libexif.pri)
} else {
	LIBS += -lexif
}

message(PhotoKit out=$$BUILD_DIR)
unix:!macx {
	QMAKE_RPATHDIR *= $$PROJECT_LIBDIR:\'\$\$ORIGIN\':\'\$\$ORIGIN/lib\':.
	QMAKE_LFLAGS *= -Wl,-z,origin
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
#exe icon on win
win32 {
    RC_FILE = ../res/PhotoKit.rc
}
#on mac
mac {
    ICON = ../res/PhotoKit.icns
}
OTHER_FILES += $$RESOURCES $$RC_FILE
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


