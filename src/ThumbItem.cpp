/******************************************************************************
	ThumbItem.cpp: description
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


#include "ThumbItem.h"
#include <QPainter>

#include "OutlineGlowItem.h"
#include "Config.h"

BEGIN_NAMESPACE_PHOTOKIT

ThumbItem::ThumbItem(QGraphicsItem *parent) :
	QGraphicsItem(parent),mGlow(0)
{
	setAcceptHoverEvents(true); //default: false
	thumb = QImage(Config::thumbItemWidth, Config::thumbItemHeight, QImage::Format_ARGB32_Premultiplied);
}

ThumbItem::ThumbItem(const QImage& image, QGraphicsItem *parent) :
	QGraphicsItem(parent),thumb(image),mGlow(0)
{
	setAcceptHoverEvents(true); //default: false
}


void ThumbItem::setThumbImage(const QImage& image)
{
	thumb = image;
	update(boundingRect());
}

QRectF ThumbItem::boundingRect() const
{
	return thumb.rect().adjusted(-(Config::thumbMargin + Config::thumbBorder) , -(Config::thumbBorder + Config::thumbMargin)
					, Config::thumbBorder + Config::thumbMargin, Config::thumbBorder + Config::thumbMargin);
}

void ThumbItem::showGlow()
{
	if (!mGlow) {
		qDebug("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
		mGlow = new OutlineGlowItem(this);
		QSizeF s = boundingRect().size();
		mGlow->setSize(QSize((int)s.width(), (int)s.height()));
		mGlow->setGlowWidth(Config::thumbMargin + Config::thumbBorder);
		QPainterPath image_shape;
		image_shape.addRect(-Config::thumbBorder, -Config::thumbBorder
					, thumb.width() + 2*Config::thumbBorder, thumb.height() + 2*Config::thumbBorder);
		mGlow->setShape(image_shape);
		mGlow->render();
	}
	mGlow->setVisible(true);
}

void ThumbItem::hideGlow()
{
	mGlow->setVisible(false);
}

void ThumbItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	//bool wasSmoothPixmapTransform = painter->testRenderHint(QPainter::SmoothPixmapTransform);
	painter->save();
	painter->setPen(Qt::black);
	//painter->drawRect(Config::thumbMargin, Config::thumbMargin, thumb.width() + 2*Config::thumbBorder, thumb.height() + 2*Config::thumbBorder);
	painter->drawRect(-Config::thumbBorder, -Config::thumbBorder, thumb.width() + 2*Config::thumbBorder, thumb.height() + 2*Config::thumbBorder);
	painter->restore();
	//painter->drawImage(Config::thumbBorder + Config::thumbMargin, Config::thumbBorder + Config::thumbMargin, thumb);
	painter->drawImage(0, 0, thumb);
}

void ThumbItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	showGlow();
}

void ThumbItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	hideGlow();
}

void ThumbItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void ThumbItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}

END_NAMESPACE_PHOTOKIT
