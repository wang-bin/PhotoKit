/******************************************************************************
	GoogleImageSearcher: a wrapper for google image search api
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


#ifndef PHOTOKIT_GOOGLEIMAGESEARCHER_H
#define PHOTOKIT_GOOGLEIMAGESEARCHER_H

#include "ImageProvider.h"

/*!
	https://developers.google.com/image-search/v1/jsondevguide
*/

class QNetworkReply;
namespace PhotoKit {

class GoogleImageSearcherPrivate;
class GoogleImageSearcher : public ImageProvider
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(GoogleImageSearcher)
public:
	enum SizeFilter {
		IconSize = 0, //imgsz=icon
		MediumSize, //imgsz=small|medium|large|xlarge
		LargeSize, //imgsz=xxlarge
		HugeSize //imgsz=huge
	};

	explicit GoogleImageSearcher(QObject *parent = 0);
	void setSizeFilter(SizeFilter size);

	virtual bool canFetchMore() const;
	virtual void fetchMore();

	void reset();

private:
	void parseJson(const QByteArray& json);
private slots:
	void parseReply(QNetworkReply* reply);
};

} //namespace PhotoKit
#endif // PHOTOKIT_GOOGLEIMAGESEARCHER_H
