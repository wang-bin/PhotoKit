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
#include "Config.h"

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
static const qreal yrot_max = 45;
static const qreal yrot_min = -45;
static const qreal xrot_max = 8;
static const qreal xrot_min = -8;
PhotoKitView::PhotoKitView(QWidget *parent) :
	QGraphicsView(parent),mPressed(false),mScale(1.0),mX(0),mY(0),mMachine(0)
{
	//setDragMode(QGraphicsView::NoDrag);
    //setAlignment(Qt::AlignBottom);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setBackgroundBrush(QBrush(Qt::gray));
	mScene = new PhotoKitScene(this);

	setRenderingSystem();
	mScene->setSceneRect(qApp->desktop()->rect());
	setScene(mScene);

	mMachine = new TransformMachine;//QGraphicsItemAnimation;
	connect(mMachine, SIGNAL(transformChanged(QTransform)), SLOT(doTransform(QTransform)));

	QTimeLine *timer = new QTimeLine(1234);
	timer->setEasingCurve(QEasingCurve::OutQuad);
	timer->setFrameRange(0, 100);
	mMachine->setTimeLine(timer);
}

//http://stackoverflow.com/questions/1355446/get-visible-rectangle-of-qgraphicsview
QRectF PhotoKitView::visibleSceneRect() const
{
    QPointF A = mapToScene(QPoint(0,0));
    QPointF B = mapToScene(QPoint(viewport()->width(), viewport()->height()));
    qDebug("%f,%f %f,%f", A.x(), A.y(), B.x(), B.y());
    return QRectF(A, B);
}

//params x, y and scale are the final value. final rotation and shear params are the values in animation, final values are 0
void PhotoKitView::smoothTransform(qreal x, qreal y, qreal scale, qreal xrot, qreal yrot, qreal zrot, qreal xshear, qreal yshear)
{
	qreal x0 = mX, y0 = mY;
	qreal s0 = mScale;
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

//TODO:
void PhotoKitView::doTransform(const QTransform& m)
{
	//TODO: change origin. default origin is (0,0).
/*
	if (UiManager::page == UiManager::PlayPage) {
		qDebug("slide page transform");
		UiManager::instance()->playPageItem()->setTransform(m);
	} else if (UiManager::page == UiManager::ThumbPage) {
		qDebug("thumb page transform");*/
		UiManager::instance()->thumbPageRootItem()->setTransform(m);//, true); //combine?
	//}
}

/*
bool PhotoKitView::event(QEvent *event)
{
	qDebug("event: %d", event->type());
	return false;
}
*/
/*
void PhotoKitView::dragMoveEvent(QDragMoveEvent *event)
{
	if (UiManager::page != UiManager::ThumbPage) {

	}
	qDebug("drag");
	event->accept();
}
*/

void PhotoKitView::contextMenuEvent(QContextMenuEvent *event)
{
	qDebug("PhotoKitView::contextMenuEvent");
	UiManager::instance()->popupMenu(mapToGlobal(event->pos()));
}

void PhotoKitView::keyPressEvent(QKeyEvent *e)
{qDebug("key %d", e->key());
	if (UiManager::page != UiManager::ThumbPage) {

	}
	switch(e->key()) {
	case Qt::Key_Right:
		break;
	case Qt::Key_Left:
		break;
	default:
		break;
	}
	//e->accept(); //accept then scene with move if arrow keys pressed
}


void PhotoKitView::mousePressEvent(QMouseEvent *e)
{
	if (UiManager::page != UiManager::ThumbPage) {

	}
	mPressed = true;
	mMousePos = e->posF();
	//mPressTime.restart();
	QGraphicsView::mousePressEvent(e);
}

//TODO: delta.y() to rotate around XAix and translate y. delta.x() rotate around YAix and translate x;
void PhotoKitView::mouseMoveEvent(QMouseEvent *e)
{
	if (!UiManager::instance()->thumbPageRootItem()->isVisible()) {
		return;
	}

	QPointF delta = e->posF() - mMousePos;
	if (mPressed) {
		mX += delta.x() * 2;
		mY += delta.y();
		//TODO: move out visible area effect
		mX = qMax(mX, -sceneRect().width() +  rect().width()); //? +qreal(Config::contentHMargin). desktop
		mX = qMin(mX, qreal(Config::contentHMargin));
		mY = qMax(mY, qreal(0.0));
		mY = qMin(mY, qreal(Config::contentVMargin));
		//qDebug("dx dy %d %d", delta.x(), delta.y());

		qreal xrot, yrot;
		if (delta.x() > 0) {
			xrot = qMin(delta.x()/4, xrot_max);
		} else {
			xrot = qMax(delta.x()/4, xrot_min);
		}
		if (delta.y() > 0) {
			yrot = qMin(delta.y()*4, yrot_max);
		} else {
			yrot = qMax(delta.y()*4, yrot_min);
		}
		//qDebug("mX=%f my=%f", mX, mY);
		//moveWithAnimation(horizontalScrollBar()->value() - delta.x(), verticalScrollBar()->value() - delta.y());
		smoothTransform(mX, mY, mScale, xrot, yrot, 0, 0, 0);
		mMousePos = e->posF();
	}
	//qDebug("move in view");
	//e->accept();
	QGraphicsView::mouseMoveEvent(e); //WARNING: item will not recive hover event if remove this
}

void PhotoKitView::mouseReleaseEvent(QMouseEvent *e)
{
	if (UiManager::page != UiManager::ThumbPage) {

	}
	mPressed = false;
	mMousePos = e->posF();
	QGraphicsView::mouseReleaseEvent(e);
}

void PhotoKitView::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;
	//qDebug("wheel steps: %d", numSteps); //1
	if (numSteps > 0) {
		mScale += 0.12;
		mScale = qMin(scale_max, mScale);
	} else {
		mScale -= 0.12;
		mScale = qMax(scale_min, mScale);
	}
	smoothTransform(mX, mY, mScale, 0, 0, 0, 0, 0);
	//QGraphicsView::wheelEvent(event); //will scroll the content. centerOn will not work
}

void PhotoKitView::resizeEvent(QResizeEvent *event)
{
	//visibleSceneRect();
	//qDebug("resize: %dx%d", event->size().width(), event->size().height());
	//UiManager::instance()->updateFixedItems();
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
		if (touchPoints.count() == 2) {
			// determine scale factor
			const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
			const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
			qreal currentScaleFactor =
					QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
					/ QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
			if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
				// if one of the fingers is released, remember the current scale
				// factor so that adding another finger later will continue zooming
				// by adding new scale factor to the existing remembered value.
				//totalScaleFactor *= currentScaleFactor;
				//currentScaleFactor = 1;
			}
			//setTransform(QTransform().scale(totalScaleFactor * currentScaleFactor,
			//								totalScaleFactor * currentScaleFactor));
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
			qDebug("- using OpenGL");
	} else // software rendering
#endif
	{
		// software rendering
        setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
		viewport = new QWidget;
		setCacheMode(QGraphicsView::CacheBackground);
		if (Config::verbose)
			qDebug("- using software rendering");
	}

	setViewport(viewport);
}

} //namespace PhotoKit
