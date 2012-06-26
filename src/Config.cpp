/******************************************************************************
	config.cpp: description
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


#include "Config.h"

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtDebug>
#ifndef QT_NO_OPENGL
	#include <QGLWidget>
#endif
#include <QtGui/QMessageBox>
//#define QT_NO_OPENGL

BEGIN_NAMESPACE_PHOTOKIT


// Config:
QColor Config::sceneBg1(QColor(91, 91, 91));
QColor Config::sceneBg1Line(QColor(114, 108, 104));
QColor Config::sceneBg2(QColor(0, 0, 0));
QColor Config::sceneLine(255, 255, 255);
//QColor Config::paperBg(QColor(100, 100, 100));
QColor Config::menuTextFg(QColor(255, 0, 0));
QColor Config::buttonBgLow(QColor(255, 255, 255, 90));
QColor Config::buttonBgHigh(QColor(255, 255, 255, 20));
QColor Config::buttonText(QColor(255, 255, 255));
QColor Config::tt_green(QColor(166, 206, 57));
QColor Config::fadeOut(QColor(206, 246, 117, 0));
QColor Config::heading(QColor(190,230,80));
QString Config::glVersion("Not detected!");

// Guides:
int Config::stageStartY = 8;
int Config::stageHeight = 536;
int Config::stageStartX = 8;
int Config::stageWidth = 785;
int Config::contentStartY = 22;
int Config::contentHeight = 510;

// Properties:
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
bool Config::showFps = false;
bool Config::noAdapt = false;
bool Config::noWindowMask = true;
bool Config::useButtonBalls = false;
bool Config::useEightBitPalette = false;
bool Config::noTimerUpdate = false;
bool Config::noTickerMorph = false;
bool Config::adapted = false;
bool Config::verbose = true;
bool Config::pause = true;
int Config::fps = 60;
int Config::menuCount = 18;
float Config::animSpeed = 1.0;
float Config::animSpeedButtons = 1.0;
float Config::benchmarkFps = -1;
int Config::tickerLetterCount = 80;
float Config::tickerMoveSpeed = 0.4f;
float Config::tickerMorphSpeed = 2.5f;
QString Config::tickerText = QString::fromUtf8(".EROM ETAERC .SSEL EDOC");
QString Config::rootMenuName = "Qt Examples and Demos";
QColor Config::color0=QColor(102, 175, 54, 200);
QColor Config::color1=QColor(102, 175, 54, 60);
QString Config::xml_config_path="xml/config.ini";

int Config::contentHMargin = 666;
int Config::contentVMargin = 88;

QString Config::thumbRecordFile = QDir::homePath() + "/PhotoKit/thumbs.qds";
int Config::thumbRows = 3; //TODO: 2 for mobile
int Config::thumbSpacing = 4;
int Config::thumbMargin = 4;
int Config::thumbBorder = 1;
int Config::thumbItemWidth = 160;
int Config::thumbItemHeight = 120;
QString Config::thumbDir = QDir::homePath() + "/PhotoKit/.thumb";

bool Config::read(const QString& xml)
{//r,font
	QSettings cfg(xml,QSettings::IniFormat);
	cfg.setIniCodec("UTF-8");
	cfg.setValue("path",xml);
	cfg.setPath(QSettings::IniFormat,QSettings::UserScope,xml);
	qDebug()<<cfg.fileName()<<" w: "<<cfg.isWritable();
	color0.setRgba(cfg.value("color0",QColor(255, 85, 54, 200).rgba()).value<QRgb>());
	color1.setRgba(cfg.value("color1",QColor(255, 85, 54, 60).rgba()).value<QRgb>());
	tickerText=cfg.value("tickerText",QString::fromUtf8(".EROM ETAERC .SSEL EDOC")).toString();
	tickerLetterCount=cfg.value("tickerLetterCount",Config::tickerLetterCount).toInt();

#if defined(Q_OS_MAC)
	cfg.value("contentFont/famaly","Arial");
#else
	cfg.value("contentFont/famaly","Verdana");
#endif

	return true;
}

bool Config::save(const QString &xml)
{
	QSettings cfg(xml,QSettings::IniFormat);
	cfg.setIniCodec("UTF-8");
	cfg.setValue("path",xml);
	cfg.setValue("color0",color0.rgba());
	cfg.setValue("color1",color1.rgba());
	cfg.setValue("tickerText",Config::tickerText);
	cfg.setValue("tickerLetterCount",Config::tickerLetterCount);


	return true;
}
/*
QFont Config::contentFont()
{
	QFont font;
	font.setStyleStrategy(QFont::PreferAntialias);
#if defined(Q_OS_MAC)
	font.setPixelSize(14);
	font.setFamily("Arial");
#else
	font.setPixelSize(13);
	font.setFamily("Verdana");
#endif
	return font;
}

QFont Config::headingFont()
{
	QFont font;
	font.setStyleStrategy(QFont::PreferAntialias);
	font.setPixelSize(23);
	font.setBold(true);
	font.setFamily("Verdana");
	return font;
}

QFont Config::buttonFont()
{
	QFont font;
	font.setStyleStrategy(QFont::PreferAntialias);
#if 0//defined(Q_OS_MAC)
	font.setPixelSize(11);
	font.setFamily("Silom");
#else
	font.setPixelSize(11);
	font.setFamily("Verdana");
#endif
	return font;
}

QFont Config::tickerFont()
{
	QFont font;
	font.setStyleStrategy(QFont::PreferAntialias);
#if defined(Q_OS_MAC)
	font.setPixelSize(11);
	font.setBold(true);
	font.setFamily("Arial");
#else
	font.setPixelSize(10);
	font.setBold(true);
	font.setFamily("sans serif");
#endif
	return font;
}
*/
float parseFloat(const QString &argument, const QString &name)
{
	if (name.length() == argument.length()){
		QMessageBox::warning(0, "Arguments",
								 QString("No argument number found for ")
								 + name
								 + ". Remember to put name and value adjacent! (e.g. -fps100)");
		exit(0);
	}
	float value = argument.mid(name.length()).toFloat();
	return value;
}

