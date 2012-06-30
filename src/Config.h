/******************************************************************************
	config.h: description
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


#ifndef PHOTOKIT_CONFIG_H
#define PHOTOKIT_CONFIG_H

#include "PhotoKit_Global.h"
#include <QtCore/QString>
#include <QtGui/QColor>

namespace PhotoKit {

class Config
{
public:
	static bool read(const QString& cfg=xml_config_path);
	static bool save(const QString& cfg=xml_config_path);

	static void detectSystemResources();
	static void postConfigure();
	static void setLowSettings();

	static QString glVersion;

	static int contentStartY;
	static int contentHeight;

	// properties:
	static bool openGlAvailable;
	static bool openGlRendering;
	static bool softwareRendering;
	static bool xRenderPresent;
	static bool noAdapt;
	static bool noTicker;
	static bool noRescale;
	static bool noAnimations;
	static bool noBlending;
	static bool noScreenSync;
	static bool useLoop;
	static bool noWindowMask;
	static bool usePixmaps;
	static bool useEightBitPalette;
	static bool fullscreen;
	static bool showBoundingRect;
	static bool showFps;
	static bool noTimerUpdate;
	static bool noTickerMorph;
	static bool useButtonBalls;
	static bool adapted;
	static bool verbose;
	static bool pause;

	static float animSpeed;
	static float animSpeedButtons;
	static float benchmarkFps;
	static int tickerLetterCount;
	static int fps;
	static int menuCount;
	static float tickerMoveSpeed;
	static float tickerMorphSpeed;
	static QString tickerText;
	static QString rootMenuName;

	static QColor color0, color1;
	static QString xml_config_path;

	static bool showLastDisplayed;
    static bool showTips;
	static int contentHMargin;
	static int contentVMargin;
	static QString thumbRecordFile;
	static QString displayedThumbRecordFile;
	static int thumbRows;
	static int thumbSpacing;
	static int thumbMargin;
	static int thumbBorder;
	static int thumbItemWidth;
	static int thumbItemHeight;
	static QString thumbDir;

	static bool keepAspectRatio;

	static QString weiboUser;
	static QString weiboPasswd;
private:
	Config(){}
};

} //namespace PhotoKit

#endif // PHOTOKIT_CONFIG_H
