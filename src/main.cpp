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
#include <QStyleFactory>
#include <QtDebug>
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
	a.setApplicationName("PhotoKit");
	a.setObjectName("wbsecg1@gmail.com");
#if QT_VERSION >= QT_VERSION_CHECK(4, 4, 0)
	a.setApplicationVersion("0.3.8");
#endif //QT_VERSION
#ifdef CACHE_APPDIR
	//Config::setAppDir(QCoreApplication::applicationDirPath());
#endif //CACHE_APPDIR
	QDir().mkpath(Config::thumbDir);
	ezlog_debug("appdir: %s, thumbdir: %s", qPrintable(QCoreApplication::applicationDirPath()), qPrintable(Config::thumbDir));
	OptionParser::parseCmd(argc, argv);
	if (Config::logToFile) {
		ezlog_registerAppender(file_appender);
		ezlog_add_logfile("/tmp/PhotoKit.ezlog", New);
	}
	QString qm("PhotoKit_" + QLocale::system().name());
	QTranslator translator;
	translator.load(qm, ":/i18n");
	a.installTranslator(&translator);
	qDebug("qm: %s", qPrintable(qm));
	//qDebug() << QStyleFactory::keys(); //("Windows", "Motif", "CDE", "Plastique", "GTK+", "Cleanlooks")
	//a.setStyle(QStyleFactory::create("Cleanlooks"));
	/*QFile f(":/style/ui.css");
	if (f.open(QIODevice::ReadOnly)) {
		QTextStream qss(&f);
		a.setStyleSheet(qss.readAll());
	} else {
		qDebug() << "Open ui.css error:" << f.errorString();
	}*/
	PhotoKitView view;
	view.setFocus();
	UiManager::instance()->init(&view);
	view.showFullScreen();
	//view.showMaximized();
	ezlog_debug("images total: %d", OptionParser::images.size());
	if (OptionParser::images.isEmpty()) {
		ezlog_debug("add default images");
		QStringList defalutimages ;
		QString mpath(":/images/"); //ends with '/'
		if (QDir(mpath).exists())
			defalutimages = QDir(mpath).entryList(Tools::imageNameFilters()).replaceInStrings(QRegExp("^"), mpath);
		mpath = qApp->applicationDirPath() + "/images/"; //ends with '/'
		ezlog_debug("Adding '%s'", qPrintable(mpath));
		if (QDir(mpath).exists()) {
			defalutimages = QDir(mpath).entryList(Tools::imageNameFilters()).replaceInStrings(QRegExp("^"), mpath);
		}
		mpath = QDir::homePath() + "/MyDocs/Pictures/"; //Meego
		if (QDir(mpath).exists())
			defalutimages << QDir(mpath).entryList(Tools::imageNameFilters()).replaceInStrings(QRegExp("^"), mpath);
		mpath = qApp->applicationDirPath() + "/images/";
		if (QDir(mpath).exists())
			defalutimages << QDir(mpath).entryList(Tools::imageNameFilters()).replaceInStrings(QRegExp("^"), mpath);
		mpath = QDir::homePath() + "/MyDocs/.images/"; //Maemo5
		if (QDir(mpath).exists())
			defalutimages << QDir(mpath).entryList(Tools::imageNameFilters()).replaceInStrings(QRegExp("^"), mpath);
		mpath = QDir::homePath() + "/MyDocs/DCIM/"; //Maemo5
		if (QDir(mpath).exists())
			defalutimages << QDir(mpath).entryList(Tools::imageNameFilters()).replaceInStrings(QRegExp("^"), mpath);

		mpath = QDir::homePath() + "/Pictures/"; //
		if (QDir(mpath).exists())
			defalutimages << QDir(mpath).entryList(Tools::imageNameFilters()).replaceInStrings(QRegExp("^"), mpath);

		UiManager::instance()->showImagesFromThumb(defalutimages, Config::useThumb);
	} else {
		UiManager::instance()->showImagesFromThumb(OptionParser::images, Config::useThumb);
	}
	ezlog_debug("PhotoKit thumbdir: %s", qPrintable(Config::thumbDir));
	return a.exec();
}
