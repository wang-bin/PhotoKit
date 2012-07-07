/******************************************************************************
	PhotoKitView.cpp: description
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


#include "PhotoKitView.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QGraphicsItem>
#include <QtCore/QTimeLine>
#include <QtGui/QWheelEvent>
#include <QContextMenuEvent>
#include <QScrollBar>
#ifndef QT_NO_OPENGL
#include <QtOpenGL/QGLWidget>
#endif
#include "TransformMachine.h"
#include "PhotoKitScene.h"
#include "SlideDisplay.h"
#include "UiManager.h"
#include "BaseItem.h"
#include "Config.h"

#include "ezlog.h"
/*!
 * WARNING: QGraphicsView's translate does not work.in Qt4. We can create a root item and translate that.
 * QGraphicsItem::ItemIgnoresTransformations ignores except translation even if it's parent is 0. OtherWise
 * its position is still anchored to its parent. So if an item want to be in a fixed position, do not scroll
 * the content. key left/right/up/down will move the scroll, we must disable that. It seems that the only way
 * to translate the other items is using root item again!
*/

namespace PhotoKit {

static const qreal scale_max = 1.618;
static const qreal scale_min = 0.618;
static const qreal yrot_max = 35;
static const qreal yrot_min = -35;
static const qreal xrot_max = 8;
static const qreal xrot_min = -8;
static const qreal xshear_min = -0.2;
static const qreal xshear_max = 0.2;
static const qreal yshear_min = -0.2;
static const qreal yshear_max = 0.2;
static const int ani_duration = 1400;
PhotoKitView::PhotoKitView(QWidget *parent) :
	QGraphicsView(parent),mPressed(false),mScale(1.0),mX(0),mY(0),mMachine(0)
{
	//setDragMode(QGraphicsView::NoDrag);
    //setAlignment(Qt::AlignBottom);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn); //TODO: always on when debug.
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //setBackgroundBrush(QBrush(Qt::gray));
	mScene = new PhotoKitScene(this);

	setRenderingSystem();
	mScene->setSceneRect(qApp->desktop()->rect());
	setScene(mScene);

	mMachine = new TransformMachine;//QGraphicsItemAnimation;
	connect(mMachine, SIGNAL(transformChanged(QTransform)), SLOT(doTransform(QTransform)));

	QTimeLine *timer = new QTimeLine(ani_duration);
	timer->setEasingCurve(QEasingCurve::OutQuad);
	timer->setFrameRange(0, 100);
	mMachine->setTimeLine(timer);
}

//http://stackoverflow.com/questions/1355446/get-visible-rectangle-of-qgraphicsview
QRectF PhotoKitView::visibleSceneRect() const
{
    QPointF A = mapToScene(QPoint(0,0));
    QPointF B = mapToScene(QPoint(viewport()->width(), viewport()->height()));
	ezlog_debug("%f,%f %f,%f", A.x(), A.y(), B.x(), B.y());
    return QRectF(A, B);
}

void PhotoKitView::setAnimationDuration(int ms)
{
	mMachine->timeLine()->setDuration(ms);
}

void PhotoKitView::setInitialPos(qreal x, qreal y)
{
    mX = x;
    mY = y;
    translate(x, y);
}

//params x, y and scale are the final value. final rotation and shear params are the values in animation, final values are 0
void PhotoKitView::smartTransform(qreal x, qreal y, qreal scale0, qreal scale, qreal xrot, qreal yrot, qreal zrot, qreal xshear, qreal yshear)
{
	//ezlog_debug();
	qreal x0 = mX, y0 = mY;
	qreal s0 = scale0;
	qreal xr0 = 0, yr0 = 0;
	qreal hs0 = 0, vs0 = 0;
	if (mMachine->timeLine()->state() == QTimeLine::Running) {
		mMachine->timeLine()->setPaused(true);
		qreal step = mMachine->timeLine()->currentValue();
		s0 = mMachine->verticalScaleAt(step);
		x0 = mMachine->xTranslationAt(step);
		y0 = mMachine->yTranslationAt(step);
		xr0 = mMachine->xRotationAt(step);
		yr0 = mMachine->yRotationAt(step);
		hs0 = mMachine->horizontalShearAt(step);
		vs0 = mMachine->verticalShearAt(step);
		mMachine->timeLine()->stop();
	}
	mMachine->setTranslationAt(0, x0, y0);
	mMachine->setTranslationAt(1, x, y);
	mMachine->setScaleAt(0, s0, s0);
	mMachine->setScaleAt(1, scale, scale);
	mMachine->setRotationAt(0, xr0, yr0, 0);
	mMachine->setRotationAt(0.618, xrot, yrot, zrot);
	mMachine->setRotationAt(1, 0, 0, 0);
	mMachine->setShearAt(0, hs0, vs0);
	mMachine->setShearAt(0.618, xshear, yshear);
	mMachine->setShearAt(1, 0, 0);
	mMachine->timeLine()->start();
}

//TODO: Move to uimanager
void PhotoKitView::doTransform(const QTransform& m)
{
	//TODO: change origin. default origin is (0,0).
/*
	if (UiManager::page == UiManager::PlayPage) {
		ezlog_debug("slide page transform");
        UiManager::instance()->currentPageRootItem()->setTransform(m);
	} else if (UiManager::page == UiManager::ThumbPage) {
		ezlog_debug("thumb page transform");*/
    //UiManager::instance()->currentPageRootItem()->setTransformOriginPoint(UiManager::instance()->currentPageRootItem()->boundingRect().center());

    QGraphicsItem *item = UiManager::instance()->currentPageRootItem();
/*    qreal sx0 = item->transform().m11(), sy0 = item->transform().m22();
    if (sx0 != m.m11() || sy0 != m.m22()) {
        QRectF r = item->boundingRect();
        item->setTransform(QTransform().translate(r.width()/2, r.height()/2)
                           .scale(m.m11()/sx0, m.m22()/sy0)
                           .translate(-r.width()/2, -r.height()/2), true);
    } else {*/
        item->setTransform(m);//, true); //combine?
   // }
}

/*
bool PhotoKitView::event(QEvent *event)
{
	ezlog_debug("event: %d", event->type());
	return false;
}
*/
/*
void PhotoKitView::dragMoveEvent(QDragMoveEvent *event)
{
	if (UiManager::page != UiManager::ThumbPage) {

	}
	ezlog_debug("drag");
	event->accept();
}
*/
/*
void PhotoKitView::contextMenuEvent(QContextMenuEvent *event)
{
	ezlog_debug("PhotoKitView::contextMenuEvent");
	UiManager::instance()->popupMenu(mapToGlobal(event->pos()));
}*/

void PhotoKitView::keyPressEvent(QKeyEvent *e)
{ezlog_debug("key %d", e->key());
	switch(e->key()) {
	case Qt::Key_Right:
		break;
	case Qt::Key_Left:
		break;
    case Qt::Key_F:
        if (isFullScreen())
            showMaximized();
        else
            showFullScreen();
        break;
	default:
		break;
	}
    QGraphicsView::keyPressEvent(e); //TODO: enable textitem editable. Filter
	//e->accept(); //accept then scene with move if arrow keys pressed
}


void PhotoKitView::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mPressed = true;
        mMousePos = e->posF();
    }
	//mPressTime.restart();
	QGraphicsView::mousePressEvent(e);
}

