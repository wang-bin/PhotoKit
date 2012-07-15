/******************************************************************************
	ItemAnimation: animation for non-qobject items
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

#include "ItemAnimation.h"
#include <QGraphicsItem>
#include <QtCore/QTimeLine>
#include "TransformMachine.h"
namespace PhotoKit {

ItemAnimation::ItemAnimation(QGraphicsItem *item, QObject *parent) :
	QObject(parent),mHide(false),mFade(None)
{
	mMachine = new TransformMachine;//QGraphicsItemAnimation;
	QTimeLine *timer = new QTimeLine(1000);
	timer->setEasingCurve(QEasingCurve::OutQuad);
	timer->setFrameRange(0, 100);
	mMachine->setTimeLine(timer);
	connect(mMachine, SIGNAL(transformChanged(QTransform)), this, SLOT(setTransform(QTransform)));
	connect(mMachine, SIGNAL(zValueChanged(qreal)), this, SLOT(setZValue(qreal)));
	connect(mMachine, SIGNAL(posChanged(QPointF)), this, SLOT(setItemPos(QPointF)));
	connect(mMachine->timeLine(), SIGNAL(finished()), this, SLOT(tryHide()));
	connect(mMachine->timeLine(), SIGNAL(finished()), this, SIGNAL(finished()));

	setItem(item);
}

void ItemAnimation::setItem(QGraphicsItem *item)
{
	if (item) {
		mItem = item;
		mMachine->setStartPos(item->pos()); //
	}
}

void ItemAnimation::setDuration(int duration)
{
	mMachine->timeLine()->setDuration(duration);
}

void ItemAnimation::setTransform(const QTransform &m)
{
	mItem->setTransform(m);
	switch (mFade) {
	case FadeOut:
		mItem->setOpacity(qMax<qreal>(1.0 - currentStep(), 0));
		break;
	case FadeIn:
		mItem->setVisible(true);
		mItem->setOpacity(currentStep());
		break;
	case None:
		mItem->setOpacity(1.0);
		break;
	default:
		break;
	}
}

void ItemAnimation::setItemPos(const QPointF &pos)
{
	mItem->setPos(pos);
}

void ItemAnimation::setZValue(qreal z)
{
	mItem->setZValue(z);
}

void ItemAnimation::setAutoHide(bool hide)
{
	mHide = hide;
}

void ItemAnimation::setFade(Fade fade)
{
	mFade = fade;
	mHide = mFade == FadeOut;
}

TransformMachine* ItemAnimation::transformMachine()
{
	return mMachine;
}

qreal ItemAnimation::currentStep() const
{
	return mMachine->timeLine()->currentValue();
}

bool ItemAnimation::isRunning() const
{
	return mMachine->timeLine()->state() == QTimeLine::Running;
}

void ItemAnimation::start()
{
	mMachine->timeLine()->start();
}

void ItemAnimation::stop()
{
	mMachine->timeLine()->stop();
}

void ItemAnimation::tryHide()
{
	mItem->setVisible(!mHide || mFade == FadeOut);
}

} //namespace PhotoKit
