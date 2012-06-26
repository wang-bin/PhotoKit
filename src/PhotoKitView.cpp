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
#include <QScrollBar>
#ifndef QT_NO_OPENGL
#include <QtOpenGL/QGLWidget>
#endif
#include "TransformMachine.h"
#include "PhotoKitScene.h"
#include "Config.h"

namespace PhotoKit {

static const qreal zoom_max = 1.618;
static const qreal zoom_min = 0.618;
PhotoKitView::PhotoKitView(QWidget *parent) :
	QGraphicsView(parent),mPressed(false),mScale(1.0),mMachine(0)
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse); //AnchorViewCenter
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setBackgroundBrush(QBrush(Qt::gray));
	mScene = new PhotoKitScene(this);

	setRenderingSystem();
	mScene->setSceneRect(qApp->desktop()->rect());
	setScene(mScene);

	mMachine = new TransformMachine;//QGraphicsItemAnimation;
	connect(mMachine, SIGNAL(matrixChanged(QMatrix)), SLOT(doTransform(QMatrix)));

	QTimeLine *timer = new QTimeLine(1000);
	timer->setEasingCurve(QEasingCurve::OutQuad);
	timer->setFrameRange(0, 100);
	mMachine->setTimeLine(timer);
}

void PhotoKitView::scaleView(ZoomAction zoom)
{
	//QPointF c = mapToScene(rect().center());
	//qDebug("c: %f %f", c.x(), c.y());
	if (zoom == ZoomIn) {
		mScale += 0.2;
		mScale = qMin(mScale, zoom_max);
	} else {
		mScale -= 0.2;
		mScale = qMax(mScale, zoom_min);
	}
	setTransform(QTransform()//.translate(contentsRect().width(), contentsRect().height())
			.scale(mScale , mScale)
			//.translate(-contentsRect().width(), -contentsRect().height())
		);
	//centerOn(c);
}

void PhotoKitView::scaleWithAnimation(ZoomAction action)
{

	qreal vs = mScale;
	qreal hs = mScale;
	//qreal tx = 0;
	//qreal ty = 0;
	if (action == ZoomIn) {
		mScale += 0.2;
		mScale = qMin(mScale, zoom_max);
	} else {
		mScale -= 0.2;
		mScale = qMax(mScale, zoom_min);
	}
	qDebug("scale: %f", mScale);
	/*!
		translate then zoom. so if keep the center, the translation t satisfies
		t_dst * s = 0.5 * (w - w0, h - h0) = 0.5 * (s - 1) * (w0, h0)
	*/
	//qreal tx_dst = -boundingWidth()*0.5*(zoom_max - 1.0)/zoom_max;
	//qreal ty_dst = -boundingHeight()*0.5*(zoom_max - 1.0)/zoom_max;
	if (mMachine->timeLine()->state() == QTimeLine::Running) {
		mMachine->timeLine()->setPaused(true);
		qreal step = mMachine->timeLine()->currentValue();
		vs = mMachine->verticalScaleAt(step);
		hs = mMachine->horizontalScaleAt(step);
		//tx = mMachine->xTranslationAt(step);
		//ty = mMachine->yTranslationAt(step);
		mMachine->timeLine()->stop();
	} else {
		if (action == ZoomOut) {
			//vs = zoom_max;
			//hs = zoom_max;
			//tx = tx_dst;
			//ty = ty_dst;
		}
	}
	//setTransformOriginPoint(transform().mapRect(boundingRect()).center());
	if (action == ZoomIn) {
		//mMachine->setTranslationAt(0.0, tx, ty);
		//mMachine->setTranslationAt(1.0, tx_dst, ty_dst);
		mMachine->setScaleAt(0.0, hs, vs);
		mMachine->setScaleAt(1.0, mScale, mScale);
	} else {
		//mMachine->setTranslationAt(0.0, tx, ty);
		//mMachine->setTranslationAt(1.0, 0.0, 0.0);
		mMachine->setScaleAt(0.0, hs, vs);
		mMachine->setScaleAt(1.0, mScale, mScale);
	}
	mMachine->timeLine()->start();//start(QAbstractAnimation::KeepWhenStopped);
}

void PhotoKitView::moveWithAnimation(qreal dx, qreal dy)
{
	qreal tx = 0;
	qreal ty = 0;

	if (mMachine->timeLine()->state() == QTimeLine::Running) {
		mMachine->timeLine()->setPaused(true);
		qreal step = mMachine->timeLine()->currentValue();
		tx = mMachine->xTranslationAt(step);
		ty = mMachine->yTranslationAt(step);
		mMachine->timeLine()->stop();
	}
	//mMachine->setStartMatrix(QMatrix().translate(this->matrix().dx(), this->matrix().dy()));
	mMachine->setTranslationAt(0.0, tx, ty);
	mMachine->setTranslationAt(1.0, dx, dy);
	mMachine->timeLine()->start();//start(QAbstractAnimation::KeepWhenStopped);
}


void PhotoKitView::doTransform(const QMatrix& m)
{
	setTransform(QTransform(m));//, true); //combine?
}

//TODO: delta.y() to rotate around XAix and translate y. delta.x() rotate around YAix and translate x;
void PhotoKitView::mouseMoveEvent(QMouseEvent *e)
{
	QPoint delta = e->pos() - mMousePos;
	if (mPressed) {
		qDebug("%d", delta.x()); //not const. delta.x() is the max value of Easing
		//moveWithAnimation(horizontalScrollBar()->value() - delta.x(), verticalScrollBar()->value() - delta.y());
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
		verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
		//setTransform(transform().translate(-e->x(), -e->y()));
		//setSceneRect(sceneRect().translated(-delta));
		mMousePos = e->pos();
	}
	QGraphicsView::mouseMoveEvent(e);
}

void PhotoKitView::mousePressEvent(QMouseEvent *e)
{
	mPressed = true;
	mMousePos = e->pos();
	//mPressTime.restart();
}

void PhotoKitView::mouseReleaseEvent(QMouseEvent *e)
{
	mPressed = false;
	mMousePos = e->pos();
}

void PhotoKitView::keyPressEvent(QKeyEvent *e)
{qDebug("key %d", e->key());
	switch(e->key()) {
	case Qt::Key_Right:
		qDebug("key right");
		setTransform(QTransform().rotate(1, Qt::YAxis).translate(-66,0), true);
		break;
	case Qt::Key_Left:
		qDebug("key left");
		setTransform(QTransform().rotate(-1, Qt::YAxis).translate(66, 0), true);

		break;
	default:
		break;
	}
	e->accept();
}

void PhotoKitView::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;
	//qDebug("wheel steps: %d", numSteps); //1
	if (numSteps > 0) {
		scaleWithAnimation(ZoomIn);
	} else {
		scaleWithAnimation(ZoomOut);
	}
	//QGraphicsView::wheelEvent(event); //will scroll the content. centerOn will not work
}

void PhotoKitView::setRenderingSystem()
{
	QWidget *viewport = 0;

#ifndef QT_NO_OPENGL
	if (Config::openGlRendering) {
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
		viewport = new QWidget;
		setCacheMode(QGraphicsView::CacheBackground);
		if (Config::verbose)
			qDebug("- using software rendering");
	}

	setViewport(viewport);
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

} //namespace PhotoKit
