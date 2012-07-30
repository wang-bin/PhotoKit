TEMPLATE = lib

QT =
CONFIG   += console
CONFIG   -= app_bundle

CONFIG *= exif-buildlib

PROJECTROOT = $$PWD/../..
!include(libexif.pri): error(could not find libexif.pri)
!exists(libexif/config.h): system(touch libexif/config.h)
#src
unix: SOURCES +=
else:win32: SOURCES +=

DEFINES += GETTEXT_PACKAGE
HEADERS += \ 
    libexif/libexif/i18n.h \
    libexif/libexif/exif-utils.h \
    libexif/libexif/exif-tag.h \
    libexif/libexif/exif-system.h \
    libexif/libexif/exif-mnote-data-priv.h \
    libexif/libexif/exif-mnote-data.h \
    libexif/libexif/exif-mem.h \
    libexif/libexif/exif-log.h \
    libexif/libexif/exif-loader.h \
    libexif/libexif/exif-ifd.h \
    libexif/libexif/exif-format.h \
    libexif/libexif/exif-entry.h \
    libexif/libexif/exif-data-type.h \
    libexif/libexif/exif-data.h \
    libexif/libexif/exif-content.h \
    libexif/libexif/exif-byte-order.h \
    libexif/libexif/exif.h \
    libexif/libexif/_stdint.h \
    libexif/libexif/pentax/mnote-pentax-tag.h \
    libexif/libexif/pentax/mnote-pentax-entry.h \
    libexif/libexif/pentax/exif-mnote-data-pentax.h \
    libexif/libexif/olympus/mnote-olympus-tag.h \
    libexif/libexif/olympus/mnote-olympus-entry.h \
    libexif/libexif/olympus/exif-mnote-data-olympus.h \
    libexif/libexif/fuji/mnote-fuji-tag.h \
    libexif/libexif/fuji/mnote-fuji-entry.h \
    libexif/libexif/fuji/exif-mnote-data-fuji.h \
    libexif/libexif/canon/mnote-canon-tag.h \
    libexif/libexif/canon/mnote-canon-entry.h \
    libexif/libexif/canon/exif-mnote-data-canon.h
SOURCES += \ 
    libexif/libexif/exif-utils.c \
    libexif/libexif/exif-tag.c \
    libexif/libexif/exif-mnote-data.c \
    libexif/libexif/exif-mem.c \
    libexif/libexif/exif-log.c \
    libexif/libexif/exif-loader.c \
    libexif/libexif/exif-ifd.c \
    libexif/libexif/exif-format.c \
    libexif/libexif/exif-entry.c \
    libexif/libexif/exif-data.c \
    libexif/libexif/exif-content.c \
    libexif/libexif/exif-byte-order.c \
    libexif/libexif/pentax/mnote-pentax-tag.c \
    libexif/libexif/pentax/mnote-pentax-entry.c \
    libexif/libexif/pentax/exif-mnote-data-pentax.c \
    libexif/libexif/olympus/mnote-olympus-tag.c \
    libexif/libexif/olympus/mnote-olympus-entry.c \
    libexif/libexif/olympus/exif-mnote-data-olympus.c \
    libexif/libexif/fuji/mnote-fuji-tag.c \
    libexif/libexif/fuji/mnote-fuji-entry.c \
    libexif/libexif/fuji/exif-mnote-data-fuji.c \
    libexif/libexif/canon/mnote-canon-tag.c \
    libexif/libexif/canon/mnote-canon-entry.c \
    libexif/libexif/canon/exif-mnote-data-canon.c