//TODO: delta.y() to rotate around XAix and translate y. delta.x() rotate around YAix and translate x;
void PhotoKitView::mouseMoveEvent(QMouseEvent *e)
{
		QPointF delta = e->posF() - mMousePos;
		if (mPressed) {
            if (UiManager::page == UiManager::ThumbPage) {
                mX += delta.x() * 4;
                mY += delta.y();
                //TODO: move out visible area effect
                mX = qMax(mX, -sceneRect().width() +  rect().width()); //? +qreal(Config::contentHMargin). desktop
                mX = qMin(mX, qreal(Config::contentHMargin));
                mY = qMax(mY, qreal(0.0));
                mY = qMin(mY, qreal(Config::contentVMargin));
                //ezlog_debug("dx dy %d %d", delta.x(), delta.y());
                qreal hs = delta.x()/100;
                qreal vs = delta.y()/100;
                qreal xrot = delta.x()/6, yrot = delta.x()/6;
                if (delta.x() > 0) {
                    xrot = qMin(xrot, xrot_max);
                    hs = qMin(hs, xshear_max);
                } else {
                    xrot = qMax(xrot, xrot_min);
                    hs = qMax(hs, xshear_min);
                }
                if (delta.y() > 0) {
                    yrot = qMin(yrot, yrot_max);
                    vs = qMin(vs, yshear_max);
                } else {
                    yrot = qMax(yrot, yrot_min);
                    vs = qMax(vs, yshear_min);
                }
                //ezlog_debug("mX=%f my=%f", mX, mY);
                //moveWithAnimation(horizontalScrollBar()->value() - delta.x(), verticalScrollBar()->value() - delta.y());
                setAnimationDuration(ani_duration);
                smartTransform(mX, mY, mScale, mScale, xrot, yrot, 0, vs, hs);
			} else if (UiManager::page == UiManager::PlayPage) {
				//TODO: animation, don't move out of the viewport
                UiManager::instance()->currentPageRootItem()->setTransform(QTransform().translate(delta.x(), delta.y()), true);
            }
			mMousePos = e->posF();
		//ezlog_debug("move in view");
		//e->accept();
	}
        QGraphicsView::mouseMoveEvent(e); //WARNING: item will not recive hover event if remove this
    e->accept();
}

