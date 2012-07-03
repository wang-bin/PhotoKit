
#include "Tools.h"

#include <QtGui/QImageReader>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsView>
#include "ToolTip.h"
#include "UiManager.h"
#include "Config.h"
#include "ezlog.h"

namespace PhotoKit {

namespace Tools {

void showTip(const QString &text, bool force, int msshow)
{
    if (Config::showTips || force) {
        ToolTip::showText(text, UiManager::instance()->view()->scene(), 8000);
		ezlog_debug("Tip: %s", qPrintable(text));
	}

}

QStringList imageNameFilters()
{
	static QStringList image_formats;
	if (image_formats.isEmpty()) {
		foreach(QByteArray f, QImageReader::supportedImageFormats()) {
			image_formats << QString("*." + f);
		}
	}
	return image_formats;
}

}


} //namespace PhotoKit
