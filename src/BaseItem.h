/******************************************************************************
	BaseItem: animation supported item. Many code are from qtdemo
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


#ifndef PHOTOKIT_BASEITEM_H
#define PHOTOKIT_BASEITEM_H

#include <QGraphicsItem>

namespace PhotoKit {


class SharedImage
{
public:
	SharedImage() : refCount(0), image(0), pixmap(0){}
	~SharedImage()
	{
		delete image;
		delete pixmap;
	}

	int refCount;
	QImage *image;
	QPixmap *pixmap;
	QMatrix matrix;
	QRectF unscaledBoundingRect;
};

class DemoItemAnimation;
class Guide;
class ItemAnimation;
class BaseItem : public QGraphicsItem
{
public:
    BaseItem(QGraphicsItem *parent = 0);
    virtual ~BaseItem();
/*
    qreal boundingWidth() const;
    qreal boundingHeight() const;
*/

    bool inTransition();
    virtual void animationStarted(int id = 0){ Q_UNUSED(id); }
    virtual void animationStopped(int id = 0){ Q_UNUSED(id); }
    virtual void prepare(){}
    void setRecursiveVisible(bool visible);
    void useSharedImage(const QString &hashKey);
    void setNeverVisible(bool never = true);
    static void setMatrix(const QMatrix &matrix);
    QMatrix currentMatrix() const {return matrix;}
    virtual QRectF boundingRect() const; // overridden
    void setPosUsingSheepDog(const QPointF &dest, const QRectF &sceneFence);

    qreal opacity;
    bool locked;
    DemoItemAnimation *currentAnimation;
    bool noSubPixeling;

    // Used if controlled by a guide:
    void useGuide(Guide *guide, float startFrame = 0);
    void guideAdvance(float distance);
    void guideMove(float moveSpeed);
    void setGuidedPos(const QPointF &position);
    QPointF getGuidedPos();
    float startFrame;
    float guideFrame;
    Guide *currGuide;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option = 0, QWidget *widget = 0); // overridden
    virtual QImage *createImage(const QMatrix &) const { return 0; }
    virtual bool collidesWithItem(const QGraphicsItem *, Qt::ItemSelectionMode) const { return false; }
    bool prepared;

	//virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
	//virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    SharedImage *sharedImage;
    QString hashKey;
    bool neverVisible;
    bool validateImage();

    // Used if controlled by a guide:
    void switchGuide(Guide *guide);
    friend class Guide;
    QPointF guidedPos;

    // The next static hash is shared amongst all demo items, and
    // has the purpose of reusing images to save memory and time
    static QHash<QString, SharedImage *> sharedImageHash;
    static QMatrix matrix;
    ItemAnimation *mItemAnimation;
	friend class PhotoKitView;
	qreal mScale;
	qreal mX, mY;
};

} //namespace PhotoKit
#endif // PHOTOKIT_BASEITEM_H
