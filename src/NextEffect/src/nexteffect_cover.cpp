/******************************************************************************
	NextEffect_Cover: Cover the current without moving effect.
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

#include "nexteffect_cover.h"
#include "nexteffectfactory.h"

REGISTER_EFFECT(CoverFromLeft, Cover)
REGISTER_EFFECT(CoverFromRight, Cover)
REGISTER_EFFECT(CoverFromBottom, Cover)
REGISTER_EFFECT(CoverFromBottomLeft, Cover)
REGISTER_EFFECT(CoverFromBottomRight, Cover)
REGISTER_EFFECT(CoverFromTop, Cover)
REGISTER_EFFECT(CoverFromTopLeft, Cover)
REGISTER_EFFECT(CoverFromTopRight, Cover)

bool NextEffect_Cover::prepareFrameAt(int frame)
{
	if (isEndFrame(frame))
		return false;


	next_clip_region = QRegion(width*(1.0-k)*(effect_type==CoverFromTopRight||effect_type==CoverFromBottomRight||effect_type==CoverFromRight)
							   , height*(1.0-k)*(effect_type==CoverFromBottom||effect_type==CoverFromBottomLeft||effect_type==CoverFromBottomRight)
							   , width*((effect_type==CoverFromBottom||effect_type==CoverFromTop)?1:k)
							   , height*((effect_type==CoverFromLeft||effect_type==CoverFromRight)?1:k));
	current_clip_region = QRegion(0, 0, width, height) - next_clip_region;

	return true;
}