void PhotoKitView::mouseReleaseEvent(QMouseEvent *e)
{
	mPressed = false;
	mMousePos = e->posF();
	QGraphicsView::mouseReleaseEvent(e);
}

void PhotoKitView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (UiManager::page == UiManager::PlayPage) {
        UiManager::instance()->gotoPage(UiManager::ThumbPage);
        event->accept();
    } else {
        QGraphicsView::mouseDoubleClickEvent(event);
    }
}

void PhotoKitView::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;
	//ezlog_debug("wheel steps: %d", numSteps); //1
    if (UiManager::page == UiManager::ThumbPage) {
        qreal scale0 = mScale;
        if (numSteps > 0) {
            mScale += 0.12;
            mScale = qMin(scale_max, mScale);
        } else {
            mScale -= 0.12;
            mScale = qMax(scale_min, mScale);
        }
        setAnimationDuration(ani_duration);
        smartTransform(mX, mY, scale0, mScale, 0, 0, 0, 0, 0);
    } else if (UiManager::page == UiManager::PlayPage) {
        QGraphicsItem *item =  UiManager::instance()->currentPageRootItem();
        qreal s = numSteps > 0 ? 1.1:0.9;
        QRectF r = item->boundingRect();
        item->setTransform(QTransform().translate(r.width()/2, r.height()/2)
                           .scale(s, s).translate(-r.width()/2, -r.height()/2), true);
    }
	//QGraphicsView::wheelEvent(event); //will scroll the content. centerOn will not work
}

void PhotoKitView::resizeEvent(QResizeEvent *event)
{
	//visibleSceneRect();
	//ezlog_debug("resize: %dx%d", event->size().width(), event->size().height());
	//UiManager::instance()->updateFixedItems();
   /* this->resetMatrix();
    this->scale(event->size().width() / 800.0, event->size().height() / 600.0); //?
    QGraphicsView::resizeEvent(event);
    BaseItem::setMatrix(this->matrix());*/
    QGraphicsView::resizeEvent(event);
}

bool PhotoKitView::viewportEvent(QEvent *event)
{
	switch (event->type()) {
	case QEvent::TouchBegin:
	case QEvent::TouchUpdate:
	case QEvent::TouchEnd:
	{
		//only do these in picture wall mode
		QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
		QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
		if (touchPoints.count() == 2) {//TODO: next/pre in PlayPage
			// determine scale factor
			const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
			const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
			qreal currentScaleFactor =
					QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
					/ QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
			if (touchEvent->touchPointStates() & Qt::TouchPointMoved
					|| touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                if (UiManager::page == UiManager::ThumbPage) {
                    qreal scale0 = mScale;
                    if (currentScaleFactor > 1) {
                        mScale += 0.12;
                        mScale = qMin(scale_max, mScale);
                    } else {
                        mScale -= 0.12;
                        mScale = qMax(scale_min, mScale);
                    }
                    setAnimationDuration(ani_duration);
                    smartTransform(mX, mY, scale0, mScale, 0, 0, 0, 0, 0); //both thumbpage and playpage works
                } else if (UiManager::page == UiManager::PlayPage) {
                    QGraphicsItem *item =  UiManager::instance()->currentPageRootItem();
                    qreal s0 = item->transform().m11();
                    qreal s = currentScaleFactor > 1 ? 1.1:0.9;
                    QRectF r = item->boundingRect();
                    item->setTransform(QTransform().translate(r.width()/2, r.height()/2)
                                       .scale(s, s).translate(-r.width()/2, -r.height()/2), true);
                }
            }
		}
		return true;
	}
	default:
		break;
    }
	return QGraphicsView::viewportEvent(event);
}


void PhotoKitView::setRenderingSystem()
{
	QWidget *viewport = 0;

#ifndef QT_NO_OPENGL
	if (Config::openGlRendering) {
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
		QGLWidget *glw = new QGLWidget(QGLFormat(QGL::SampleBuffers));
		if (Config::noScreenSync)
			glw->format().setSwapInterval(0);
		glw->setAutoFillBackground(false);
		viewport = glw;
		setCacheMode(QGraphicsView::CacheNone);
		if (Config::verbose)
			ezlog_debug("- using OpenGL");
	} else // software rendering
#endif
	{
		// software rendering
        setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
		viewport = new QWidget;
		setCacheMode(QGraphicsView::CacheBackground);
		if (Config::verbose)
			ezlog_debug("- using software rendering");
	}

	setViewport(viewport);
}

} //namespace PhotoKit
