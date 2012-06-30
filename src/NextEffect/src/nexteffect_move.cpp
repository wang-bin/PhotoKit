/******************************************************************************
	NextEffect_Move: Move together effect.
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

#include "nexteffect_move.h"
#include "nexteffectfactory.h"

REGISTER_EFFECT(MoveFromBottom, Move)
REGISTER_EFFECT(MoveFromLeft, Move)
REGISTER_EFFECT(MoveFromRight, Move)
REGISTER_EFFECT(MoveFromTop, Move)

bool NextEffect_Move::prepare()
{
	NextEffect::prepare(); //Important!!!
	if (effect_type == MoveFromBottom)
		calculateRegion_ptr = &NextEffect_Move::calculateRegion_FromBottom;
	else if (effect_type == MoveFromLeft)
		calculateRegion_ptr = &NextEffect_Move::calculateRegion_FromLeft;
	else if (effect_type == MoveFromRight)
		calculateRegion_ptr = &NextEffect_Move::calculateRegion_FromRight;
	else if (effect_type == MoveFromTop)
		calculateRegion_ptr = &NextEffect_Move::calculateRegion_FromTop;
	else
		calculateRegion_ptr = &NextEffect_Move::calculateRegion_FromRight;
		//calculateRegion_ptr = &NextEffect_Move::calculateRegion_Random;
	return true;
}

bool NextEffect_Move::prepareFrameAt(int frame)
{
	if (isEndFrame(frame))
		return false;
	(this->*calculateRegion_ptr)(k);
	current_clip_region = QRegion(0, 0, width, height) - next_clip_region;
	return true;
}

void NextEffect_Move::calculateRegion_FromBottom(qreal k)
{
	next_clip_region = QRegion(0, height*(1.0-k), width, height*k);
	next_rect = QRect(0, (k-1.0)*height, width, height);
	current_rect = QRect(0, k*height, width, height);
}

void NextEffect_Move::calculateRegion_FromLeft(qreal k)
{
	next_clip_region = QRegion(0, 0, k*width, height);
	next_rect = QRect((1.0-k)*width, 0, width, height);
	current_rect = QRect(-k*width, 0, width, height);
}

void NextEffect_Move::calculateRegion_FromRight(qreal k)
{
	next_clip_region = QRegion((1.0-k)*width, 0, k*width, height);
	next_rect = QRect((k-1.0)*width, 0, width, height);
	current_rect = QRect(k*width, 0, width, height);
}

void NextEffect_Move::calculateRegion_FromTop(qreal k)
{
	next_clip_region = QRegion(0, 0, width, k*height);
	next_rect = QRect(0, (1.0-k)*height, width, height);
	current_rect = QRect(0, -k*height, width, height);
}
