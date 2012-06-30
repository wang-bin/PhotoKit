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
#include "ItemAnimation.h"
#include "OutlineGlowItem.h"
#include "TransformMachine.h"
#include "UiManager.h"
#include "Config.h"

namespace PhotoKit {

//TODO: calculate to fit screen
static const qreal zoom_max = 2.2;

ThumbItem::ThumbItem(QGraphicsItem *parent) :
	QGraphicsItem(parent),mGlow(0),mMachine(0),mItemAnimation(0)
{
	//setAcceptTouchEvents(true);
	setAcceptHoverEvents(true); //default: false
	setCacheMode(QGraphicsItem::ItemCoordinateCache); //item.scroll enabled(not for gl). speed up
	thumb = QImage(Config::thumbItemWidth, Config::thumbItemHeight, QImage::Format_ARGB32_Premultiplied);
}

ThumbItem::ThumbItem(const QImage& image, QGraphicsItem *parent) :
	QGraphicsItem(parent),thumb(image),mGlow(0),mMachine(0)
{
	setAcceptHoverEvents(true); //default: false
}

ThumbItem::~ThumbItem()
{
	if (mMachine) {
		delete mMachine;
		mMachine = 0;
	}
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

void ThumbItem::setThumbImage(const QImage& image)
{
	thumb = image;
	update(boundingRect());
}
//add mirror rect?
QRectF ThumbItem::boundingRect() const
{
	return thumb.rect().adjusted(0, 0//-(Config::thumbMargin + Config::thumbBorder) , -(Config::thumbBorder + Config::thumbMargin)
					, 2*(Config::thumbBorder + Config::thumbMargin), 2*(Config::thumbBorder + Config::thumbMargin));
}

qreal ThumbItem::boundingWidth() const
{
	return boundingRect().size().width();
}

qreal ThumbItem::boundingHeight() const
{
	return boundingRect().size().height();
}

void ThumbItem::showGlow()
{
	if (!mGlow) {
		qDebug("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
		mGlow = new OutlineGlowItem(this);
		//mGlow->setZValue(zValue() + 1);
		QSizeF s = boundingRect().size();
		mGlow->setSize(QSize((int)s.width(), (int)s.height()));
		mGlow->setGlowWidth(Config::thumbMargin);// + Config::thumbBorder);
		QPainterPath image_shape;
		image_shape.addRect(Config::thumbMargin, Config::thumbMargin//-Config::thumbBorder, -Config::thumbBorder
					, thumb.width() + 2*Config::thumbBorder, thumb.height() + 2*Config::thumbBorder);
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
	if (!mMachine) {
		mMachine = new TransformMachine;//QGraphicsItemAnimation;
		mItemAnimation = new ItemAnimation(this);
		QObject::connect(mMachine, SIGNAL(transformChanged(QTransform)), mItemAnimation, SLOT(setTransform(QTransform)));
		QObject::connect(mMachine, SIGNAL(zValueChanged(qreal)), mItemAnimation, SLOT(setZValue(qreal)));

		//mMachine->setItem(this);
        QTimeLine *timer = new QTimeLine(1000);
		timer->setEasingCurve(QEasingCurve::OutQuad);
        timer->setFrameRange(0, 100);
		mMachine->setTimeLine(timer);
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
	qreal tx_dst = -boundingWidth()*0.5*(zoom_max - 1.0)/zoom_max;
	qreal ty_dst = -boundingHeight()*0.5*(zoom_max - 1.0)/zoom_max;
	if (mMachine->timeLine()->state() == QTimeLine::Running) {
		mMachine->timeLine()->setPaused(true);
		qreal step = mMachine->timeLine()->currentValue();
		vs = mMachine->verticalScaleAt(step);
		hs = mMachine->horizontalScaleAt(step);
		tx = mMachine->xTranslationAt(step);
		ty = mMachine->yTranslationAt(step);
		z = mMachine->zValueAt(step);
		mMachine->timeLine()->stop();
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
		mMachine->setZValueAt(0.0, z);
		mMachine->setZValueAt(1.0, 2.0);
		mMachine->setTranslationAt(0.0, tx, ty);
		mMachine->setTranslationAt(1.0, tx_dst, ty_dst);
		mMachine->setScaleAt(0.0, hs, vs);
		mMachine->setScaleAt(1.0, zoom_max, zoom_max);
	} else {
        hideGlow();
		mMachine->setZValueAt(0.0, z);
		mMachine->setZValueAt(1.0, 0);
		mMachine->setTranslationAt(0.0, tx, ty);
		mMachine->setTranslationAt(1.0, 0.0, 0.0);
		mMachine->setScaleAt(0.0, hs, vs);
		mMachine->setScaleAt(1.0, 1.0, 1.0);
	}
	mMachine->timeLine()->start();//start(QAbstractAnimation::KeepWhenStopped);
}

void ThumbItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	//bool wasSmoothPixmapTransform = painter->testRenderHint(QPainter::SmoothPixmapTransform);
    painter->save();
	painter->setPen(Qt::black);
	painter->drawRect(Config::thumbMargin, Config::thumbMargin, thumb.width() + 2*Config::thumbBorder, thumb.height() + 2*Config::thumbBorder);
	//painter->drawRect(-Config::thumbBorder, -Config::thumbBorder, thumb.width() + 2*Config::thumbBorder, thumb.height() + 2*Config::thumbBorder);
    painter->restore();
	painter->drawImage(Config::thumbBorder + Config::thumbMargin, Config::thumbBorder + Config::thumbMargin, thumb);
	//painter->drawImage(0, 0, thumb);
}

void ThumbItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
#if ZOOM_ON_HOVER
    if (UiManager::lastHoverThumb) {
        UiManager::lastHoverThumb->zoom(ZoomOut);
        qDebug("last hover: %#x", UiManager::lastHoverThumb);
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
	qDebug("last press: %#x", UiManager::lastHoverThumb);
	if (UiManager::lastHoverThumb) {
		qDebug("zoom out");
		UiManager::lastHoverThumb->zoom(ZoomOut);
	}
	if (UiManager::lastHoverThumb == this) {
		UiManager::lastHoverThumb = 0;
		return;
	}
	if (!mGlow || !mGlow->isVisible()) {//test !mGlow first. mGlow may be 0
		qDebug("zoom in");
		zoom(ZoomIn);
		showGlow();
		ensureVisible();
		//TODO: rewrite center on
		//scene()->views().at(0)->centerOn(this); //too fast
		UiManager::lastHoverThumb = this;
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
