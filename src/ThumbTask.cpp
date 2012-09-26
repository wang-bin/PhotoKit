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

#include "ThumbTask.h"

#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtCore/QFile>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtConcurrent/QtConcurrentMap>
#else
#include <QtCore/QtConcurrentMap>
#endif //QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/QImageReader>
#include "ThumbRecorder.h"
#include "tools/Tools.h"
#include "Config.h"
#include "ezlog.h"

namespace PhotoKit {

//OwnPtr<ThumbInfo>?

static ThumbInfo loadImage(const QString& path)
{
	ThumbInfo thumb;
	QImage image(path);
	if (image.isNull()) {
		ThumbRecorder::instance()->displayedThumbs()->removeOne(path);
	}
	thumb.thumb = image;
	thumb.path = path;
	return thumb;
}

static ThumbInfo createThumb(const QString& path)
{
	ezlog_debug("%s", qPrintable(path));
	QCryptographicHash md5sum(QCryptographicHash::Md5);
	QFile f(path);
	if (!f.open(QIODevice::ReadOnly)) {
        qWarning("open file [%s] error: %s", qPrintable(path), qPrintable(f.errorString()));
	}
	while (!f.atEnd()) {
		md5sum.addData(f.read(8192));
	}
	QString md5(md5sum.result().toHex());
	if (ThumbRecorder::instance()->thumbHash()->contains(path)) {
		QString thumb_path = ThumbRecorder::instance()->thumbHash()->value(path);
		if (thumb_path.endsWith(md5)) {
			ThumbInfo thumb;
			thumb.thumb = QImage(thumb_path);
			thumb.path = path;
			if (!thumb.thumb.isNull()) {
				ThumbRecorder::instance()->addDisplayedThumb(path);
				return thumb;
			}
		}
        qDebug("existing thumb file not match");
		ThumbRecorder::instance()->thumbHash()->remove(path);
		QFile::remove(thumb_path);
	}
	ThumbInfo thumb;
	QImage image(path);
	if (image.isNull()) {
		ThumbRecorder::instance()->displayedThumbs()->removeOne(path);
		return thumb;
	}
	QSize s = image.size();
	qreal k = qreal(s.width())/qreal(s.height());
	qreal q = qreal(Config::thumbItemWidth)/qreal(Config::thumbItemHeight);
	if (k > q) {
		image = image.scaledToWidth(Config::thumbItemWidth);// << 2).scaledToWidth(Config::thumbItemWidth, Qt::SmoothTransformation);
		//image = image.scaledToWidth(Config::thumbItemWidth, Qt::SmoothTransformation);
		//image = image.scaledToWidth(Config::thumbItemWidth << 2).scaledToWidth(Config::thumbItemWidth, Qt::SmoothTransformation);
	} else {
		image = image.scaledToHeight(Config::thumbItemHeight);// << 2).scaledToHeight(Config::thumbItemHeight, Qt::SmoothTransformation);
		//image = image.scaledToHeight(Config::thumbItemHeight, Qt::SmoothTransformation);
		//image = image.scaledToHeight(Config::thumbItemHeight << 2).scaledToHeight(Config::thumbItemHeight, Qt::SmoothTransformation);
	}

	//save thumb to file. use imagewriter and setKey() update hash with
	QString thumbPath = Config::thumbDir + "/" + md5;
	ThumbRecorder::instance()->thumbHash()->insert(path, thumbPath);
	ThumbRecorder::instance()->addDisplayedThumb(path);
	image.save(thumbPath, "PNG");
	thumb.thumb = image;
	thumb.path = path;
	return thumb;
}


ThumbTask::ThumbTask()
{
	mThumbsWatcher = new QFutureWatcher<ThumbInfo>();
	QObject::connect(mThumbsWatcher, SIGNAL(finished()), ThumbRecorder::instance(), SLOT(save()));
}

ThumbTask::~ThumbTask()
{
	if (mThumbsWatcher) {
		if (mThumbsWatcher->isRunning())
			mThumbsWatcher->cancel();
		delete mThumbsWatcher;
		mThumbsWatcher = 0;
	}
	ThumbRecorder::instance()->save();
}

QFutureWatcher<ThumbInfo>* ThumbTask::watcher()
{
    return mThumbsWatcher;
}

void ThumbTask::createThumbs(const QStringList& paths, bool create)
{/*
	foreach(const QString& path, paths) {
		//new thumitem

	}*/
	if (create)
		mThumbsWatcher->setFuture(QtConcurrent::mapped(paths, createThumb));
	else
		mThumbsWatcher->setFuture(QtConcurrent::mapped(paths, loadImage));
}

void ThumbTask::createThumbsFromDirs(const QStringList& dirs, bool create)
{
    qDebug("%s", qPrintable(Tools::imageNameFilters().join(";")));
    QStringList files;
    foreach(const QString& dir, dirs) {
        QDir d(dir);
        if (!d.exists())
            continue;
		QStringList list = d.entryList(Tools::imageNameFilters(), QDir::Files);
        list.replaceInStrings(QRegExp("^(.*)"), dir + "/\\1");
        files << list;
    }
	createThumbs(files, create);
}

void ThumbTask::createThumbsFromDirsAndPaths(const QStringList &dirs, const QStringList &paths, bool create)
{
    QStringList files;
    foreach(const QString& dir, dirs) {
        QDir d(dir);
        if (!d.exists())
            continue;
		QStringList list = d.entryList(Tools::imageNameFilters(), QDir::Files);
        list.replaceInStrings(QRegExp("^(.*)"), dir + "/\\1");
        files << list;
    }
    files << paths;
	createThumbs(files, create);
}

QImage ThumbTask::thumbAt(int index)
{
	return mThumbsWatcher->resultAt(index).thumb;
}

ThumbInfo ThumbTask::thumbInfoAt(int index)
{
    return mThumbsWatcher->resultAt(index);
}

void ThumbTask::stop()
{
	if (mThumbsWatcher->isRunning())
		mThumbsWatcher->cancel();
}

} //namespace PhotoKit
