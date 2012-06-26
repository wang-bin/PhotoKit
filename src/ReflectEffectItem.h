#ifndef REFLECTEFFECTITEM_H
#define REFLECTEFFECTITEM_H

#include <QGraphicsItem>
class QGraphicsBlurEffect;
namespace PhotoKit {

class ThumbItem;
class ReflectEffectItem : public QGraphicsItem
{
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
    void drawReflect();

private:
    bool mBlur, mGradient;
    QGraphicsBlurEffect *mBlurEffect;
    ThumbItem *mTarget;
    MirrorDirection mDirection;
    int mMirrorDistance;
    QImage *mReflect;
    QPointF mPos;
};

} //namespace PhotoKit
#endif // REFLECTEFFECTITEM_H
