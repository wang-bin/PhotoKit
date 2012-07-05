/******************************************************************************
    DemoItemAnimation.h: description
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


#ifndef DEMOITEMANIMATION_H
#define DEMOITEMANIMATION_H

#include <QGraphicsItemAnimation>
#include <QPointF>
namespace PhotoKit {
class BaseItem;
class DemoItemAnimation : public QGraphicsItemAnimation
{
    Q_OBJECT

public:
    enum INOROUT {ANIM_IN, ANIM_OUT, ANIM_UNSPECIFIED};

    DemoItemAnimation(BaseItem *item, INOROUT inOrOut = ANIM_UNSPECIFIED, bool hideOnFinished = false);
    virtual ~DemoItemAnimation();

    virtual void play(bool fromStart = true, bool force = false);
    virtual void playReverse();
    virtual void stop(bool reset = true);
    virtual void setRepeat(int nr = 0);

    void setDuration(int duration);
    void setDuration(float duration){ setDuration(int(duration)); }
    void setOpacityAt0(qreal opacity);
    void setOpacityAt1(qreal opacity);
    void setOpacity(qreal step);
    void setCurrentTime(int ms);
    void setStartPos(const QPointF &pos);
    bool notOwnerOfItem();

    bool running();
    bool runningOrItemLocked();
    void lockItem(bool state);
    void prepare();

    BaseItem *baseAnimationItem();

    virtual void afterAnimationStep(qreal step); // overridden

    QTimeLine *timeline;
    qreal opacityAt0;
    qreal opacityAt1;
    int startDelay;
    QPointF startPos;
    bool hideOnFinished;
    bool moveOnPlay;
    bool forcePlay;
    bool fromStart;
    INOROUT inOrOut;

private slots:
    virtual void playWithoutDelay();
};
} //namespace PhotoKit
#endif // DEMOITEMANIMATION_H
