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
	QGraphicsObject(parent),mTextChanged(false),mRect(QRectF(0, 0, 256, 100))
{
	setFlag(QGraphicsItem::ItemIgnoresTransformations);
	setZValue(8);
}

QRectF ToolTip::boundingRect() const
{
	//if (mTextChanged)
	//	computeRect();
	return mRect;
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
	painter->setBrush(QColor(245, 245, 255, 220));
	painter->setClipRect(boundingRect());
	painter->drawRoundRect(3, -100 + 3, 400 - 6, 250 - 6);

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

void ToolTip::computeRect()
{/*
	{
		int sw = QApplication::desktop()->width() / 3;
		if ( sw < 200 )
			sw = 200;
		else if ( sw > 300 )
			sw = 300;

		r = fontMetrics().boundingRect( 0, 0, sw, 1000, Qt::AlignLeft + Qt::AlignTop + Qt::WordBreak + Qt::ExpandTabs, currentText );
	}

	int w = r.width() + 2*hMargin;
	int h = r.height() + 2*vMargin;

	// okay, now to find a suitable location

	int x;

	// first try locating the widget immediately above/below,
	// with nice alignment if possible.
	QPoint pos;
	if ( parentWidget() )
		pos = parentWidget()->mapToGlobal( QPoint( 0,0 ) );

	if ( parentWidget() && w > parentWidget()->width() + 16 )
		x = pos.x() + parentWidget()->width()/2 - w/2;
	else
		x = ppos.x() - w/2;

	// squeeze it in if that would result in part of what's this
	// being only partially visible
	if ( x + w > QApplication::desktop()->width() )
		x = (parentWidget()? (QMIN(QApplication::desktop()->width(),
				  pos.x() + parentWidget()->width())
			 ) : QApplication::desktop()->width() )
		- w;

	int sx = QApplication::desktop()->x();
	int sy = QApplication::desktop()->y();

	if ( x < sx ) x = sx;*/
}

} //namespace PhotoKit
