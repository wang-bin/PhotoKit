#include "Button.h"

#include <QtGui/QPainter>

#include "OutlineGlowItem.h"
/*hover/press to highlight*/

namespace PhotoKit {

Button::Button(const QString& text, QGraphicsItem * parent, Qt::WindowFlags wFlags) :
    QGraphicsWidget(parent, wFlags),mText(text),mOn(false)
{
    prepairBackgrounds();
}

Button::Button(const QPixmap& icon, QGraphicsItem * parent, Qt::WindowFlags wFlags) :
    QGraphicsWidget(parent, wFlags),mBg(icon),mOn(false)
{
    prepairBackgrounds();
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	//qDebug("size: %fx%f", rect().width(), rect().height());

    if (mOn) {
        qDebug("on");
        painter->drawPixmap(rect(), mBgHiglight, mBgHiglight.rect());
    }else
        painter->drawPixmap(rect(), mBg, mBg.rect());
}


void Button::prepairBackgrounds()
{
    QLinearGradient g(QPointF(), rect().bottomLeft());
    g.setColorAt(0, QColor(255,255,255, 168));
    g.setColorAt(1, QColor(33,33,33, 234));
    g.setColorAt(0.5, QColor(22,22,22, 255));
    QPainter painter(&mBg);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(mBg.rect(), g);
    painter.end();
    mBgHiglight = QPixmap(mBg.size());
    mBgHiglight.fill(QColor(44, 222, 22, 0));
    QPainter painter1(&mBgHiglight);
    painter1.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter1.drawPixmap(QPoint(), mBg);
    mGlow = new OutlineGlowItem(this);
    //mGlow->setZValue(zValue() + 1);
    QSizeF s = boundingRect().size();
    mGlow->setSize(QSize((int)s.width(), (int)s.height()));
    mGlow->setGlowWidth(8);
    mGlow->setColor(QColor(Qt::green).lighter(134));
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    mOn = true;
    qDebug("button press");
    mGlow->show();
    update();
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    mOn = false;
    mGlow->hide();
}

} //namespace PhotoKit
