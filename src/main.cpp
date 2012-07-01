/******************************************************************************
	main.cpp: description
	Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/

#include <QtCore/QDir>
#include <QApplication>
#include <QtCore/QTranslator>
#include <QtDebug>
#include "ezlog.h"
#include "OptionParser.h"
#include "PhotoKitView.h"
#include "UiManager.h"
#include "tools/Tools.h"
#include "Config.h"

using namespace PhotoKit;

int main(int argc, char *argv[])
{
	ezlog_init_default();
    QApplication a(argc, argv);
    OptionParser::parseCmd(argc, argv);
	QDir().mkpath(Config::thumbDir);

	QTranslator translator;
	translator.load("PhotoKit", ":/i18n");
	a.installTranslator(&translator);

    PhotoKitView view;
	view.setFocus();
    UiManager::instance()->init(&view);
    //view.showFullScreen();
    view.showMaximized();
	qDebug() << QDir::homePath();
	if (OptionParser::images.isEmpty()) {
		QStringList defalutimages = QDir(":/images").entryList(Tools::imageNameFilters()).replaceInStrings(QRegExp("^"), ":/images/");
		foreach(QString f, defalutimages)
			qDebug() << f;
		qDebug("default images: %d", defalutimages.size());
		UiManager::instance()->showImagesFromThumb(defalutimages);
	} else {
		UiManager::instance()->showImagesFromThumb(OptionParser::images);
	}
    qDebug("PhotoKit thumbdir: %s", qPrintable(Config::thumbDir));
	return a.exec();
}
