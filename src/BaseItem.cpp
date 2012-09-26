/******************************************************************************
	BaseItem: animation supported item. Many code are from qtdemo
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
#include <QGraphicsSceneMouseEvent>
#include "DemoItemAnimation.h"
#include "Guide.h"
#include "Config.h"
#include "ezlog.h"
namespace PhotoKit {


QHash<QString, SharedImage *> BaseItem::sharedImageHash;
QMatrix BaseItem::matrix;

BaseItem::BaseItem(QGraphicsItem *parent) : QGraphicsItem(parent),mScale(1.0),mX(0),mY(0)
{
    Config::noRescale = true; //TODO:
	opacity = 1.0;
	locked = false;
	prepared = false;
	neverVisible = false;
	noSubPixeling = false;
	currentAnimation = 0;
	currGuide = 0;
	guideFrame = 0;
	sharedImage = new SharedImage();
	++sharedImage->refCount;
}

BaseItem::~BaseItem()
{
	if(--sharedImage->refCount == 0){
		if (!hashKey.isEmpty())
			BaseItem::sharedImageHash.remove(hashKey);
		delete sharedImage;
    }
}

void BaseItem::setNeverVisible(bool never)
{
    Q_UNUSED(never);
/*
	neverVisible = never;
    if (never){
		setVisible(false);
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
	if (visible && neverVisible){
		setVisible(false);
        return;
    }

	setVisible(visible);
	QList<QGraphicsItem *> c = childItems();
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
	guideFrame = startFrame;
	while (guideFrame > guide->startLength + guide->length()){
        if (guide->nextGuide == guide->firstGuide)
            break;

        guide = guide->nextGuide;
    }
	currGuide = guide;
}

void BaseItem::guideAdvance(float distance)
{
	guideFrame += distance;
	while (guideFrame > currGuide->startLength + currGuide->length()){
		currGuide = currGuide->nextGuide;
		if (currGuide == currGuide->firstGuide)
			guideFrame -= currGuide->lengthAll();
    }
}

void BaseItem::guideMove(float moveSpeed)
{
	currGuide->guide(this, moveSpeed);
}

void BaseItem::setPosUsingSheepDog(const QPointF &dest, const QRectF &sceneFence)
{
	setPos(dest);
    if (sceneFence.isNull())
        return;

    // I agree. This is not the optimal way of doing it.
    // But don't want for use time on it now....
	float itemWidth = boundingRect().width();
	float itemHeight = boundingRect().height();
    float fenceRight = sceneFence.x() + sceneFence.width();
    float fenceBottom = sceneFence.y() + sceneFence.height();

	if (scenePos().x() < sceneFence.x()) moveBy(mapFromScene(QPointF(sceneFence.x(), 0)).x(), 0);
	if (scenePos().x() > fenceRight - itemWidth) moveBy(mapFromScene(QPointF(fenceRight - itemWidth, 0)).x(), 0);
	if (scenePos().y() < sceneFence.y()) moveBy(0, mapFromScene(QPointF(0, sceneFence.y())).y());
	if (scenePos().y() > fenceBottom - itemHeight) moveBy(0, mapFromScene(QPointF(0, fenceBottom - itemHeight)).y());
}

void BaseItem::setGuidedPos(const QPointF &pos)
{
	guidedPos = pos;
}

QPointF BaseItem::getGuidedPos()
{
	return guidedPos;
}

void BaseItem::switchGuide(Guide *guide)
{
	currGuide = guide;
	guideFrame = 0;
}

bool BaseItem::inTransition()
{
	if (currentAnimation)
		return currentAnimation->running();
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
		sharedImageHash.insert(hashKey, sharedImage);
    else {
		if(--sharedImage->refCount == 0)
			delete sharedImage;
		sharedImage = sharedImageHash.value(hashKey);
		++sharedImage->refCount;
    }
}

bool BaseItem::validateImage()
{
	if ((sharedImage->matrix != BaseItem::matrix && !Config::noRescale) || !(sharedImage->image || sharedImage->pixmap)){
        // (Re)create image according to new matrix
		delete sharedImage->image;
		sharedImage->image = 0;
		delete sharedImage->pixmap;
		sharedImage->pixmap = 0;
		sharedImage->matrix = BaseItem::matrix;

        // Let subclass create and draw a new image according to the new matrix
		//TODO: QImage *image = createImage(Config::noRescale ? QMatrix() : BaseItem::matrix);
		QImage *image = createImage(BaseItem::matrix);
        if (image){
           /* if (Config::showBoundingRect){
                // draw red transparent rect
                QPainter painter(image);
                painter.fillRect(image->rect(), QColor(255, 0, 0, 50));
                painter.end();
            }*/

			sharedImage->unscaledBoundingRect = sharedImage->matrix.inverted().mapRect(image->rect());
            if (Config::usePixmaps){
                if (image->isNull())
					sharedImage->pixmap = new QPixmap(1, 1);
                else
					sharedImage->pixmap = new QPixmap(image->size());
				sharedImage->pixmap->fill(QColor(0, 0, 0, 0));
				QPainter painter(sharedImage->pixmap);
                painter.drawImage(0, 0, *image);
                delete image;
            } else {
				sharedImage->image = image;
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
	return sharedImage->unscaledBoundingRect;
}


void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

	if (validateImage()){

        bool wasSmoothPixmapTransform = painter->testRenderHint(QPainter::SmoothPixmapTransform);
        painter->setRenderHint(QPainter::SmoothPixmapTransform);

        if (Config::noRescale){
            // Let the painter scale the image for us.
            // This may degrade both quality and performance
			if (sharedImage->image)
				painter->drawImage(boundingRect(), *sharedImage->image);
            else
				painter->drawPixmap(boundingRect().toRect(), *sharedImage->pixmap);
        }
        else {
            QMatrix m = painter->worldMatrix();
            painter->setWorldMatrix(QMatrix());
			float x = noSubPixeling ? qRound(m.dx()) : m.dx();
			float y = noSubPixeling ? qRound(m.dy()) : m.dy();
			if (sharedImage->image)
				painter->drawImage(QPointF(x, y), *sharedImage->image);
            else
				painter->drawPixmap(QPointF(x, y), *sharedImage->pixmap);
        }

        if (!wasSmoothPixmapTransform) {
            painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
        }

    }
}


} //namespace PhotoKit
