/******************************************************************************
	Config: global configurations
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

//TODO: compress config and records
#include "Config.h"

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>
#include <QtDebug>
#include <QWidget>
#ifndef QT_NO_OPENGL
	#include <QGLWidget>
#endif
//#define QT_NO_OPENGL

namespace PhotoKit {

QString Config::glVersion("Not detected!");

bool Config::openGlRendering = true;
bool Config::softwareRendering = false;
bool Config::openGlAvailable = true;
bool Config::xRenderPresent = true;

bool Config::noTicker = false;
bool Config::noRescale = false;
bool Config::noAnimations = false;
bool Config::noBlending = false;
bool Config::noScreenSync = false;
bool Config::fullscreen = false;
bool Config::usePixmaps = false;
bool Config::useLoop = false;
bool Config::showBoundingRect = false;
float Config::animSpeed = 1.0;
bool Config::showFps = false;
int Config::fps = 60;
bool Config::noAdapt = false;
bool Config::noWindowMask = true;
bool Config::useEightBitPalette = false;
bool Config::adapted = false;
bool Config::verbose = true;

QString Config::language = "zh_CN";
QColor Config::backgroundColor = QColor(66, 66, 66);
QColor Config::glowColor = QColor(Qt::green).lighter(100);
QString Config::configPath = QDir::homePath() + "/.PhotoKit/config.ini";

bool Config::showLastDisplayed = true;
bool Config::showTips = true;

int Config::contentHMargin = 444;
int Config::contentVMargin = 120;

bool Config::useThumb = true;
QString Config::thumbRecordFile = QDir::homePath() + "/.PhotoKit/thumbs.qds";
QString Config::displayedThumbRecordFile = QDir::homePath() + "/.PhotoKit/thumbs_show.qds";
int Config::thumbRows = 2; //TODO: 2 for mobile
int Config::thumbSpacing = 4;
int Config::thumbMargin = 4;
int Config::thumbBorder = 1;
int Config::thumbItemWidth = 160;
int Config::thumbItemHeight = 120;
QString Config::thumbDir = QDir::homePath() + "/.PhotoKit/thumb";

bool Config::keepAspectRatio = true;

QString Config::weiboUser;
QString Config::weiboPasswd;

bool Config::logToFile = false;

void Config::setAppDir(const QString &dir)
{
    Config::configPath = dir + "/.PhotoKit/config.ini";
    Config::thumbRecordFile = dir + "/.PhotoKit/thumbs.qds";
    Config::displayedThumbRecordFile = dir + "/.PhotoKit/thumbs_show.qds";
    Config::thumbDir = dir + "/.PhotoKit/thumb";
    QDir().mkpath(Config::thumbDir);
}

bool Config::read(const QString& ini)
{
    //TODO: version check
	if (QFileInfo(ini).created() < QDateTime(QDate(2012, 7, 8))) { //remove old version configuration
        QFile(ini).remove();
        QFile(Config::displayedThumbRecordFile).remove();
    }
	QSettings cfg(ini, QSettings::IniFormat);
	cfg.setIniCodec("UTF-8");
	cfg.setValue("configPath", ini);
	cfg.setPath(QSettings::IniFormat, QSettings::UserScope, ini);

	backgroundColor.setRgb(cfg.value("backgroundColor", QColor(66, 66, 66).rgb()).value<QRgb>());
	glowColor.setRgb(cfg.value("glowColor", QColor(Qt::green).lighter(100).rgb()).value<QRgb>());
	openGlRendering = cfg.value("opengl", true).toBool();
	showTips = cfg.value("showTips", true).toBool();
	weiboUser = qUncompress(cfg.value("weiboUser", "").toByteArray());
	weiboPasswd = qUncompress(cfg.value("weiboPasswd", "").toByteArray());
	qDebug() << "weibo: " << weiboUser << " " << weiboPasswd;

	contentHMargin = cfg.value("contentHMargin", 666).toInt();
	contentVMargin = cfg.value("contentVMargin", 88).toInt();
    thumbRows = cfg.value("thumbRows", 2).toInt();
	thumbSpacing = cfg.value("thumbSpacing", 4).toInt();
	thumbMargin = cfg.value("thumbMargin", 4).toInt();
	thumbBorder = cfg.value("thumbBorder", 1).toInt();

	thumbItemWidth = cfg.value("thumbItemWidth", 160).toInt();
	thumbItemHeight = cfg.value("thumbItemHeight", 120).toInt();
	useThumb = cfg.value("useThumb", true).toBool();

	return true;
}

bool Config::save(const QString &ini)
{
	QSettings cfg(ini, QSettings::IniFormat);
	cfg.setIniCodec("UTF-8");
	cfg.setValue("configPath", ini);
	cfg.setValue("backgroundColor", backgroundColor.rgb());
	cfg.setValue("glowColor", glowColor.rgb());
	cfg.setValue("opengl", openGlRendering);
	cfg.setValue("showTips", showTips);
	cfg.setValue("weiboUser", qCompress(weiboUser.toLatin1()));
	cfg.setValue("weiboPasswd", qCompress(weiboPasswd.toLatin1()));
	cfg.setValue("contentHMargin", contentHMargin);
	cfg.setValue("contentVMargin", contentVMargin);
	cfg.setValue("thumbRows", thumbRows);
	cfg.setValue("thumbSpacing", thumbSpacing);
	cfg.setValue("thumbMargin", thumbMargin);
	cfg.setValue("thumbBorder", thumbBorder);
	cfg.setValue("thumbItemWidth", thumbItemWidth);
	cfg.setValue("thumbItemHeight", thumbItemHeight);
	cfg.setValue("useThumb", useThumb);

	return true;
}

void Config::setLowSettings()
{
	Config::openGlRendering = false;
	Config::softwareRendering = true;
	Config::noTicker = true;
	Config::fps = 30;
	Config::usePixmaps = true;
	Config::noAnimations = true;
	Config::noBlending = true;
}

void Config::detectSystemResources()
{
#ifndef QT_NO_OPENGL
	Config::openGlAvailable = QGLFormat::hasOpenGL();
	if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_ES_Version_2_0)
		Config::glVersion = "ES 2.0";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_ES_Common_Version_1_0)
		Config::glVersion = "ES Common 1.0";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_ES_CommonLite_Version_1_0)
		Config::glVersion = "ES Common Lite 1.0";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_ES_Common_Version_1_1)
		Config::glVersion = "ES Common 1.1";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_ES_CommonLite_Version_1_1)
		Config::glVersion = "ES Common Lite 1.1";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_2_1)
		Config::glVersion = "2.1";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_2_0)
		Config::glVersion = "2.0";
#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_4_0)
		Config::glVersion = "4.0";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_3_3)
		Config::glVersion = "3.3";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_3_2)
		Config::glVersion = "3.2";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_3_1)
		Config::glVersion = "3.1";
#endif //QT_VERSION
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_3_0)
		Config::glVersion = "3.0";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_5)
		Config::glVersion = "1.5";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_4)
		Config::glVersion = "1.4";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_3)
		Config::glVersion = "1.3";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_2)
		Config::glVersion = "1.2";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_1)
		Config::glVersion = "1.1";

	if (Config::verbose)
		qDebug() << "- OpenGL version:" << Config::glVersion;
/*
	QGLWidget glw;
	if (!QGLFormat::hasOpenGL()
		|| !glw.format().directRendering()
		|| !(QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_5)
		|| glw.depth() < 24
    )
#else
	if (Config::verbose)
		qDebug() << "- OpenGL not supported by current build of Qt";
#endif
	{
		Config::openGlAvailable = false;
		if (Config::verbose)
			qDebug("- OpenGL not recommended on this system");
	}

#if defined(Q_WS_X11)
	// check if X render is present:
	QPixmap tmp(1, 1);
	if (!tmp.x11PictureHandle() && tmp.paintEngine()->type() == QPaintEngine::X11){
		Config::xRenderPresent = false;
		if (Config::verbose)
			qDebug("- X render not present");
	}
*/
#endif
/*
	QWidget w;
	if (Config::verbose)
		qDebug() << "- Color depth: " << QString::number(w.depth());*/

}

void Config::postConfigure()
{
	if (!Config::noAdapt){
		QWidget w;
		if (w.depth() < 16){
			Config::useEightBitPalette = true;
			Config::adapted = true;
			if (Config::verbose)
				qDebug() << "- Adapt: Color depth less than 16 bit. Using 8 bit palette";
		}

		if (!Config::xRenderPresent){
			Config::setLowSettings();
			Config::adapted = true;
			if (Config::verbose)
				qDebug() << "- Adapt: X renderer not present. Using low settings";
		}
	}

	if (!Config::openGlRendering && !Config::softwareRendering){
		// The user has not decided rendering system. So we do it instead:
		if (Config::openGlAvailable)
			Config::openGlRendering = true;
		else
			Config::softwareRendering = true;
	}
}


} //namespace PhotoKit
