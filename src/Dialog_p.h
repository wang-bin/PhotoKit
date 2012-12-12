/******************************************************************************
	DialogPrivate: base class for the private class of Dialog's subclasses
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


#ifndef PHOTOKI_DIALOG_P_H
#define PHOTOKI_DIALOG_P_H

#include <QtCore/QObject>
#include <QtCore/QEventLoop>
#include <QtGui/QColor>
class QSizeF;
class QGraphicsWidget;
class QGraphicsScene;
class QEventLoop;
class QTimeLine;
namespace PhotoKit {
class Dialog;
class ItemAnimation;
class Button;
class DialogPrivate
{
	Q_DECLARE_PUBLIC(Dialog)
public:
	DialogPrivate();
	QSizeF size() const;

	void setupUi(Dialog* ui);

	virtual ~DialogPrivate();

	QGraphicsWidget *titleBar;
	QGraphicsWidget *central;
	QGraphicsWidget *buttonBar;
	//QGraphicsTextItem *text;
	QEventLoop loop;
	QGraphicsScene *scene;
	ItemAnimation *animation;
	QTimeLine *timeline; //for 3d animation
	qreal width; //FIXME: width after resize is not the value i resized, why?
	int result;
	QColor color;
protected:
	Dialog *q_ptr;
};


} //namespace PhotoKit
#endif // PHOTOKI_DIALOG_P_H
