/******************************************************************************
    BaseItem.cpp: description
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


#include "BaseItem.h"
#include <QPainter>
#include "DemoItemAnimation.h"
#include "Guide.h"
#include "Config.h"
#include "ezlog.h"
namespace PhotoKit {


QHash<QString, SharedImage *> BaseItem::sharedImageHash;
QMatrix BaseItem::matrix;

BaseItem::BaseItem(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    Config::noRescale = true; //TODO:
    this->opacity = 1.0;
    this->locked = false;
    this->prepared = false;
    this->neverVisible = false;
    this->noSubPixeling = false;
    this->currentAnimation = 0;
    this->currGuide = 0;
    this->guideFrame = 0;
    this->sharedImage = new SharedImage();
    ++this->sharedImage->refCount;
}

BaseItem::~BaseItem()
{
    if(--this->sharedImage->refCount == 0){
        if (!this->hashKey.isEmpty())
            BaseItem::sharedImageHash.remove(this->hashKey);
        delete this->sharedImage;
    }
}

void BaseItem::setNeverVisible(bool never)
{
    Q_UNUSED(never);
/*
    this->neverVisible = never;
    if (never){
        this->setVisible(false);
        QList<QGraphicsItem *> c = children();
        for (int i=0; i<c.size(); i++){
            BaseItem *d = dynamic_cast<BaseItem *>(c[i]); // Don't use dynamic cast because it needs RTTI support.
            if (d)
                d->setNeverVisible(true);
            else{
                c[i]->setVisible(false);
            }
        }
    }
*/
}

void BaseItem::setRecursiveVisible(bool visible){
	if (visible && this->neverVisible){
        this->setVisible(false);
        return;
    }

    this->setVisible(visible);
    QList<QGraphicsItem *> c = children();
    for (int i=0; i<c.size(); i++){
        // BaseItem *d = dynamic_cast<BaseItem *>(c[i]);
        // if (d)
        //     d->setRecursiveVisible(visible);
        // else{
            c[i]->setVisible(visible);
        // }
    }
}

void BaseItem::useGuide(Guide *guide, float startFrame)
{
    this->startFrame = startFrame;
    this->guideFrame = startFrame;
    while (this->guideFrame > guide->startLength + guide->length()){
        if (guide->nextGuide == guide->firstGuide)
            break;

        guide = guide->nextGuide;
    }
    this->currGuide = guide;
}

void BaseItem::guideAdvance(float distance)
{
    this->guideFrame += distance;
    while (this->guideFrame > this->currGuide->startLength + this->currGuide->length()){
        this->currGuide = this->currGuide->nextGuide;
        if (this->currGuide == this->currGuide->firstGuide)
            this->guideFrame -= this->currGuide->lengthAll();
    }
}

void BaseItem::guideMove(float moveSpeed)
{
    this->currGuide->guide(this, moveSpeed);
}

void BaseItem::setPosUsingSheepDog(const QPointF &dest, const QRectF &sceneFence)
{
    this->setPos(dest);
    if (sceneFence.isNull())
        return;

    // I agree. This is not the optimal way of doing it.
    // But don't want for use time on it now....
    float itemWidth = this->boundingRect().width();
    float itemHeight = this->boundingRect().height();
    float fenceRight = sceneFence.x() + sceneFence.width();
    float fenceBottom = sceneFence.y() + sceneFence.height();

    if (this->scenePos().x() < sceneFence.x()) this->moveBy(this->mapFromScene(QPointF(sceneFence.x(), 0)).x(), 0);
    if (this->scenePos().x() > fenceRight - itemWidth) this->moveBy(this->mapFromScene(QPointF(fenceRight - itemWidth, 0)).x(), 0);
    if (this->scenePos().y() < sceneFence.y()) this->moveBy(0, this->mapFromScene(QPointF(0, sceneFence.y())).y());
    if (this->scenePos().y() > fenceBottom - itemHeight) this->moveBy(0, this->mapFromScene(QPointF(0, fenceBottom - itemHeight)).y());
}

