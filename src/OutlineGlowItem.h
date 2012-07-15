/******************************************************************************
	OutlineGlowItem: blured outline for items
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


#ifndef PHOTOKIT_OUTLINEGLOWITEM_H
#define PHOTOKIT_OUTLINEGLOWITEM_H

#include "PhotoKit_Global.h"

#include <QGraphicsPixmapItem>

namespace PhotoKit {
//TODO: shapeitem
class OutlineGlowItem : public QGraphicsPixmapItem
{
public:
	OutlineGlowItem(QGraphicsItem * parent = 0);
	qreal glowWidth() const;
	QColor color() const;
	void render();
	void setGlowWidth(qreal glowWidth);
	void setColor(const QColor& color);
	void setShape(const QPainterPath& shape);
	void setSize(const QSize& size);

private:
	qreal mGlowWidth;
	QColor mColor;
	QPainterPath mOutline, mShape;
	QSize mSize;
	QPixmap mPixmap;
};

} //namespace PhotoKit
#endif // PHOTOKIT_OUTLINEGLOWITEM_H
