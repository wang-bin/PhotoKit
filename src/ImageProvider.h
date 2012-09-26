/******************************************************************************
	ImageProvider: base class for image filtering
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


#ifndef PHOTOKIT_IMAGEPROVIDER_H
#define PHOTOKIT_IMAGEPROVIDER_H

#include <QtCore/QObject>
#include <QtGui/QImage>

namespace PhotoKit {

class ImageBaseInfo
{
public:
	int width, height;
	int thumbWidth, thumbHeight;
	QString path;
	QString thumbPath;
	//QString title;
};

class ImageProviderPrivate;
class ImageProvider : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(ImageProvider)
public:
	ImageProvider(QObject *parent = 0);
	virtual ~ImageProvider();

	void setNameFilter(const QString& filter);
	//void setSizeFilter();

	virtual bool canFetchMore() const;
	virtual void fetchMore() = 0;

signals:
	//void imageReady(const QImage& image);
	void imageReady(const ImageBaseInfo& path);
	void imagesReady(const QList<QImage>& paths);
	//void imagesReady(const QList<ImageBaseInfo>& paths);


protected:
	ImageProvider(ImageProviderPrivate& d, QObject* parent = 0);

	ImageProviderPrivate *d_ptr;
};

} //namespace PhotoKit
#endif // PHOTOKIT_IMAGEPROVIDER_H
