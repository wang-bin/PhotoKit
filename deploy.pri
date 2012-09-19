isEmpty(PROJECTROOT): PROJECTROOT = $PWD
INSTALL_PREFIX = /usr/local
INSTALL_FOLDER = lib
isEqual(TEMPLATE, app): INSTALL_FOLDER = bin

isEqual(TEMPLATE, app) {
	unix:!symbian {
		share.files = $$PROJECTROOT/qtc_packaging/changelog \
					$$PROJECTROOT/qtc_packaging/copyright

		!isEmpty(MEEGO_VERSION_MAJOR) {
			DEFINES += CACHE_APPDIR
			INSTALL_PREFIX = /opt/$${TARGET}
			desktopfile.files = $$PROJECTROOT/qtc_packaging/harmattan/$${TARGET}.desktop
			desktopfile.path = /usr/share/applications
			icon.files = $$PROJECTROOT/qtc_packaging/$${TARGET}.png
			icon.path = /usr/share/icons/hicolor/80x80/apps
			#debian.files = $$PROJECTROOT/qtc_packaging/harmattan/control
		} else:maemo5 {
			desktopfile.files = $$PROJECTROOT/qtc_packaging/fremantle/$${TARGET}.desktop
			desktopfile.path = /usr/share/applications/hildon
			icon.files = $$PROJECTROOT/qtc_packaging/$${TARGET}.png
			icon.path = /usr/share/icons/hicolor/64x64/apps
			#debian.files = $$PROJECTROOT/qtc_packaging/fremantle/control
		} else {
			desktopfile.files = $$PROJECTROOT/qtc_packaging/i386/$${TARGET}.desktop
			desktopfile.path = /usr/share/applications
			icon.files = $$PROJECTROOT/qtc_packaging/$${TARGET}.png
			icon.path = /usr/share/icons/hicolor/64x64/apps
			#debian.files = $$PROJECTROOT/qtc_packaging/i386/control
		}
		INSTALLS += desktopfile icon
		#debian.path = /DEBIAN
	}
}

target.path = $${INSTALL_PREFIX}/$${INSTALL_FOLDER}
INSTALLS += target



