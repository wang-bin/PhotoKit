
#include "Tools.h"

#include <QToolTip>
#include <QtGui/QImageReader>
#include <QApplication>
#include <QDesktopWidget>
#include "Config.h"

namespace PhotoKit {

namespace Tools {

void showTip(const QString &text)
{
	if (Config::showTips) {
		QToolTip::showText(qApp->desktop()->rect().center(), text);
		qDebug("Tip: %s", qPrintable(text));
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
