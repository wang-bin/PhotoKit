/******************************************************************************
	ThumbItem.cpp: description
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


#include "ThumbItem.h"
#include <QtCore/QEvent>
#include <QtCore/QTimeLine>
#include <QTouchEvent>
#include <QGraphicsItemAnimation>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include "ItemAnimation.h"
#include "OutlineGlowItem.h"
#include "TransformMachine.h"
#include "UiManager.h"
#include "Config.h"
#include "ezlog.h"
namespace PhotoKit {

//TODO: calculate to fit screen
static const qreal zoom_max = 2.2;

ThumbItem::ThumbItem(QGraphicsItem *parent) :
	QGraphicsItem(parent),mGlow(0),mItemAnimation(0)
  ,mWidth(Config::thumbItemHeight - 2*(Config::thumbBorder + Config::thumbMargin))
  ,mHeight(Config::thumbItemHeight - 2*(Config::thumbBorder + Config::thumbMargin))
{
	//setAcceptTouchEvents(true);
	setAcceptHoverEvents(true); //default: false
	setCacheMode(QGraphicsItem::ItemCoordinateCache); //item.scroll enabled(not for gl). speed up
	thumb = QImage(Config::thumbItemWidth, Config::thumbItemHeight, QImage::Format_ARGB32_Premultiplied);
}

ThumbItem::ThumbItem(const QImage& image, QGraphicsItem *parent) :
	QGraphicsItem(parent),thumb(image),mGlow(0),mItemAnimation(0)
  ,mWidth(Config::thumbItemWidth - 2*(Config::thumbBorder + Config::thumbMargin))
  ,mHeight(Config::thumbItemHeight - 2*(Config::thumbBorder + Config::thumbMargin))
{
	setAcceptHoverEvents(true); //default: false
	prepair();
}

ThumbItem::~ThumbItem()
{
}

void ThumbItem::setOriginImage(const QString& path)
{
    origin_image_path = path;
}

qreal ThumbItem::borderWidth() const
{
    return Config::thumbBorder;
}

qreal ThumbItem::marginWidth() const
{
    return Config::thumbMargin;
}

QImage ThumbItem::thumbImage() const
{
    return thumb;
}

QImage ThumbItem::scaledThumbImage() const
{
	if (mWidth != thumb.width() || mHeight != thumb.height())
		return thumb.scaled(mWidth, mHeight);
	return thumb;
}

void ThumbItem::setThumbImage(const QImage& image)
{
	thumb = image;
	prepair();
	update(boundingRect());
}
//add mirror rect?
QRectF ThumbItem::boundingRect() const
{
	return QRectF(0, 0, mWidth, mHeight).adjusted(0, 0//thumb.rect().adjusted(0, 0//-(Config::thumbMargin + Config::thumbBorder) , -(Config::thumbBorder + Config::thumbMargin)
					, 2*(Config::thumbBorder + Config::thumbMargin), 2*(Config::thumbBorder + Config::thumbMargin));
}

qreal ThumbItem::boundingWidth() const
{
	return mWidth + 2*(Config::thumbBorder + Config::thumbMargin);
}

qreal ThumbItem::boundingHeight() const
{
	return mHeight + 2*(Config::thumbBorder + Config::thumbMargin);
}

qreal ThumbItem::contentWidth() const
{
	return mWidth;
}

qreal ThumbItem::contentHeight() const
{
	return mHeight;
}

void ThumbItem::showGlow()
{
	if (!mGlow) {
		ezlog_debug();
		mGlow = new OutlineGlowItem(this);
		//mGlow->setZValue(zValue() + 1);
		QSizeF s = boundingRect().size();
		mGlow->setSize(QSize((int)s.width(), (int)s.height()));
		mGlow->setGlowWidth(Config::thumbMargin);// + Config::thumbBorder);
		QPainterPath image_shape;
		image_shape.addRect(Config::thumbMargin, Config::thumbMargin//-Config::thumbBorder, -Config::thumbBorder
					, mWidth + 2*Config::thumbBorder, mHeight + 2*Config::thumbBorder);
		mGlow->setShape(image_shape);
		mGlow->render();
	}
	mGlow->setVisible(true);
}

void ThumbItem::hideGlow()
{
	if (mGlow)
		mGlow->setVisible(false);
}
//TODO:matrix
void ThumbItem::zoom(ZoomAction action)
{
	if (!mItemAnimation) {
		mItemAnimation = new ItemAnimation(this);
	}
	qreal vs = 1.0;
	qreal hs = 1.0;
	qreal tx = 0;
	qreal ty = 0;
	qreal z = 0;
	/*!
		translate then zoom. so if keep the center, the translation t satisfies
		t_dst * s = 0.5 * (w - w0, h - h0) = 0.5 * (s - 1) * (w0, h0)
	*/
	static qreal tx_dst = -boundingWidth()*0.5*(zoom_max - 1.0)/zoom_max;
	static qreal ty_dst = -boundingHeight()*0.5*(zoom_max - 1.0)/zoom_max;
	if (mItemAnimation->isRunning()) {
		mItemAnimation->stop();
		qreal step = mItemAnimation->currentStep();
		vs = mItemAnimation->transformMachine()->verticalScaleAt(step);
		hs = mItemAnimation->transformMachine()->horizontalScaleAt(step);
		tx = mItemAnimation->transformMachine()->xTranslationAt(step);
		ty = mItemAnimation->transformMachine()->yTranslationAt(step);
		z = mItemAnimation->transformMachine()->zValueAt(step);
	} else {
		if (action == ZoomOut) {
			vs = zoom_max;
			hs = zoom_max;
			tx = tx_dst;
			ty = ty_dst;
			z = 2.0;
		}
	}
	//mMachine->setStartMatrix(matrix());
	//setTransformOriginPoint(transform().mapRect(boundingRect()).center());
	if (action == ZoomIn) {
        showGlow();
		mItemAnimation->transformMachine()->setZValueAt(0.0, z);
		mItemAnimation->transformMachine()->setZValueAt(1.0, 2.0);
		mItemAnimation->transformMachine()->setTranslationAt(0.0, tx, ty);
		mItemAnimation->transformMachine()->setTranslationAt(1.0, tx_dst, ty_dst);
		mItemAnimation->transformMachine()->setScaleAt(0.0, hs, vs);
		mItemAnimation->transformMachine()->setScaleAt(1.0, zoom_max, zoom_max);
	} else {
		mItemAnimation->transformMachine()->setZValueAt(0.0, z);
		mItemAnimation->transformMachine()->setZValueAt(1.0, 0);
		mItemAnimation->transformMachine()->setTranslationAt(0.0, tx, ty);
		mItemAnimation->transformMachine()->setTranslationAt(1.0, 0.0, 0.0);
		mItemAnimation->transformMachine()->setScaleAt(0.0, hs, vs);
		mItemAnimation->transformMachine()->setScaleAt(1.0, 1.0, 1.0);
		hideGlow();
	}
	mItemAnimation->start();//start(QAbstractAnimation::KeepWhenStopped);
}

