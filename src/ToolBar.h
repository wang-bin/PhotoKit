#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QGraphicsWidget>

namespace PhotoKit {

class ToolBar : public QGraphicsWidget
{
    Q_OBJECT
public:
    //enum direction
    explicit ToolBar(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    
public slots:
    
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

};

} //namespace PhotoKit
#endif // TOOLBAR_H