void BaseItem::setGuidedPos(const QPointF &pos)
{
    this->guidedPos = pos;
}

QPointF BaseItem::getGuidedPos()
{
    return this->guidedPos;
}

void BaseItem::switchGuide(Guide *guide)
{
    this->currGuide = guide;
    this->guideFrame = 0;
}

bool BaseItem::inTransition()
{
    if (this->currentAnimation)
        return this->currentAnimation->running();
    else
        return false;
}

void BaseItem::setMatrix(const QMatrix &matrix)
{
    BaseItem::matrix = matrix;
}

void BaseItem::useSharedImage(const QString &hashKey)
{
    this->hashKey = hashKey;
    if (!sharedImageHash.contains(hashKey))
        sharedImageHash.insert(hashKey, this->sharedImage);
    else {
        if(--this->sharedImage->refCount == 0)
            delete this->sharedImage;
        this->sharedImage = sharedImageHash.value(hashKey);
        ++this->sharedImage->refCount;
    }
}

bool BaseItem::validateImage()
{
    if ((this->sharedImage->matrix != BaseItem::matrix && !Config::noRescale) || !(this->sharedImage->image || this->sharedImage->pixmap)){
        // (Re)create image according to new matrix
        delete this->sharedImage->image;
        this->sharedImage->image = 0;
        delete this->sharedImage->pixmap;
        this->sharedImage->pixmap = 0;
        this->sharedImage->matrix = BaseItem::matrix;

        // Let subclass create and draw a new image according to the new matrix
        //TODO: QImage *image = this->createImage(Config::noRescale ? QMatrix() : BaseItem::matrix);
        QImage *image = this->createImage(BaseItem::matrix);
        if (image){
           /* if (Config::showBoundingRect){
                // draw red transparent rect
                QPainter painter(image);
                painter.fillRect(image->rect(), QColor(255, 0, 0, 50));
                painter.end();
            }*/

            this->sharedImage->unscaledBoundingRect = this->sharedImage->matrix.inverted().mapRect(image->rect());
            if (Config::usePixmaps){
                if (image->isNull())
                    this->sharedImage->pixmap = new QPixmap(1, 1);
                else
                    this->sharedImage->pixmap = new QPixmap(image->size());
                this->sharedImage->pixmap->fill(QColor(0, 0, 0, 0));
                QPainter painter(this->sharedImage->pixmap);
                painter.drawImage(0, 0, *image);
                delete image;
            } else {
                this->sharedImage->image = image;
            }
            return true;
        } else
            return false;
    }
    return true;
}

QRectF BaseItem::boundingRect() const
{
    const_cast<BaseItem *>(this)->validateImage();
    return this->sharedImage->unscaledBoundingRect;
}


void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (this->validateImage()){

        bool wasSmoothPixmapTransform = painter->testRenderHint(QPainter::SmoothPixmapTransform);
        painter->setRenderHint(QPainter::SmoothPixmapTransform);

        if (Config::noRescale){
            // Let the painter scale the image for us.
            // This may degrade both quality and performance
            if (this->sharedImage->image)
                painter->drawImage(boundingRect(), *this->sharedImage->image);
            else
                painter->drawPixmap(boundingRect().toRect(), *this->sharedImage->pixmap);
        }
        else {
            QMatrix m = painter->worldMatrix();
            painter->setWorldMatrix(QMatrix());
            float x = this->noSubPixeling ? qRound(m.dx()) : m.dx();
            float y = this->noSubPixeling ? qRound(m.dy()) : m.dy();
            if (this->sharedImage->image)
                painter->drawImage(QPointF(x, y), *this->sharedImage->image);
            else
                painter->drawPixmap(QPointF(x, y), *this->sharedImage->pixmap);
        }

        if (!wasSmoothPixmapTransform) {
            painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
        }

    }
}

} //namespace PhotoKit
