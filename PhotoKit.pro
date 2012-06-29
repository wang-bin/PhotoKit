#-------------------------------------------------
#
# Project created by QtCreator 2012-06-23T21:41:01
#
#-------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = libprogramoptions photokit

libprogramoptions.file = src/ProgramOptions/src/libProgramOptions.pro
photokit.file = src/PhotoKit.pro
photokit.depends += libprogramoptions

symbian {
    TARGET.UID3 = 0xea6f847b
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/PhotoKit/bin
    INSTALLS += target
}

