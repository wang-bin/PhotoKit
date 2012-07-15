/******************************************************************************
	ThumbItem: display thumbnail on the wall
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


#ifndef PHOTOKIT_THUMBITEM_H
#define PHOTOKIT_THUMBITEM_H

#include "PhotoKit_Global.h"
#include <QGraphicsObject>
#define NO_BASE
class QGraphicsItemAnimation;
class QNetworkReply;
class QNetworkAccessManager;
namespace PhotoKit {
class ItemAnimation;
class OutlineGlowItem;
class ThumbItem : public QGraphicsObject  //QGraphicsItem
{
	Q_OBJECT
public:
	enum ZoomAction { ZoomIn, ZoomOut};
	explicit ThumbItem(QGraphicsItem *parent = 0);
	explicit ThumbItem(const QImage& image, QGraphicsItem *parent = 0);
	~ThumbItem();

	void setOnlineImage(bool online);
	bool isOnlineImage() const;
	void setThumbPath(const QString& path);
    void setOriginImage(const QString& path);

	void resize(qreal width, qreal height);
	void resize(const QSizeF& size);
    qreal borderWidth() const;
    qreal marginWidth() const;
	QImage thumbImage() const; //origin thumb
	QImage scaledThumbImage() const; //thumb scaled to fit item size
	void setThumbImage(const QImage& image);
#ifdef NO_BASE
    virtual QRectF boundingRect() const; // overridden
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option = 0, QWidget *widget = 0);
#else

#endif //NO_BASE
    void showGlow();
	void hideGlow();
	void zoom(ZoomAction action);
	qreal boundingWidth() const;
	qreal boundingHeight() const;
	qreal contentWidth() const;
	qreal contentHeight() const;

signals:
	void loadFinished();

protected:
    void prepairSize();
    virtual QImage* createImage(const QMatrix &) const;

	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //virtual bool sceneEvent(QEvent *event);

private slots:
	void updateLoadProgress(qint64 value);
	void loadFinish(QNetworkReply* reply); //TODO: no param

private:
	bool mIsOnlineImage;
	QNetworkAccessManager *mNetwork;
	QString thumb_path, origin_image_path;
	QImage thumb;
	//QImage origin;
	OutlineGlowItem *mGlow;
	ItemAnimation *mItemAnimation;
	qreal mWidth, mHeight; //scaled thumb width, height
    int maxWidth;
    int maxHeight;
	//ProgressItem
	//TextItem *name
    bool adjustSize;
    float scale;
};

} //namespace PhotoKit
#endif // PHOTOKIT_THUMBITEM_H
