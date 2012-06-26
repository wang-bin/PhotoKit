#include "ReflectEffectItem.h"

#include <QPainter>

#include "ThumbItem.h"
namespace PhotoKit {

ReflectEffectItem::ReflectEffectItem(ThumbItem* target, MirrorDirection direction)
    :QGraphicsItem(target),mBlur(false),mGradient(true),mTarget(target),mDirection(direction),mMirrorDistance(2),mReflect(0)
{
    setFlag(QGraphicsItem::ItemStacksBehindParent);
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
    if (!mReflect) {
        drawReflect();
    }
    painter->drawImage(mPos, *mReflect);
}

void ReflectEffectItem::drawReflect()
{
    if (mReflect) {
        delete mReflect;
        mReflect = 0;
    }
    mReflect = new QImage(mTarget->thumbImage());
    QLinearGradient g(QPointF(), QPointF(0, mReflect->height()));
    if (mDirection == MirrorBottom) {
        mReflect = new QImage(mTarget->thumbImage().mirrored(false, true));
        g.setColorAt(0, QColor(0, 0, 0, 111));
        g.setColorAt(0.618, QColor(0, 0, 0, 222));
        g.setColorAt(1, Qt::black);
        mPos.setX(mTarget->marginWidth() + mTarget->borderWidth());
        mPos.setY(mTarget->boundingHeight() + mTarget->marginWidth() + mTarget->borderWidth() + 2*mMirrorDistance);
    } else if (mDirection == MirrorTop) {
        mReflect = new QImage(mTarget->thumbImage().mirrored(false, true));
        g.setColorAt(0, Qt::black);
        g.setColorAt(1, Qt::transparent);
        mPos.setX(mTarget->marginWidth() + mTarget->borderWidth());
        mPos.setY(- mTarget->boundingHeight() + mTarget->marginWidth() + mTarget->borderWidth() - 2*mMirrorDistance);
    } else if (mDirection == MirrorLeft) {
        mReflect = new QImage(mTarget->thumbImage().mirrored(true, false));
        g.setColorAt(0, Qt::black);
        g.setColorAt(1, Qt::transparent);
        mPos.setX(-mTarget->boundingWidth() - 2*mMirrorDistance + mTarget->marginWidth() + mTarget->borderWidth());
        mPos.setY(mTarget->marginWidth() + mTarget->borderWidth());
    } else {
        mReflect = new QImage(mTarget->thumbImage().mirrored(true, false));
        g.setColorAt(1, Qt::black);
        g.setColorAt(0, Qt::transparent);
        mPos.setX(mTarget->boundingWidth() + 2*mMirrorDistance + mTarget->marginWidth() + mTarget->borderWidth());
        mPos.setY(mTarget->marginWidth() + mTarget->borderWidth());
    }
    QPainter painter(mReflect);
    painter.fillRect(0, 0, mReflect->width(), mReflect->height(), g);
}

} //namespace PhotoKit