//image must be in size(mWidth, mHeight)
void ThumbItem::prepair()
{
	if (thumb.width() != mWidth || thumb.height() != mHeight) {
		qreal k = qreal(thumb.width())/qreal(thumb.height());
		qreal q = qreal(mWidth)/qreal(mHeight);
		if (k > q) {
			if (k > 1)
				mHeight = qreal(mWidth)/k;
		} else {
			if (k < 1)
				mWidth = mHeight * k;
		}
	}
}

void ThumbItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	//bool wasSmoothPixmapTransform = painter->testRenderHint(QPainter::SmoothPixmapTransform);
	//QMatrix m = painter->worldMatrix();
	//painter->setWorldMatrix(QMatrix());
	painter->save();
	painter->setPen(Qt::black);
	painter->drawRect(Config::thumbMargin, Config::thumbMargin, mWidth + 2*Config::thumbBorder, mHeight + 2*Config::thumbBorder);
	//painter->drawRect(-Config::thumbBorder, -Config::thumbBorder, thumb.width() + 2*Config::thumbBorder, thumb.height() + 2*Config::thumbBorder);
	painter->restore();
	painter->save();
	painter->translate(Config::thumbBorder + Config::thumbMargin,  Config::thumbBorder + Config::thumbMargin);
	painter->drawImage(QRectF(0, 0, mWidth, mHeight), thumb);
	//painter->drawImage(Config::thumbBorder + Config::thumbMargin, Config::thumbBorder + Config::thumbMargin, thumb);
	//painter->drawImage(0, 0, thumb);
	painter->restore();
}

void ThumbItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
#if ZOOM_ON_HOVER
    if (UiManager::lastHoverThumb) {
        UiManager::lastHoverThumb->zoom(ZoomOut);
		ezlog_debug("last hover: %#x", UiManager::lastHoverThumb);
    }
    UiManager::lastHoverThumb = this;
	//scene()->views().at(0)->centerOn(this);
    ensureVisible();
	zoom(ZoomIn);
#endif //ZOOM_ON_HOVER
	QGraphicsItem::hoverEnterEvent(event);
}

//TODO: record last zoom item. hover leave event some times not recived

void ThumbItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
#if ZOOM_ON_HOVER
    UiManager::lastHoverThumb = 0;
	zoom(ZoomOut);
#endif
	QGraphicsItem::hoverLeaveEvent(event);
}

void ThumbItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //toggle zoom
	ezlog_debug("last press: %#x", UiManager::lastHoverThumb);
	if (event->button() == Qt::LeftButton) {
		if (UiManager::lastHoverThumb) {
			ezlog_debug("zoom out");
			UiManager::lastHoverThumb->zoom(ZoomOut);
		}
		if (UiManager::lastHoverThumb == this) {
			UiManager::lastHoverThumb = 0;
			return;
		}
		if (!mGlow || !mGlow->isVisible()) {//test !mGlow first. mGlow may be 0
			ezlog_debug("zoom in");
			zoom(ZoomIn);
			showGlow();
			ensureVisible(); //UiManager::tryMoveCenter(this);
			//TODO: rewrite center on
			//scene()->views().at(0)->centerOn(this); //too fast
			UiManager::lastHoverThumb = this;
		}
		event->accept();
	}
	QGraphicsItem::mousePressEvent(event);
}

void ThumbItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	UiManager::instance()->gotoPage(UiManager::PlayPage, origin_image_path);
	QGraphicsItem::mouseDoubleClickEvent(event);
}

bool ThumbItem::sceneEvent(QEvent *event)
{
	switch (event->type()) {
	case QEvent::TouchBegin:
	case QEvent::TouchUpdate:
	case QEvent::TouchEnd:
	{
		QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
		if (touchEvent->touchPoints().count() == 2) {
			//select/unselect the item and show/hide an selected icon. show option bar at the same time
			//conflict with scene.event? or scene 3 touch event to do these options
			const QTouchEvent::TouchPoint &touchPoint1 = touchEvent->touchPoints().first();
			const QTouchEvent::TouchPoint &touchPoint2 = touchEvent->touchPoints().last();

			QLineF line1(touchPoint1.lastScenePos(), touchPoint2.lastScenePos());
			QLineF line2(touchPoint1.scenePos(), touchPoint2.scenePos());

			rotate(line2.angleTo(line1));
		}
		break;
	}
	default:
		return QGraphicsItem::sceneEvent(event);
	}
	return true;
}

} //namespace PhotoKit
