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


#include "OutlineGlowItem.h"

#include <QGraphicsBlurEffect>
#include <QPainter>
#include "Config.h"

namespace PhotoKit {

OutlineGlowItem::OutlineGlowItem(QGraphicsItem *parent)
	:QGraphicsPixmapItem(parent),mGlowWidth(7),mColor(Config::glowColor)
{
	setCacheMode(QGraphicsItem::ItemCoordinateCache); //item.scroll enabled(not for gl). speed up
	setFlag(QGraphicsItem::ItemStacksBehindParent);
	QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
	blur->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
    blur->setBlurRadius(22);
	setGraphicsEffect(blur);
}

QColor OutlineGlowItem::color() const
{
	return mColor;
}

qreal OutlineGlowItem::glowWidth() const
{
	return mGlowWidth;
}

void OutlineGlowItem::render()
{
	QPainterPathStroker pps;
	pps.setCapStyle(Qt::RoundCap);
	pps.setJoinStyle(Qt::RoundJoin);
	pps.setWidth(mGlowWidth);
	mOutline = pps.createStroke(mShape).simplified();//.united(mShape).simplified();
	//QImage image(mSize, QImage::Format_ARGB32_Premultiplied);
	mPixmap = QPixmap(mSize);
	mPixmap.fill(Qt::transparent);
	//QPainter painter(&image);//&mPixmap);
	QPainter painter(&mPixmap);
	QBrush brush(mColor);
	painter.drawPath(mOutline);
	painter.fillPath(mOutline, brush);
	//painter.setPen(mColor.lighter(123));
	//painter.drawPath(mOutline);
	setPixmap(mPixmap);
	//setPixmap(QPixmap::fromImage(image));
}

void OutlineGlowItem::setColor(const QColor& color)
{
	mColor = color;
}

void OutlineGlowItem::setGlowWidth(qreal glowWidth)
{
	mGlowWidth = glowWidth;
}

void OutlineGlowItem::setShape(const QPainterPath& shape)
{
	mShape = shape;
}

void OutlineGlowItem::setSize(const QSize& size)
{
	mSize = size;
}


} //namespace PhotoKit
