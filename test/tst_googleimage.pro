QT += network
#QT -= gui

TARGET = tst_googleimage
PROJECTROOT=$$PWD/..
isEmpty(BUILD_DIR):BUILD_DIR=$$(BUILD_DIR)
isEmpty(BUILD_DIR):BUILD_DIR=$$[BUILD_DIR]
isEmpty(BUILD_DIR):BUILD_IN_SRC = yes
!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$OUT_PWD/../out
include($$PROJECTROOT/common.pri)
#DESTDIR = $$PWD

SRCPATH = $$PROJECTROOT/src
INCLUDEPATH += $$SRCPATH

SOURCES = $$SRCPATH/ImageProvider.cpp \
		$$SRCPATH/network/GoogleImageSearcher.cpp \
		tst_googleimage.cpp

HEADERS = $$SRCPATH/ImageProvider.h \
		$$SRCPATH/ImageProvider_p.h \
		$$SRCPATH/network/GoogleImageSearcher.h
