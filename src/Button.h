#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsWidget>

namespace PhotoKit {
class OutlineGlowItem;
class Button : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Button(const QString& text, QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    explicit Button(const QPixmap& text, QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //virtual void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


protected:
    void prepairBackgrounds();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    bool mOn;
    QString mText;
    QPixmap mBg, mBgHiglight;
    OutlineGlowItem *mGlow;
};

} //namespace PhotoKit
#endif // BUTTON_H