QString parseText(const QString &argument, const QString &name)
{
	if (name.length() == argument.length()){
		QMessageBox::warning(0, "Arguments",
								 QString("No argument number found for ")
								 + name
								 + ". Remember to put name and value adjacent! (e.g. -fps100)");
		exit(0);
	}
	QString value = argument.mid(name.length());
	return value;
}

void Config::parseArgs(int argc, char *argv[])
{
	// some arguments should be processed before
	// others. Handle them now:
	for (int i=1; i<argc; i++){
		QString s(argv[i]);
		if (s == "-verbose")
			Config::verbose = true;
	}

	Config::detectSystemResources();

	// Handle the rest of the arguments. They may
	// override attributes already set:
	for (int i=1; i<argc; i++){
		QString s(argv[i]);
		if (s == "-opengl")
			Config::openGlRendering = true;
		else if (s == "-software")
			Config::softwareRendering = true;
		else if (s == "-no-opengl") // support old style
			Config::softwareRendering = true;
		else if (s == "-no-ticker") // support old style
			Config::noTicker = true;
		else if (s.startsWith("-ticker"))
			Config::noTicker = !bool(parseFloat(s, "-ticker"));
		else if (s == "-no-animations")
			Config::noAnimations = true; // support old style
		else if (s.startsWith("-animations"))
			Config::noAnimations = !bool(parseFloat(s, "-animations"));
		else if (s == "-no-adapt")
			Config::noAdapt = true;
		else if (s == "-low")
			Config::setLowSettings();
		else if (s == "-no-rescale")
			Config::noRescale = true;
		else if (s == "-use-pixmaps")
			Config::usePixmaps = true;
		else if (s == "-fullscreen")
			Config::fullscreen = true;
		else if (s == "-show-br")
			Config::showBoundingRect = true;
		else if (s == "-show-fps")
			Config::showFps = true;
		else if (s == "-no-blending")
			Config::noBlending = true;
		else if (s == "-no-sync")
			Config::noScreenSync = true;
		else if (s.startsWith("-menu"))
			Config::menuCount = int(parseFloat(s, "-menu"));
		else if (s.startsWith("-use-timer-update"))
			Config::noTimerUpdate = !bool(parseFloat(s, "-use-timer-update"));
		else if (s.startsWith("-pause"))
			Config::pause = bool(parseFloat(s, "-pause"));
		else if (s == "-no-ticker-morph")
			Config::noTickerMorph = true;
		else if (s == "-use-window-mask")
			Config::noWindowMask = false;
		else if (s == "-use-loop")
			Config::useLoop = true;
		else if (s == "-use-8bit")
			Config::useEightBitPalette = true;
		else if (s.startsWith("-8bit"))
			Config::useEightBitPalette = bool(parseFloat(s, "-8bit"));
		else if (s == "-use-balls")
			Config::useButtonBalls = true;
		else if (s.startsWith("-ticker-letters"))
			Config::tickerLetterCount = int(parseFloat(s, "-ticker-letters"));
		else if (s.startsWith("-ticker-text"))
			Config::tickerText = parseText(s, "-ticker-text");
		else if (s.startsWith("-ticker-speed"))
			Config::tickerMoveSpeed = parseFloat(s, "-ticker-speed");
		else if (s.startsWith("-ticker-morph-speed"))
			Config::tickerMorphSpeed = parseFloat(s, "-ticker-morph-speed");
		else if (s.startsWith("-animation-speed"))
			Config::animSpeed = parseFloat(s, "-animation-speed");
		else if (s.startsWith("-fps"))
			Config::fps = int(parseFloat(s, "-fps"));
		else if (s.startsWith("-h") || s.startsWith("-help")){
			QMessageBox::warning(0, "Arguments",
								 QString("Usage: qtdemo [-verbose] [-no-adapt] [-opengl] [-software] [-fullscreen] [-ticker[0|1]] ")
								 + "[-animations[0|1]] [-no-blending] [-no-sync] [-use-timer-update[0|1]] [-pause[0|1]]  "
								 + "[-use-window-mask] [-no-rescale] "
								 + "[-use-pixmaps] [-show-fps] [-show-br] [-8bit[0|1]] [-menu<int>] [-use-loop] [-use-balls] "
								 + "[-animation-speed<float>] [-fps<int>] "
								 + "[-low] [-ticker-letters<int>] [-ticker-speed<float>] [-no-ticker-morph] "
								 + "[-ticker-morph-speed<float>] [-ticker-text<string>]");
			exit(0);
		} else if (s == "-verbose") {
			// this option was already handled above
		} else{
			QMessageBox::warning(0, "QtDemo", QString("Unrecognized argument:\n") + s);
			exit(0);
		}
	}

	Config::postConfigure();

}

void Config::setLowSettings()
{
	Config::openGlRendering = false;
	Config::softwareRendering = true;
	Config::noTicker = true;
	Config::noTimerUpdate = true;
	Config::fps = 30;
	Config::usePixmaps = true;
	Config::noAnimations = true;
	Config::noBlending = true;
}

void Config::detectSystemResources()
{
#ifndef QT_NO_OPENGL
	if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_2_0)
		Config::glVersion = "2.0 or higher";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_5)
		Config::glVersion = "1.5";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_4)
		Config::glVersion = "1.4";
	else if (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_3)
		Config::glVersion = "1.3 or lower";
	if (Config::verbose)
		qDebug() << "- OpenGL version:" << Config::glVersion;

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

#endif

	QWidget w;
	if (Config::verbose)
		qDebug() << "- Color depth: " << QString::number(w.depth());
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


END_NAMESPACE_PHOTOKIT
