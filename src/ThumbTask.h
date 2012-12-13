/******************************************************************************
	ThumbTask.h: Thumbnail creator and recorder
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


#ifndef PHOTOKIT_THUMBTASK_H
#define PHOTOKIT_THUMBTASK_H

#include "PhotoKit_Global.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
//#include <Qt/qtconcurrentexception.h> //internal namespace
//#include <QtConcurrent/QFutureWatcher>
#include <Qt/QtConcurrent> //Why in Qt5 must include this?
#else
#include <QtCore/QFutureWatcher>
#endif //QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/QImage>

namespace PhotoKit {

struct ThumbInfo
{
	QImage thumb; //OwnPtr? use thumbPath?
	QString path;
};

class ThumbTask //Singleton?
{
public:
	ThumbTask();
	~ThumbTask();

    QFutureWatcher<ThumbInfo>* watcher();
	void createThumbs(const QStringList& paths, bool create = true); //
	void createThumbsFromDirs(const QStringList& dirs, bool create = true);
	void createThumbsFromDirsAndPaths(const QStringList& dirs, const QStringList& paths, bool create = true);

    QImage thumbAt(int index);
    ThumbInfo thumbInfoAt(int index);

	void stop();
private:
#ifdef QT_NO_CONCURRENT

#else
    QFutureWatcher<ThumbInfo> *mThumbsWatcher; //OwnPtr<ThumbInfo>?
#endif //QT_NO_CONCURRENT
};

} //namespace PhotoKit

#endif // PHOTOKIT_THUMBTASK_H
