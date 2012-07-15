/******************************************************************************
	ReflectEffectItem: reflection effect for a ThumbItem
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


#ifndef PHOTOKIT_REFLECTEFFECTITEM_H
#define PHOTOKIT_REFLECTEFFECTITEM_H

/*!
  TODO: subclass of QGraphicsEffect
*/
#include <QGraphicsObject>
class QGraphicsBlurEffect;
namespace PhotoKit {

class ThumbItem;
class ReflectEffectItem : public QGraphicsObject
{
	Q_OBJECT
public:
	//TODO: MirrorLeft and MirrorTop
	/*MirrorBottom and MirrorRight can be directly installed to an item without special settings*/
    enum MirrorDirection { MirrorLeft, MirrorTop, MirrorRight, MirrorBottom};
	ReflectEffectItem(ThumbItem* target, MirrorDirection direction = MirrorBottom);
    ~ReflectEffectItem();

    MirrorDirection mirrorDirection() const;
    void setMirrorDirection(MirrorDirection direction);

    int mirrorDistance() const;
    void setMirrorDistance(int distance);

    bool blurEnabled() const;
    void enableBlur(bool enable = true);

    bool gradientEnabled() const;
    void enableGradient(bool enable);

    virtual QRectF boundingRect() const;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option = 0, QWidget *widget = 0);
    void drawReflect(); //createImage

protected slots:
	void updateSourceReflect();

private:
	bool mSourceAvailable;
    bool mBlur, mGradient;
    QGraphicsBlurEffect *mBlurEffect;
    ThumbItem *mTarget;
    MirrorDirection mDirection;
    int mMirrorDistance;
    QImage *mReflect;
    QPointF mPos;
};

} //namespace PhotoKit
#endif // PHOTOKIT_REFLECTEFFECTITEM_H
