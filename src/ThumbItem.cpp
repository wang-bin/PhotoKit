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
#include <QtCore/QTimeLine>
#include <QGraphicsItemAnimation>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "ItemAnimation.h"
#include "OutlineGlowItem.h"
#include "TransformMachine.h"
#include "Config.h"

BEGIN_NAMESPACE_PHOTOKIT

//TODO: calculate to fit screen
static const qreal zoom_max = 2.2;

ThumbItem::ThumbItem(QGraphicsItem *parent) :
	QGraphicsItem(parent),mGlow(0),mAnimation(0),mItemAnimation(0)
{
	setAcceptHoverEvents(true); //default: false
	thumb = QImage(Config::thumbItemWidth, Config::thumbItemHeight, QImage::Format_ARGB32_Premultiplied);
}

ThumbItem::ThumbItem(const QImage& image, QGraphicsItem *parent) :
	QGraphicsItem(parent),thumb(image),mGlow(0),mAnimation(0)
{
	setAcceptHoverEvents(true); //default: false
}

ThumbItem::~ThumbItem()
{
	if (mAnimation) {
		delete mAnimation;
		mAnimation = 0;
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
	mGlow->setVisible(false);
}
//TODO:matrix
void ThumbItem::zoom(ZoomAction action)
{
	if (!mAnimation) {
		mAnimation = new TransformMachine;//QGraphicsItemAnimation;
		mItemAnimation = new ItemAnimation(this);
		QObject::connect(mAnimation, SIGNAL(matrixChanged(QMatrix)), mItemAnimation, SLOT(setMatrix(QMatrix)));
		QObject::connect(mAnimation, SIGNAL(zValueChanged(qreal)), mItemAnimation, SLOT(setZValue(qreal)));

		//mAnimation->setItem(this);
		QTimeLine *timer = new QTimeLine(1000);
		timer->setEasingCurve(QEasingCurve::OutQuad);
		timer->setFrameRange(0, 100);
		mAnimation->setTimeLine(timer);
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
	if (mAnimation->timeLine()->state() == QTimeLine::Running) {
		mAnimation->timeLine()->setPaused(true);
		qreal step = mAnimation->timeLine()->currentValue();
		vs = mAnimation->verticalScaleAt(step);
		hs = mAnimation->horizontalScaleAt(step);
		tx = mAnimation->xTranslationAt(step);
		ty = mAnimation->yTranslationAt(step);
		z = mAnimation->zValueAt(step);
		mAnimation->timeLine()->stop();
	} else {
		if (action == ZoomOut) {
			vs = zoom_max;
			hs = zoom_max;
			tx = tx_dst;
			ty = ty_dst;
			z = 2.0;
		}
	}
	//mAnimation->setStartMatrix(matrix());
	//setTransformOriginPoint(transform().mapRect(boundingRect()).center());
	if (action == ZoomIn) {
		mAnimation->setZValueAt(0.0, z);
		mAnimation->setZValueAt(1.0, 2.0);
		mAnimation->setTranslationAt(0.0, tx, ty);
		mAnimation->setTranslationAt(1.0, tx_dst, ty_dst);
		mAnimation->setScaleAt(0.0, hs, vs);
		mAnimation->setScaleAt(1.0, zoom_max, zoom_max);
	} else {
		mAnimation->setZValueAt(0.0, z);
		mAnimation->setZValueAt(1.0, 0);
		mAnimation->setTranslationAt(0.0, tx, ty);
		mAnimation->setTranslationAt(1.0, 0.0, 0.0);
		mAnimation->setScaleAt(0.0, hs, vs);
		mAnimation->setScaleAt(1.0, 1.0, 1.0);
	}
	mAnimation->timeLine()->start();//start(QAbstractAnimation::KeepWhenStopped);
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
	//scene()->views().at(0)->centerOn(this);
	//scene()->views().at(0)->ensureVisible(this);
	//setTransform(QTransform().scale(2.0, 2.0));
	//qDebug("z=%f", zValue()); //keep in animation
	setZValue(zValue() + 1.0);
	zoom(ZoomIn);
	showGlow();
	QGraphicsItem::hoverEnterEvent(event);
}

void ThumbItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	//scene()->views().at(0)->ensureVisible(this);
	//setTransform(QTransform().scale(1.0, 1.0));
	setZValue(zValue() - 1);
	zoom(ZoomOut);
	hideGlow();
	QGraphicsItem::hoverLeaveEvent(event);
}

void ThumbItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void ThumbItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}

END_NAMESPACE_PHOTOKIT
