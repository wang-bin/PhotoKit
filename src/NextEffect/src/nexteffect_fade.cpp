/******************************************************************************
	NextEffect_Fade: Fade effect.
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

#include "nexteffect_fade.h"
#include <qpainter.h>
#include "nexteffectfactory.h"

REGISTER_EFFECT(Fade, Fade)

NextEffect_Fade::NextEffect_Fade()
	:NextEffect()
{
	effect_type = Fade;
}

bool NextEffect_Fade::prepare()
{
	NextEffect::prepare();
	next_clip_region = QRegion();
	current_clip_region = QRegion(0, 0, width, height);// - next_clip_region;
	next_rect = QRect();
	if (frame_image) {
		qDebug("delete frame_image");
		delete frame_image;
	}
	frame_image = current_image;
	return true;
}

bool NextEffect_Fade::prepareFrameAt(int frame)
{
	if (isEndFrame(frame)) {
		current_image = 0;  //avoid delete current_image twice and ensure frame_image not empty.
		return false;
	}
	return true;
}

void NextEffect_Fade::renderBackground()
{
#if QT_VERSION > 0x030000
	QPainter p(current_image);
	p.setOpacity(k); //use const
	p.drawImage(0, 0, *next_image);
#else

#endif
}
