/******************************************************************************
	NextEffect: Base class of all effect classes.
	Copyright (C) 2011 Wang Bin <wbsecg1@gmail.com>

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

#ifndef NEXTEFFECT_H
#define NEXTEFFECT_H

#include <qimage.h>
#include "nexteffect_global.h"

typedef int EffectId;

const EffectId Invalid = -2;
const EffectId Random = -1;
/*!
  How to use:
  1) Display on your widget
  call NextEffectFactory::instance().createEffect() or NextEffectFactory::instance().getRandomEffect() to get
  an effect, then set pixmaps using setInitialPixmaps() and other optinal parameter. After the foreplay,
  call prepare();

  start widget's effect timer. In it's timerEvent, add
	if (!mEffect->prepareNextFrame()) {
		return;
	}
  update();
  in widget's painterEvent, add
	QPainter painter(this);
	painter.drawPixmap(rect(), *mEffect->currentFrame());

  2) Add your own effect
  Create a subclass of NextEffect, usually reimplement prepareFrameAt(int) is enough.
  If you reimplement prepare(), don't forget to call NextEffect::prepare().
  Add new EffectId(s) in your new header.
  call REGIST_EFFECT(ID, MAINTYPE) in your new cpp.
*/

#if QT_VERSION < 0x040000
#define double qreal;
#endif

class Q_EXPORT NextEffect
{
public:
	NextEffect();
	virtual ~NextEffect();

	virtual bool prepare(); //after all parameter setted and before effect start
/*!
	false: no more frames
*/
	bool prepareNextFrame(); //No virtual?
	virtual bool prepareFrameAt(int frame) = 0;


	QImage *currentFrame() const {return frame_image;}
	void setType(EffectId type) {effect_type = type;}
	EffectId type() const {return effect_type;}
/*!
  Some class may have several effect types.
*/
	virtual bool hasType(EffectId t) const {return effect_type==t;}

	void setSpeed(qreal s) {speed = s;}
	void setFrames(int frames) {frames_total = frames;}
	int currentFrameNumber() const {return current_frame;}
	int frames() const {return frames_total;}

	void setInitialPixmaps(const QImage& pixmapCurrent, const QImage& pixmapNext);
/*!
	set images' size. fill black background
*/
	void setSize(const QSize& s);
	QSize size() const {return QSize(width, height);}

protected:
	virtual bool isEndFrame(int frame);
	virtual void renderBackground();

protected:
	bool finished;
	qreal k; //
	qreal speed; //>1.0
	EffectId effect_type;
	int frames_total, current_frame;
	//if current_image is null, just paint next_image inside
	QImage *frame_image, *current_image, *next_image;
	int width, height;
	//clip region of currentFrame() to paint current and next frame_image
	QRegion current_clip_region, next_clip_region;

/*
	rect of current and frame_image to be paint. The size is always the frame_image's size.
	when calling drawPixmap(const QRect& target, const QPixmap& frame_image, const QRect& source),
	if rect not equals target's rect, then the frame_image will be scaled to target's rect.
	Here, target's rect is it's rect()
*/
	QRect current_rect, next_rect;

};

#define DECLARE_NEXT_EFFECT(T) \
	class Q_EXPORT NextEffect_##T : public NextEffect { \
	public: \
	NextEffect_##T(); \
	virtual bool prepareFrameAt(int frame); \
	};


#endif // NEXTEFFECT_H
