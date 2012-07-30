#include "ToolBar.h"
#include <QtGui/QPainter>


namespace PhotoKit {

ToolBar::ToolBar(QGraphicsItem * parent, Qt::WindowFlags wFlags) :
    QGraphicsWidget(parent, wFlags)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations); //auto ignore transform?
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsPanel);
    setZValue(9); //auto ignore transform
    //setMinimumWidth(qApp->desktop()->rect().width());
    //setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
}

void ToolBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QLinearGradient g(QPointF(), rect().bottomLeft());
    g.setColorAt(0, QColor(255,255,255, 168));
    g.setColorAt(1, QColor(33,33,33, 168));
    g.setColorAt(0.5, QColor(22,22,22, 168));
    painter->fillRect(rect(), g);
}


QVariant ToolBar::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        qDebug("pos change: (%f, %f)", value.toPointF().x(), value.toPointF().y());
    } else if (change == QGraphicsItem::ItemPositionChange) {
        qDebug("pos changed:  (%f, %f)", value.toPointF().x(), value.toPointF().y());
    }
    return value;
}

} //namespace PhotoKit
