/******************************************************************************
	ReflectEffectItem: reflection effect for a ThumbItem
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

#include "ReflectEffectItem.h"

#include <QGraphicsBlurEffect>
#include <QPainter>

#include "ThumbItem.h"
namespace PhotoKit {

ReflectEffectItem::ReflectEffectItem(ThumbItem* target, MirrorDirection direction)
	:QGraphicsObject(target),mBlur(true),mGradient(true),mBlurEffect(0),mTarget(target),mDirection(direction),mMirrorDistance(2),mReflect(0)
{
	mSourceAvailable = !mTarget->isOnlineImage();
	setCacheMode(QGraphicsItem::ItemCoordinateCache); //item.scroll enabled(not for gl). speed up
	setFlag(QGraphicsItem::ItemStacksBehindParent);

	connect(mTarget, SIGNAL(loadFinished()), this, SLOT(updateSourceReflect()));
}

ReflectEffectItem::~ReflectEffectItem()
{
    if (mReflect) {
        delete mReflect;
        mReflect = 0;
    }
}

int ReflectEffectItem::mirrorDistance() const
{
    return mMirrorDistance;
}

void ReflectEffectItem::setMirrorDistance(int distance)
{
    mMirrorDistance = distance;
}

ReflectEffectItem::MirrorDirection ReflectEffectItem::mirrorDirection() const
{
    return mDirection;
}

void ReflectEffectItem::setMirrorDirection(MirrorDirection direction)
{
    mDirection = direction;
}

bool ReflectEffectItem::blurEnabled() const
{
    return mBlur;
}

void ReflectEffectItem::enableBlur(bool enable)
{
    mBlur = enable;
}

bool ReflectEffectItem::gradientEnabled() const
{
    return mGradient;
}

void ReflectEffectItem::enableGradient(bool enable)
{
    mGradient = enable;
}

QRectF ReflectEffectItem::boundingRect() const
{
    if (mDirection == MirrorBottom || mDirection == MirrorTop)
        return QRectF(0, 0, mTarget->boundingRect().width(), 2*mTarget->boundingRect().height() + 2*mMirrorDistance);
    else
        return QRectF(0, 0, 2*mTarget->boundingRect().width() + 2*mMirrorDistance, mTarget->boundingRect().height());
}

void ReflectEffectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!mReflect && mSourceAvailable) {
        drawReflect();
    }
	if (mReflect)
		painter->drawImage(mPos, *mReflect);
}

void ReflectEffectItem::drawReflect()
{
    if (mReflect) {
        delete mReflect;
        mReflect = 0;
    }
	QLinearGradient g(QPointF(), QPointF(0, mTarget->contentHeight()));
    if (mDirection == MirrorBottom) {
		mReflect = new QImage(mTarget->scaledThumbImage().mirrored(false, true));
        g.setColorAt(0, QColor(0, 0, 0, 100));
        g.setColorAt(1, Qt::transparent);
        mPos.setX(mTarget->marginWidth() + mTarget->borderWidth());
        mPos.setY(mTarget->boundingHeight() + mTarget->marginWidth() + mTarget->borderWidth() + 2*mMirrorDistance);
    } else if (mDirection == MirrorTop) {
		mReflect = new QImage(mTarget->scaledThumbImage().mirrored(false, true));
        g.setColorAt(1, QColor(0, 0, 0, 100));
        g.setColorAt(0, Qt::transparent);
        mPos.setX(mTarget->marginWidth() + mTarget->borderWidth());
        mPos.setY(- mTarget->boundingHeight() + mTarget->marginWidth() + mTarget->borderWidth() - 2*mMirrorDistance);
    } else if (mDirection == MirrorLeft) {
		mReflect = new QImage(mTarget->scaledThumbImage().mirrored(true, false));
		g.setFinalStop(QPointF(mReflect->width(), 0));
        g.setColorAt(1, QColor(0, 0, 0, 100));
        g.setColorAt(0, Qt::transparent);
        mPos.setX(-mTarget->boundingWidth() - 2*mMirrorDistance + mTarget->marginWidth() + mTarget->borderWidth());
        mPos.setY(mTarget->marginWidth() + mTarget->borderWidth());
    } else {
		mReflect = new QImage(mTarget->scaledThumbImage().mirrored(true, false));
		g.setFinalStop(QPointF(mReflect->width(), 0));
        g.setColorAt(1, QColor(0, 0, 0, 100));
        g.setColorAt(1, Qt::transparent);
        mPos.setX(mTarget->boundingWidth() + 2*mMirrorDistance + mTarget->marginWidth() + mTarget->borderWidth());
        mPos.setY(mTarget->marginWidth() + mTarget->borderWidth());
    }
	if (mReflect->format() != QImage::Format_ARGB32_Premultiplied)
		*mReflect = mReflect->convertToFormat(QImage::Format_ARGB32_Premultiplied); //fast scaled image does not have alpha?
    QPainter painter(mReflect);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(0, 0, mReflect->width(), mReflect->height(), g);

    if (mBlur) {
        if (mBlurEffect)
            return;
        mBlurEffect = new QGraphicsBlurEffect;
        mBlurEffect->setBlurRadius(1.3);
        mBlurEffect->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
        setGraphicsEffect(mBlurEffect);
    } //TODO: how to remove effect? can not delete
}

void ReflectEffectItem::updateSourceReflect()
{
	drawReflect();
	mSourceAvailable = true;
	update();
}

} //namespace PhotoKit

