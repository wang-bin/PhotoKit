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
#include <QtGui/QWheelEvent>
#include <QScrollBar>
#ifndef QT_NO_OPENGL
#include <QtOpenGL/QGLWidget>
#endif
#include "PhotoKitScene.h"
#include "Config.h"
BEGIN_NAMESPACE_PHOTOKIT

PhotoKitView::PhotoKitView(QWidget *parent) :
	QGraphicsView(parent),mPressed(false),mScale(1.0)
{
	qDebug("ctor view");
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setBackgroundBrush(QBrush(Qt::gray));
	mScene = new PhotoKitScene(this);

	setRenderingSystem();
	mScene->setSceneRect(qApp->desktop()->rect());
	setScene(mScene);
}

void PhotoKitView::scaleView(ZoomAction zoom)
{
	if (zoom == ZoomIn) {
		mScale += 0.2;
		mScale = qMin(mScale, 2.0);
	} else {
		mScale -= 0.2;
		mScale = qMax(mScale, 0.2);
	}
	setTransform(QTransform()//.translate(contentsRect().width(), contentsRect().height())
			.scale(mScale , mScale)
			//.translate(contentsRect().width(), contentsRect().height())
		);
	//centerOn(center);
}
//TODO: Easing. QVariantAnimation
void PhotoKitView::mouseMoveEvent(QMouseEvent *e)
{
	QPoint delta = e->pos() - mMousePos;
	//qDebug("%d", delta.x()); //not const. delta.x() is the max value of Easing
	if (mPressed) {
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
		verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
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

void PhotoKitView::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;
	//qDebug("wheel steps: %d", numSteps); //1
	if (numSteps > 0) {
		scaleView(ZoomIn);
	} else {
		scaleView(ZoomOut);
	}
	QGraphicsView::wheelEvent(event);
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

END_NAMESPACE_PHOTOKIT
