/******************************************************************************
	OutlineGlowItem.cpp: description
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

BEGIN_NAMESPACE_PHOTOKIT

OutlineGlowItem::OutlineGlowItem(QGraphicsItem *parent)
    :QGraphicsPixmapItem(parent),mGlowWidth(7),mColor(QColor(Qt::blue).lighter(123))
{
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
	qDebug("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
	QPainterPathStroker pps;
	pps.setCapStyle(Qt::RoundCap);
	pps.setJoinStyle(Qt::RoundJoin);
	pps.setWidth(mGlowWidth);
	mOutline = pps.createStroke(mShape).simplified();//.united(mShape).simplified();
	mPixmap = QPixmap(mSize);
	mPixmap.fill(Qt::transparent);
	QPainter painter(&mPixmap);
	QBrush brush(mColor);
	painter.fillPath(mOutline, brush);
	//painter.setPen(mColor.lighter(123));
	//painter.drawPath(mOutline);
	setPixmap(mPixmap);
}

void OutlineGlowItem::setColor(const QColor& color)
{
	mColor = color;
	//mColor.setAlpha(88);
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


END_NAMESPACE_PHOTOKIT
