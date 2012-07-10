
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
		ToolTip::showText(text, UiManager::instance()->view()->scene(), msshow);
	}

}

void showTip(const QImage &image, bool force, int msshow)
{
	if (Config::showTips || force) {
		ToolTip::showImage(image, UiManager::instance()->view()->scene(), msshow);
	}
}

void showOk(int msshow)
{
	ToolTip::showImage(QImage(":/icons/ok.png"), UiManager::instance()->view()->scene(), msshow);
}

void showError(int msshow)
{
	ToolTip::showImage(QImage(":/icons/close.png"), UiManager::instance()->view()->scene(), msshow);
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
