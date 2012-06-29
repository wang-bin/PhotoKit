/******************************************************************************
	SlideDisplay.h: description
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


#ifndef PHOTOKIT_SLIDEDISPLAY_H
#define PHOTOKIT_SLIDEDISPLAY_H

class NextEffect;
#include <QGraphicsItem>

namespace PhotoKit {
//TODO: flip background to show information
class SlideDisplay : public QGraphicsItem
{
public:
	enum ViewMode { SingleImage, SlideImage};
	SlideDisplay(QGraphicsItem *parent = 0);
	void setEffect(NextEffect* effect);
	QSize size();

	virtual QRectF boundingRect() const;

protected:
	//depends on mode
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event); //next pre
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event); //go back to ThumbPage
	virtual bool sceneEvent(QEvent *event); //touch

protected:
	ViewMode mMode;
	NextEffect *mSlideEffect;
	qreal mWidth, mHeight;
};

} //namespace PhotoKit
#endif // PHOTOKIT_SLIDEDISPLAY_H
