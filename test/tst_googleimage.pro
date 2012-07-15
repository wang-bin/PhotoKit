QT += network
QT -= gui

TARGET = tst_googleimage
include(../common.pri)
DESTDIR = $$PWD

SRCPATH = ../src
INCLUDEPATH += $$SRCPATH

SOURCES = $$SRCPATH/ImageProvider.cpp \
		$$SRCPATH/network/GoogleImageSearcher.cpp \
		tst_googleimage.cpp

HEADERS = $$SRCPATH/ImageProvider.h \
		$$SRCPATH/ImageProvider_p.h \
		$$SRCPATH/network/GoogleImageSearcher.h
