/******************************************************************************
	ThumbRecorder.h: description
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


#ifndef PHOTOKIT_THUMBRECORDER_H
#define PHOTOKIT_THUMBRECORDER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QStringList>
//template <typename Key, typename Value> class QHash; //enough if static member
//class QStringList;
namespace PhotoKit {

typedef QHash<QString, QString> ThumbHash;

class ThumbRecorder : public QObject
{
	Q_OBJECT
public:
	static ThumbRecorder* instance();
	ThumbHash* thumbHash();
	QStringList* displayedThumbs();
	void addDisplayedThumb(const QString& path);

	void clearDisplay();
public slots:
	void save();
private:
	ThumbRecorder(QObject *parent = 0);
	static ThumbRecorder* self;
	ThumbHash mThumbs;
	QStringList mDisplay;
};

} //namespace PhotoKit
#endif // PHOTOKIT_THUMBRECORDER_H
