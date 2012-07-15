/******************************************************************************
	SlideDisplay: an item to show slide effect
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


#include "SlideDisplay.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>
#include <QtCore/QTimeLine>
#include "SlidePlayControl.h"
#include "UiManager.h"
#include "nexteffect.h"
#include "TransformMachine.h"
#include "Config.h"
#include "ezlog.h"
namespace PhotoKit {

SlideDisplay::SlideDisplay(QGraphicsItem *parent)
	:BaseItem(parent),mMode(SingleImage)
{
	mSlideEffect = 0;
    mMaxWidth = (qreal)qApp->desktop()->width();
    mMaxHeight = (qreal)qApp->desktop()->height();
    mWidth = mMaxWidth;
    mHeight = mMaxHeight;
	setAcceptTouchEvents(true);
	setAcceptDrops(true);
    //setFlag(QGraphicsItem::ItemIgnoresTransformations); //zoom origin will be bottom right, why?

	mItemAnimation = new ItemAnimation(this);
	//mMachine->setItem(this);
}

void SlideDisplay::setImagePath(const QString &path)
{
	mPath = path;
	prepairImage();
}

void SlideDisplay::setPlayControl(SlidePlayControl *control)
{
	mControl = control;
	mControl->setDisplay(this);
}

SlidePlayControl* SlideDisplay::playControl() const
{
	return mControl;
}

QString SlideDisplay::imagePath() const
{
	return mPath;
}

void SlideDisplay::setEffect(NextEffect *effect)
{
	mSlideEffect = effect;
}

QSize SlideDisplay::size()
{
    return QSize(mMaxWidth, mMaxHeight);
}

QRectF SlideDisplay::boundingRect() const
{/*
    if (mImage.isNull()) {
        prepairImage();
    }*/
    return QRectF(0, 0, mMaxWidth, mMaxHeight);
}

void SlideDisplay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	if (!mControl->isRunning()) {
		if (mImage.isNull())
			prepairImage();
		painter->drawImage(boundingRect(), mImage);
	} else {

		painter->drawImage(boundingRect(), *mSlideEffect->currentFrame());
	}
}

void SlideDisplay::smoothScale(qreal s0, qreal s, ItemAnimation::Fade fade)
{
	mItemAnimation->setFade(fade);
	/*!
		translate then zoom(origin is the origin origin). so if keep the center, the translation t satisfies
		t_dst * s = 0.5 * (w - w0, h - h0) = 0.5 * (s - 1) * (w0, h0)
		here w0 = mWidth
	*/
	qreal tx0 = -mWidth*0.5*(s0 - 1.0)/s0;
	qreal ty0 = -mHeight*0.5*(s0 - 1.0)/s0;
	qreal tx = -mWidth*0.5*(s - 1.0)/s;
	qreal ty = -mHeight*0.5*(s - 1.0)/s;
	if (mItemAnimation->isRunning()) {
		mItemAnimation->stop();
		qreal step = mItemAnimation->currentStep();
		s0 = mItemAnimation->transformMachine()->verticalScaleAt(step);
		tx0 = mItemAnimation->transformMachine()->xTranslationAt(step);
		ty0 = mItemAnimation->transformMachine()->yTranslationAt(step);
	}
	//mMachine->setStartMatrix(matrix());
	//setTransformOriginPoint(transform().mapRect(boundingRect()).center());
	mItemAnimation->transformMachine()->setTranslationAt(0.0, tx0, ty0);
	mItemAnimation->transformMachine()->setTranslationAt(1.0, tx, ty);
	mItemAnimation->transformMachine()->setScaleAt(0.0, s0, s0);
	mItemAnimation->transformMachine()->setScaleAt(1.0, s, s);
	mItemAnimation->start();//start(QAbstractAnimation::KeepWhenStopped);
}

void SlideDisplay::prepairImage()
{
	mImage.load(mPath);
    //scale width, height 1 by 1
#if 0
    mWidth = mImage.width();
    mHeight = mImage.height();
    if (mWidth > mMaxWidth) {
        mHeight = mMaxWidth * mHeight/mWidth; //set image width to mMaxWidth;
        mWidth = mMaxWidth;
    }
    if (mHeight > mMaxHeight) {
        mWidth = mMaxHeight * mWidth/mHeight; //set image height to mMaxHeight;
        mHeight = mMaxHeight;
    }
    setTransform(mWidth/mImage.width(), mHeight/mImage.height());
#else
    //scale to fit screen
	if (mImage.size() != size()) {
		if (Config::keepAspectRatio) {
			mImage = mImage.scaled(size(), Qt::KeepAspectRatio);
			if (mImage.size() != size()) {
				QImage backGoundImage1(size(), QImage::Format_ARGB32_Premultiplied);
				QPainter painter(&backGoundImage1);
				painter.fillRect(backGoundImage1.rect(), Qt::transparent);
				painter.drawImage((mWidth-mImage.width())/2, (mHeight-mImage.height())/2, mImage);
				mImage = backGoundImage1;
			}
		} else {
			mImage = mImage.scaled(size(), Qt::IgnoreAspectRatio);
		}
    }
#endif
}

void SlideDisplay::keyPressEvent(QKeyEvent *event)
{
	//move to view
	switch(event->key()) {
	case Qt::Key_Space:
		if (mControl->isRunning()) {
			mControl->stop();
		} else {
			mControl->setDirection(SlidePlayControl::Forward);
			mControl->setCurrentImage(mPath);
			mControl->start();
		}
		break;
	}
	//event->accept();
	QGraphicsItem::keyPressEvent(event);
}

/*
	SingleImage: move to move image.
	SlideImage
*/
/*
void SlideDisplay::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	ezlog_debug("Slide  pressed");
	mMousePos = event->pos();
	if (mMode == SingleImage) {
		mMouseOnTime.restart();

	} else if (mMode == SlideImage) {

	}
	QGraphicsItem::mousePressEvent(event);
}
*//*
void SlideDisplay::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ezlog_debug("elapsed: %d", mMouseOnTime.elapsed());*/
	/*if (mMode != SingleImage) {
		QGraphicsItem::mouseReleaseEvent(event);
		return;
	}*/
/*
	if (mMouseOnTime.elapsed() < 200) {
		QPointF delta = (event->pos() - mMousePos);
		if (delta.x() > 0) {
			mControl->setDirection(SlidePlayControl::Forward);
		} else {
			mControl->setDirection(SlidePlayControl::Backward);
		}
		mControl->setCurrentImage(mPath);
		mControl->startOne();
	}

	QGraphicsItem::mouseReleaseEvent(event);
}

void SlideDisplay::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	ezlog_debug("Slide mouse move");
	if (mMode == SingleImage) {
		//translate the item. ensureVisible; //2 finger to next/pre
		//
		QPointF delta = (event->pos() - mMousePos);
		mMousePos = event->pos();
		QTransform t;
		t.translate(delta.x(), delta.y());
		setTransform(t, true);
	}
	QGraphicsItem::mouseMoveEvent(event);
}
*/
void SlideDisplay::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	//go back to ThumbPage
	if (mControl->isRunning())
		mControl->stop();
	UiManager::instance()->gotoPage(UiManager::ThumbPage);
	QGraphicsItem::mouseDoubleClickEvent(event);
}
/*
bool SlideDisplay::sceneEvent(QEvent *event)
{
	return QGraphicsItem::sceneEvent(event);
}
*/
}

