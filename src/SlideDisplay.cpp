/******************************************************************************
	SlideDisplay.cpp: description
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


#include "SlideDisplay.h"
#include <QApplication>
#include <QDesktopWidget>

#include "nexteffect.h"

namespace PhotoKit {

SlideDisplay::SlideDisplay(QGraphicsItem *parent)
	:QGraphicsItem(parent),mMode(SingleImage)
{
	mSlideEffect = 0;
	mWidth = (qreal)qApp->desktop()->width();
	mHeight = (qreal)qApp->desktop()->height();
}

void SlideDisplay::setEffect(NextEffect *effect)
{
	mSlideEffect = effect;
}

QSize SlideDisplay::size()
{
	return QSize(mWidth, mHeight);
}

QRectF SlideDisplay::boundingRect() const
{
	return QRectF(0, 0, mWidth, mHeight);
}

void SlideDisplay::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (mMode == SingleImage) {

	}
	QGraphicsItem::mousePressEvent(event);
}

void SlideDisplay::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
}

void SlideDisplay::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (mMode == SingleImage) {
		//translate the item. ensureVisible; //2 finger to next/pre
		//
	}
	QGraphicsItem::mouseMoveEvent(event);
}

void SlideDisplay::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	//go back to ThumbPage
	QGraphicsItem::mouseDoubleClickEvent(event);
}

bool SlideDisplay::sceneEvent(QEvent *event)
{
	QGraphicsItem::sceneEvent(event);
}

}

