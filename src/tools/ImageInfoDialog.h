/******************************************************************************
	ImageInfoDialog: a dialog to display image information
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


#ifndef PHOTOKIT_IMAGEINFODIALOG_H
#define PHOTOKIT_IMAGEINFODIALOG_H

#include "Dialog.h"

namespace PhotoKit {
class ImageInfoDialogPrivate;
class ImageInfoDialog : public Dialog
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(ImageInfoDialog)
public:
	explicit ImageInfoDialog(QGraphicsScene *scene, QGraphicsItem *parent = 0);
	~ImageInfoDialog();

	void setBaseImageInfo(const QString& text);
	void setExifInfo(const QString& text);
	void setGPSInfo(const QString& text);

	void showBaseInfo();
protected:
	//ImageInfoDialog(ImageInfoDialogPrivate& d, QGraphicsScene *scene, QGraphicsItem *parent = 0);
	
private slots:
	void showInfo();
};

} //namespace PhotoKit

#endif // PHOTOKIT_IMAGEINFODIALOG_H
