/******************************************************************************
	ItemAnimation.cpp: description
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

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "ItemAnimation.h"

namespace PhotoKit {

ItemAnimation::ItemAnimation(QGraphicsItem *item, QObject *parent) :
	QObject(parent),mItem(item)
{
}

void ItemAnimation::setTransform(const QTransform &m)
{
	mItem->setTransform(m);
}

void ItemAnimation::setZValue(qreal z)
{
	mItem->setZValue(z);
}

} //namespace PhotoKit
