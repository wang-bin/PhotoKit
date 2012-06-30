TEMPLATE = lib
CONFIG += nexteffect-buildlib
include(libNextEffect.pri)

#src
SOURCES += nexteffect.cpp \
	nexteffect_none.cpp \
	nexteffect_cover.cpp \
	nexteffect_center.cpp \
	nexteffect_blinds.cpp \
	nexteffect_leave.cpp \
	nexteffect_enter.cpp \
	nexteffect_move.cpp \
	nexteffect_tile.cpp \
	nexteffect_fade.cpp \
	nexteffect_sector.cpp \
    nexteffectfactory.cpp

HEADERS  += nexteffect_global.h \
	nexteffect.h \
	nexteffect_cover.h \
	nexteffect_none.h \
	effects.h \
	nexteffect_center.h \
	nexteffect_blinds.h \
	nexteffect_leave.h \
	nexteffect_enter.h \
	nexteffect_move.h \
	nexteffect_tile.h \
	nexteffect_fade.h \
	nexteffect_sector.h \
    nexteffectfactory.h













