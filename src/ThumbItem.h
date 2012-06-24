/******************************************************************************
	ThumbItem.h: description
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


#ifndef THUMBITEM_H
#define THUMBITEM_H

#include "PhotoKit_Global.h"
#include <QGraphicsItem>

BEGIN_NAMESPACE_PHOTOKIT

class OutlineGlowItem;
class ThumbItem : public QGraphicsItem
{
public:
	explicit ThumbItem(QGraphicsItem *parent = 0);
	explicit ThumbItem(const QImage& image, QGraphicsItem *parent = 0);
	void setThumbImage(const QImage& image);
	virtual QRectF boundingRect() const; // overridden
	void showGlow();
	void hideGlow();

protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option = 0, QWidget *widget = 0);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	//virtual QImage *createImage(const QMatrix &) const { return 0; }
	//virtual bool collidesWithItem(const QGraphicsItem *, Qt::ItemSelectionMode) const { return false; }

private:
	QString origin_image_path;
	QImage thumb;
	OutlineGlowItem *mGlow;
};

END_NAMESPACE_PHOTOKIT
#endif // THUMBITEM_H
