/******************************************************************************
	NextEffect_Center: Open from or close to center effect.
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

#include "nexteffect_center.h"
#include <cmath>
#include <QtGui/QMatrix>
#include "nexteffectfactory.h"

REGISTER_EFFECT(EllipseOpen, Center)
REGISTER_EFFECT(EllipseClose, Center)

bool NextEffect_Center::prepare()
{
	NextEffect::prepare(); //Important!!!
	if (effect_type==EllipseOpen)
		func = &NextEffect_Center::prepareEllipseFromCenterFrameAt;
	else
		func = &NextEffect_Center::prepareEllipseToCenterFrameAt;

	current_clip_region = QRegion();
	current_rect = QRect();
	//frame_image = current_image; //?
	return true;
}

bool NextEffect_Center::prepareFrameAt(int frame)
{
	return (this->*func)(frame);
}

bool NextEffect_Center::isEndFrame(int frame)
{
	//current_clip_region.isEmpty() is true at first frame
	if (current_clip_region.isEmpty() && current_frame>1)
	//if (frame>frames_total)
		return true;
	current_frame = frame;
	k = (qreal)current_frame/(qreal)frames_total*3.1416*speed; //*3.1416 to speed up
	return false;
}

bool NextEffect_Center::prepareEllipseFromCenterFrameAt(int frame)
{
	if (isEndFrame(frame))
		return false;
	//next_clip_region = QRegion(width*(1.0-k)*0.5, height*(1.0-k)*0.5, width*k, height*k, QRegion::Ellipse);
	//current_clip_region = QRegion(0, 0, width, height) - next_clip_region;
	QRegion region = QRegion(width*(1.0-k)*0.5, height*(1.0-k)*0.5, width*k, height*k, QRegion::Ellipse);
	next_clip_region = region - next_clip_region;
	current_clip_region = QRegion(0, 0, width, height) - region;
	return true;
}


bool NextEffect_Center::prepareEllipseToCenterFrameAt(int frame)
{
	if (current_frame<2) {
		int r = sqrt(qreal(width*width+height*height));
		current_clip_region = QRegion(width/2-r, height/2-r, 2*r, 2*r);
	}

	//current_clip_region.isEmpty() is true at first frame
	if (current_clip_region.isEmpty() || current_frame>frames_total)
	//if (frame>frames_total)
		return false;
	current_frame = frame;
	qreal k = (qreal)current_frame/(qreal)frames_total;
	QMatrix matrix;
	matrix.scale(1.0-k, 1.0-k);
	QPoint p = current_clip_region.boundingRect().center()-matrix.map(current_clip_region).boundingRect().center();
	matrix.translate(p.x(), p.y());
	current_clip_region = matrix.map(current_clip_region);
	next_clip_region = QRegion(0, 0, width, height) - current_clip_region;
	return true;
}
