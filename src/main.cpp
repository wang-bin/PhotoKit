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
#include <QtCore/QLocale>
#include "ezlog.h"
#include "OptionParser.h"
#include "PhotoKitView.h"
#include "UiManager.h"
#include "tools/Tools.h"
#include "Config.h"
#include "ezlog.h"
using namespace PhotoKit;

int main(int argc, char *argv[])
{
	ezlog_init_default();
    QApplication a(argc, argv);
    OptionParser::parseCmd(argc, argv);
	QDir().mkpath(Config::thumbDir);

    QString qm("PhotoKit_" + QLocale::system().name());
	QTranslator translator;
    translator.load(qm, ":/i18n");
	a.installTranslator(&translator);
    qDebug("qm: %s", qPrintable(qm));
    PhotoKitView view;
	view.setFocus();
    UiManager::instance()->init(&view);
    //view.showFullScreen();
    view.showFullScreen();
	if (OptionParser::images.isEmpty()) {
		QStringList defalutimages = QDir(":/images").entryList(Tools::imageNameFilters()).replaceInStrings(QRegExp("^"), ":/images/");
		ezlog_debug("default images: %d", defalutimages.size());
		UiManager::instance()->showImagesFromThumb(defalutimages);
	} else {
		UiManager::instance()->showImagesFromThumb(OptionParser::images);
	}
	ezlog_debug("PhotoKit thumbdir: %s", qPrintable(Config::thumbDir));
	return a.exec();
}
