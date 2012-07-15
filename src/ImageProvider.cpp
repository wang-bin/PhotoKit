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


#include "ImageProvider.h"
#include "ImageProvider_p.h"

namespace PhotoKit {


ImageProvider::ImageProvider(QObject *parent)
	:QObject(parent),d_ptr(new ImageProviderPrivate)
{
}

ImageProvider::ImageProvider(ImageProviderPrivate &d, QObject *parent)
	:QObject(parent),d_ptr(&d)
{

}

ImageProvider::~ImageProvider()
{
	if (d_ptr) {
		delete d_ptr;
		d_ptr = 0;
	}
}

void ImageProvider::setNameFilter(const QString &filter)
{
	Q_D(ImageProvider);
	d->nameFilter = filter;
	d->page = 0; //reset for a new search
}

bool ImageProvider::canFetchMore() const
{
	return true;
}

} //namespace PhotoKit
