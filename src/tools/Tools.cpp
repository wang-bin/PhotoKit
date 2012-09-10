/******************************************************************************
	Tools: tiny but useful tools
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

#include "Tools.h"

#include <QtGui/QImageReader>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsView>
#include "ToolTip.h"
#include "UiManager.h"
#include "../Config.h" //"Config.h": vc or icl will fail to compile; name followed by '::' must be a class or namespace name 
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
