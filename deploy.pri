isEmpty(PROJECTROOT): PROJECTROOT = $PWD
INSTALL_PREFIX = /usr/local
INSTALL_FOLDER = lib
isEqual(TEMPLATE, app): INSTALL_FOLDER = bin

isEqual(TEMPLATE, app) {
	unix:!symbian {
		icon.files = $$PROJECTROOT/qtc_packaging/$${TARGET}.png
		!isEmpty(MEEGO_VERSION_MAJOR) {
			DEFINES += CACHE_APPDIR
			INSTALL_PREFIX = /opt/$${TARGET}
			desktopfile.files = $$PROJECTROOT/qtc_packaging/harmattan/$${TARGET}.desktop
			desktopfile.path = /usr/share/applications
			icon.path = /usr/share/icons/hicolor/80x80/apps
		} else:maemo5 {
			desktopfile.files = $$PROJECTROOT/qtc_packaging/fremantle/$${TARGET}.desktop
			desktopfile.path = /usr/share/applications/hildon
			icon.path = /usr/share/icons/hicolor/64x64/apps
		}
		INSTALLS += desktopfile icon
	}
}

target.path = $${INSTALL_PREFIX}/$${INSTALL_FOLDER}
INSTALLS += target

