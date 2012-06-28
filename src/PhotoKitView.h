/******************************************************************************
	PhotoKitView.h: description
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


#ifndef PHOTOKITVIEW_H
#define PHOTOKITVIEW_H

#include "PhotoKit_Global.h"

//#include <QtCore/QTime>
#include <QGraphicsView>

namespace PhotoKit {
class TransformMachine;
class PhotoKitScene;
class PhotoKitView : public QGraphicsView
{
	Q_OBJECT
public:
	enum ZoomAction { ZoomIn, ZoomOut};
	explicit PhotoKitView(QWidget *parent = 0);
    QRectF visibleSceneRect() const;
    void transformThumbs(qreal x, qreal y, qreal scale, qreal xShear, qreal yShear, qreal maxRotation);//AXix may not be x, y or z
signals:
	
public slots:
	void scaleView(ZoomAction zoom);

protected:
	void scaleWithAnimation(ZoomAction zoom);
	void moveWithAnimation(qreal dx, qreal dy);
	//TODO: multiTouch
	//virtual bool event(QEvent *event);
    /*not move to scene. scene may be transformed*/
	virtual void dragMoveEvent(QDragMoveEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event); //TODO:test rotate
	virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
	virtual bool viewportEvent(QEvent *event);

private slots:
	void doTransform(const QMatrix& m);
private:
	void setRenderingSystem();

	bool mPressed;
	PhotoKitScene *mScene;
	qreal mScale;
	QPoint mMousePos;
	//QTime mPressTime; //not required. use movement distance is enough

	TransformMachine *mMachine;
};

} //namespace PhotoKit

#endif // PHOTOKITVIEW_H
