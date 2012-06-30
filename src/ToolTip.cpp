/******************************************************************************
	ToolTip.cpp: description
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


#include "ToolTip.h"
#include <QtGui/QPainter>

namespace PhotoKit {

ToolTip::ToolTip(QGraphicsItem *parent) :
    QGraphicsWidget(parent),mTextChanged(false),mRect(QRectF(0, 0, 256, 100))
{
	setFlag(QGraphicsItem::ItemIgnoresTransformations);
	setZValue(8);
}

void ToolTip::showText(const QString &text)
{
	mText = text;
	update();
}

void ToolTip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(QColor(245, 245, 255, 168));
	painter->setClipRect(boundingRect());
    painter->drawRoundedRect(boundingRect(), 10, 90, Qt::RelativeSize);

	QRectF textRect = boundingRect().adjusted(10, 10, -10, -10);
	int flags = Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap;

	if (mText.isEmpty())
		return;
	QFont font;
	font.setPixelSize(18);
	painter->setPen(Qt::black);
	painter->setFont(font);
	painter->drawText(textRect, flags, mText);
}


} //namespace PhotoKit
