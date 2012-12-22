/******************************************************************************
	PhotoKitView: PhotoKit's view
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


#ifndef PHOTOKIT_PHOTOKITVIEW_H
#define PHOTOKIT_PHOTOKITVIEW_H

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
	bool canTransform() const;
	void setCanTransform(bool can);
    QRectF visibleSceneRect() const;
	void setAnimationDuration(int ms);
    void setInitialPos(qreal x, qreal y);
	void smartTransform(qreal x, qreal y, qreal scale0, qreal scale, qreal xRot, qreal yRot, qreal zRot, qreal xShear, qreal yShear);
	void smoothScale(ZoomAction zoom, qreal dRatio_abs = 0.1);
signals:

protected:
	//TODO: add smartTransform(qreal x, qreal y, qreal scale);
	//TODO: multiTouch
	//virtual bool event(QEvent *event);
    /*not move to scene. scene may be transformed*/
	//virtual void dragMoveEvent(QDragMoveEvent *event);
	virtual void contextMenuEvent(QContextMenuEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event); //TODO:test rotate
	virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
	virtual bool viewportEvent(QEvent *event);

private slots:
	void doTransform(const QTransform& m);
private:
	void setRenderingSystem();

	bool mPressed;
	PhotoKitScene *mScene;
	/*qreal &mScale;
	qreal mScaleS, mScaleT; //search page, thumb page
	//qreal mXRot, mYRot, mZRot;
	qreal &mX, &mY;
	qreal mXS, mYS, mXT, mYT; //search page
	//qreal mHShear, mVShear;*/
	QPointF mMousePos;
	//QTime mPressTime; //not required. use movement distance is enough

	TransformMachine *mMachine;
	bool mCanTransform;
	bool mZoomOnMove;
};

} //namespace PhotoKit

#endif // PHOTOKIT_PHOTOKITVIEW_H
