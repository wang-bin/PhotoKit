/******************************************************************************
	widget.cpp: description
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

#include <QApplication>
#include <QtCore/QDataStream>
#include <QtCore/QDir>
#include <QtCore/QFile>

#include <QtCore/QtConcurrentMap>
#include <QtCore/QUuid>
#include <QtGui/QImageReader>

#include "Config.h"
#include "PhotoKitScene.h"

BEGIN_NAMESPACE_PHOTOKIT

static QStringList image_formats;
ThumbHash ThumbRecorder::thumbs;
//OwnPtr<ThumbInfo>?

QFutureWatcher<ThumbInfo> *ThumbTask::mThumbsWatcher = 0;
//TODO: check md5
static ThumbInfo createThumb(const QString& path)
{
	qDebug(qPrintable(path));
	if (ThumbRecorder::thumbHash()->contains(path)) {
		ThumbInfo thumb;
		thumb.thumb = QImage(ThumbRecorder::thumbHash()->value(path));
		thumb.path = path;
		if (thumb.thumb.isNull()) {
			ThumbRecorder::thumbHash()->remove(path);
		} else {
			return thumb;
		}
	}
	QImage image(path);
	QSize s = image.size();
	qreal k = qreal(s.width())/qreal(s.height());
	qreal q = qreal(Config::thumbItemWidth)/qreal(Config::thumbItemHeight);
	if (k > q) {
		image = image.scaledToWidth(Config::thumbItemWidth);
    } else {
		image = image.scaledToHeight(Config::thumbItemHeight);
	}

	//save thumb to file. use imagewriter and setKey() update hash with
	QString thumbPath = Config::thumbDir + "/" + QUuid::createUuid().toString() + ".png"; //md5?
	ThumbRecorder::thumbHash()->insert(path, thumbPath);
	image.save(thumbPath, "PNG");
	ThumbInfo thumb;
	thumb.thumb = image;
	thumb.path = path;
	return thumb;
}

ThumbRecorder::ThumbRecorder(QObject *parent)
	:QObject(parent)
{
	QFile f(Config::thumbRecordFile);
	if (!f.open(QIODevice::ReadOnly)) {
		qWarning("Open thumb record file error: %s", qPrintable(f.errorString()));
		return;
	}
	QDataStream d(&f);
	d >> thumbs;
}

ThumbHash *ThumbRecorder::thumbHash()
{
	return &ThumbRecorder::thumbs;
}

void ThumbRecorder::save()
{
	QFile f(Config::thumbRecordFile);
	if (!f.open(QIODevice::WriteOnly)) {
		qWarning("Open thumb record file error: %s", qPrintable(f.errorString()));
		return;
	}
	QDataStream d(&f);
	d << thumbs;
}

ThumbTask::ThumbTask()
{
	image_formats.clear();
	foreach(QByteArray f, QImageReader::supportedImageFormats()) {
		image_formats << QString("*." + f);
	}
	mThumbRecorder = new ThumbRecorder;
	mThumbsWatcher = new QFutureWatcher<ThumbInfo>();
	QObject::connect(mThumbsWatcher, SIGNAL(resultReadyAt(int)), &PhotoKit::PhotoKitScene::instance(), SLOT(updateThumbItemAt(int)));
	QObject::connect(mThumbsWatcher, SIGNAL(finished()), mThumbRecorder, SLOT(save()));
}

ThumbTask::~ThumbTask()
{
	if (mThumbsWatcher) {
		if (mThumbsWatcher->isRunning())
			mThumbsWatcher->cancel();
		delete mThumbsWatcher;
		mThumbsWatcher = 0;
	}
	if (mThumbRecorder) {
		delete mThumbRecorder;
		mThumbRecorder = 0;
	}
}

void ThumbTask::createThumbs(const QStringList& paths)
{/*
	foreach(const QString& path, paths) {
		//new thumitem

	}*/
	mThumbsWatcher->setFuture(QtConcurrent::mapped(paths, createThumb));
}

void ThumbTask::createThumbsFromDir(const QString& dir)
{
	qDebug(qPrintable(image_formats.join(";")));
	QDir d(dir);
	QStringList paths = d.entryList(image_formats, QDir::Files);
	paths.replaceInStrings(QRegExp("^(.*)"), dir + "/\\1");
	createThumbs(paths);
}

QImage ThumbTask::thumbAt(int index)
{
	return mThumbsWatcher->resultAt(index).thumb;
}

END_NAMESPACE_PHOTOKIT
