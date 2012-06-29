/******************************************************************************
	widget.h: description
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


#ifndef PHOTOKIT_WIDGET_H
#define PHOTOKIT_WIDGET_H

#include "PhotoKit_Global.h"

#include <QtCore/QFutureWatcher>
#include <QtGui/QImage>

namespace PhotoKit {

struct ThumbInfo
{
	QImage thumb; //OwnPtr? use thumbPath?
	QString path;
};

typedef QHash<QString, QString> ThumbHash;

class ThumbRecorder : public QObject
{
	Q_OBJECT
public:
	static ThumbHash* thumbHash();
	ThumbRecorder(QObject *parent = 0);
public slots:
	void save();
private:
	static ThumbHash thumbs;
};

class ThumbTask //Singleton?
{
public:
	ThumbTask();
	~ThumbTask();

    QFutureWatcher<ThumbInfo>* watcher();
    void createThumbs(const QStringList& paths); //
    void createThumbsFromDirs(const QStringList& dirs);
    void createThumbsFromDirsAndPaths(const QStringList& dirs, const QStringList& paths);

    QImage thumbAt(int index);
    ThumbInfo thumbInfoAt(int index);
private:
	ThumbRecorder *mThumbRecorder;
#ifdef QT_NO_CONCURRENT

#else
    QFutureWatcher<ThumbInfo> *mThumbsWatcher; //OwnPtr<ThumbInfo>?
#endif //QT_NO_CONCURRENT
};

} //namespace PhotoKit

#endif // PHOTOKIT_WIDGET_H
