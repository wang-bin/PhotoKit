/******************************************************************************
	ThumbRecorder.cpp: description
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


#include "ThumbRecorder.h"
#include <QtCore/QDataStream>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QStringList>
#include "Config.h"


namespace PhotoKit {

//ThumbHash ThumbRecorder::mThumbs;
//QStringList ThumbRecorder::mDisplay;
ThumbRecorder* ThumbRecorder::self = 0;
ThumbRecorder::ThumbRecorder(QObject *parent)
	:QObject(parent)
{
	self = this;
	QFile f(Config::thumbRecordFile);
	if (!f.open(QIODevice::ReadOnly)) {
		qWarning("Open thumb record file error: %s", qPrintable(f.errorString()));
	} else {QDataStream d(&f);
		d >> mThumbs;
	}
	QFile f2(Config::displayedThumbRecordFile);
	if (!f2.open(QIODevice::ReadOnly)) {
		qWarning("Open thumb record file error: %s", qPrintable(f2.errorString()));
	} else {QDataStream d2(&f2);
		d2 >> mDisplay;
	}
}

ThumbRecorder* ThumbRecorder::instance()
{
	if (!self) {
		new ThumbRecorder;
	}
	return self;
}

ThumbHash *ThumbRecorder::thumbHash()
{
	if (!self)
		new ThumbRecorder;
	return &mThumbs;
}

QStringList* ThumbRecorder::displayedThumbs()
{
	if (!self)
		new ThumbRecorder;
	return &mDisplay;
}

void ThumbRecorder::addDisplayedThumb(const QString &path)
{
	if (mDisplay.contains(path))
		return;
	mDisplay.append(path);
}

void ThumbRecorder::clearDisplay()
{
	mDisplay.clear();
}

void ThumbRecorder::save()
{
	QFile f(Config::thumbRecordFile);
	if (!f.open(QIODevice::WriteOnly)) {
		qWarning("Open thumb record file error: %s", qPrintable(f.errorString()));
		return;
	}
	QDataStream d(&f);
	d << mThumbs;
	QFile f2(Config::displayedThumbRecordFile);
	if (!f2.open(QIODevice::WriteOnly)) {
		qWarning("Open thumb record file error: %s", qPrintable(f2.errorString()));
		return;
	}
	QDataStream d2(&f2);
	d2 << mDisplay;
}

} //namespace PhotoKit
