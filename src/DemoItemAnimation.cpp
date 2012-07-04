/******************************************************************************
    DemoItemAnimation.cpp: description
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


#include "DemoItemAnimation.h"

#include <QtCore/QTimeLine>
#include <QtCore/QTimer>
#include "BaseItem.h"
#include "Config.h"
#include "ezlog.h"
namespace PhotoKit {


DemoItemAnimation::DemoItemAnimation(BaseItem *item, INOROUT inOrOut)
{
    this->opacityAt0 = 1.0;
    this->opacityAt1 = 1.0;
    this->startDelay = 0;
    this->inOrOut = inOrOut;
    this->hideOnFinished = false;
    this->forcePlay = false;
    this->timeline = new QTimeLine(5000);
    this->timeline->setFrameRange(0, 2000);
    this->timeline->setUpdateInterval(int(1000.0/Config::fps));
    this->moveOnPlay = false;
    setTimeLine(this->timeline);
    setItem(item);
}

DemoItemAnimation::~DemoItemAnimation()
{
    // Do not delete demoitem. It is not
    // owned by an animation
    delete this->timeline;
}

void DemoItemAnimation::prepare()
{
    this->baseAnimationItem()->prepare();
}

void DemoItemAnimation::setStartPos(const QPointF &pos){
    this->startPos = pos;
}

void DemoItemAnimation::setDuration(int duration)
{
    duration = int(duration * Config::animSpeed);
    this->timeline->setDuration(duration);
    this->moveOnPlay = true;
}

void DemoItemAnimation::setCurrentTime(int ms)
{
    this->timeline->setCurrentTime(ms);
}

bool DemoItemAnimation::notOwnerOfItem()
{
    return this != baseAnimationItem()->currentAnimation;
}

void DemoItemAnimation::play(bool fromStart, bool force)
{
	this->fromStart = fromStart;
    this->forcePlay = force;

    QPointF currPos = this->baseAnimationItem()->pos();

    // If the item that this animation controls in currently under the
    // control of another animation, stop that animation first
    if (this->baseAnimationItem()->currentAnimation)
        this->baseAnimationItem()->currentAnimation->timeline->stop();
    this->baseAnimationItem()->currentAnimation = this;
    this->timeline->stop();

    if (Config::noAnimations && !this->forcePlay){
        this->timeline->setCurrentTime(1);
        this->baseAnimationItem()->setPos(this->posAt(1));
    }
    else{
        if (this->baseAnimationItem()->isVisible())
            // If the item is already visible, start the animation from
            // the items current position rather than from start.
            this->setPosAt(0.0, currPos);
        else
            this->setPosAt(0.0, this->startPos);

        if (this->fromStart){
            this->timeline->setCurrentTime(0);
            this->baseAnimationItem()->setPos(this->posAt(0));
        }
    }
	if (this->inOrOut == ANIM_IN)
        this->baseAnimationItem()->setRecursiveVisible(true);

    if (this->startDelay){
        QTimer::singleShot(this->startDelay, this, SLOT(playWithoutDelay()));
        return;
    }
    else
        this->playWithoutDelay();
}

void DemoItemAnimation::playWithoutDelay()
{
    if (this->moveOnPlay && !(Config::noAnimations && !this->forcePlay))
        this->timeline->start();
    this->baseAnimationItem()->animationStarted(this->inOrOut);
}

void DemoItemAnimation::stop(bool reset)
{
    this->timeline->stop();
    if (reset)
        this->baseAnimationItem()->setPos(this->posAt(0));
    if (this->hideOnFinished && !this->moveOnPlay)
        this->baseAnimationItem()->setRecursiveVisible(false);
    this->baseAnimationItem()->animationStopped(this->inOrOut);
}

void DemoItemAnimation::setRepeat(int nr)
{
    this->timeline->setLoopCount(nr);
}

void DemoItemAnimation::playReverse()
{
}

bool DemoItemAnimation::running()
{
    return (this->timeLine()->state() == QTimeLine::Running);
}

bool DemoItemAnimation::runningOrItemLocked()
{
    return (this->running() || this->baseAnimationItem()->locked);
}

void DemoItemAnimation::lockItem(bool state)
{
    this->baseAnimationItem()->locked = state;
}

BaseItem *DemoItemAnimation::baseAnimationItem()
{
    return (BaseItem *) this->item();
}

void DemoItemAnimation::setOpacityAt0(qreal opacity)
{
    this->opacityAt0 = opacity;
}

void DemoItemAnimation::setOpacityAt1(qreal opacity)
{
    this->opacityAt1 = opacity;
}

void DemoItemAnimation::setOpacity(qreal step)
{
    BaseItem *baseAnimationItem = (BaseItem *) item();
    baseAnimationItem->opacity = this->opacityAt0 + step * step * step * (this->opacityAt1 - this->opacityAt0);
}

void DemoItemAnimation::afterAnimationStep(qreal step)
{
    if (step == 1.0f){
        if (this->timeline->loopCount() > 0){
            // animation finished.
            if (this->hideOnFinished)
                this->baseAnimationItem()->setRecursiveVisible(false);
            this->baseAnimationItem()->animationStopped(this->inOrOut);
        }
    } else if (Config::noAnimations && !this->forcePlay){
        // The animation is not at end, but
        // the animations should not play, so go to end.
        this->setStep(1.0f); // will make this method being called recursive.
    }
}


} //namespace PhotoKit
