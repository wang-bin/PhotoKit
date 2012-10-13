isEmpty(PROJECTROOT): PROJECTROOT = $$PWD
INSTALL_PREFIX = /usr/local
INSTALL_FOLDER = lib
isEqual(TEMPLATE, app): INSTALL_FOLDER = bin

isEqual(TEMPLATE, app) {
	unix:!symbian {
		share.files = $$PROJECTROOT/qtc_packaging/common/changelog \
					$$PROJECTROOT/qtc_packaging/common/copyright \
					$$PROJECTROOT/qtc_packaging/common/README
		share.path = /usr/share/doc/$${TARGET}

		!isEmpty(MEEGO_VERSION_MAJOR) {
			DEFINES += CACHE_APPDIR
			INSTALL_PREFIX = /opt/$${TARGET}
			desktopfile.files = $$PROJECTROOT/qtc_packaging/debian_harmattan/$${TARGET}.desktop
			desktopfile.path = /usr/share/applications
			icon.files = $$PROJECTROOT/qtc_packaging/debian_harmattan/$${TARGET}.png
			icon.path = /usr/share/icons/hicolor/80x80/apps
			#debian.files = $$PROJECTROOT/qtc_packaging/harmattan/control
		} else:maemo5 {
			INSTALL_PREFIX = /opt/$${TARGET}
			desktopfile.files = $$PROJECTROOT/qtc_packaging/debian_fremantle/$${TARGET}.desktop
			desktopfile.path = /usr/share/applications/hildon
			icon.files = $$PROJECTROOT/qtc_packaging/debian_fremantle/$${TARGET}.png
			icon.path = /usr/share/icons/hicolor/64x64/apps
			#debian.files = $$PROJECTROOT/qtc_packaging/fremantle/control
		} else {
			desktopfile.files = $$PROJECTROOT/qtc_packaging/debian_generic/$${TARGET}.desktop
			desktopfile.path = /usr/share/applications
			icon.files = $$PROJECTROOT/qtc_packaging/debian_generic/$${TARGET}.png
			icon.path = /usr/share/icons/hicolor/64x64/apps
			#debian.files = $$PROJECTROOT/qtc_packaging/generic/control
		}
		INSTALLS += desktopfile icon share
		#debian.path = /DEBIAN
	}
}

target.path = $${INSTALL_PREFIX}/$${INSTALL_FOLDER}
INSTALLS += target



