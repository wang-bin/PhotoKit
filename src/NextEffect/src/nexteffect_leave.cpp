/******************************************************************************
	NextEffect_Leave: Leave away effect.
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

#include "nexteffect_leave.h"
#include "nexteffectfactory.h"

REGISTER_EFFECT(LeaveToBottom, Leave)
REGISTER_EFFECT(LeaveToBottomLeft, Leave)
REGISTER_EFFECT(LeaveToBottomRight, Leave)
REGISTER_EFFECT(LeaveToLeft, Leave)
REGISTER_EFFECT(LeaveToRight, Leave)
REGISTER_EFFECT(LeaveToTop, Leave)
REGISTER_EFFECT(LeaveToTopLeft, Leave)
REGISTER_EFFECT(LeaveToTopRight, Leave)

bool NextEffect_Leave::prepare()
{
	NextEffect::prepare(); //Important!!!
	if (effect_type == LeaveToBottom)
		calculateRegion_ptr = &NextEffect_Leave::calculateRegion_ToBottom;
	else if (effect_type == LeaveToBottomLeft)
		calculateRegion_ptr = &NextEffect_Leave::calculateRegion_ToBottomLeft;
	else if (effect_type == LeaveToBottomRight)
		calculateRegion_ptr = &NextEffect_Leave::calculateRegion_ToBottomRight;
	else if (effect_type == LeaveToLeft)
		calculateRegion_ptr = &NextEffect_Leave::calculateRegion_ToLeft;
	else if (effect_type == LeaveToRight)
		calculateRegion_ptr = &NextEffect_Leave::calculateRegion_ToRight;
	else if (effect_type == LeaveToTop)
		calculateRegion_ptr = &NextEffect_Leave::calculateRegion_ToTop;
	else if (effect_type == LeaveToTopLeft)
		calculateRegion_ptr = &NextEffect_Leave::calculateRegion_ToTopLeft;
	else if (effect_type == LeaveToTopRight)
		calculateRegion_ptr = &NextEffect_Leave::calculateRegion_ToTopRight;
	else
		calculateRegion_ptr = &NextEffect_Leave::calculateRegion_ToTopRight;
		//calculateRegion_ptr = &NextEffect_Leave::calculateRegion_Radom;

	return true;
}

bool NextEffect_Leave::prepareFrameAt(int frame)
{
	if (isEndFrame(frame))
		return false;
	(this->*calculateRegion_ptr)(k);
	next_clip_region = QRegion(0, 0, width, height) - current_clip_region;

	return true;
}

void NextEffect_Leave::calculateRegion_ToBottom(qreal k)
{
	current_clip_region = QRegion(0, k*height, width, (1.0-k)*height);
	current_rect = QRect(0, -k*height, width, height);
}

void NextEffect_Leave::calculateRegion_ToBottomLeft(qreal k)
{
	current_clip_region = QRegion(0, k*height, (1.0-k)*width, (1.0-k)*height);
	current_rect = QRect(k*width, -k*height, width, height);
}

void NextEffect_Leave::calculateRegion_ToBottomRight(qreal k)
{
	current_clip_region = QRegion(k*width, k*height, (1.0-k)*width, (1.0-k)*height);
	current_rect = QRect(-k*width, -k*height, width, height);
}

void NextEffect_Leave::calculateRegion_ToLeft(qreal k)
{
	current_clip_region = QRegion(0, 0, (1.0-k)*width, height);
	current_rect = QRect(k*width, 0, width, height);
}

void NextEffect_Leave::calculateRegion_ToRight(qreal k)
{
	current_clip_region = QRegion(k*width, 0, (1.0-k)*width, height);
	current_rect = QRect(-k*width, 0, width, height);
}

void NextEffect_Leave::calculateRegion_ToTop(qreal k)
{
	current_clip_region = QRegion(0, 0, width, (1.0-k)*height);
	current_rect = QRect(0, k*height, width, height);
}

void NextEffect_Leave::calculateRegion_ToTopLeft(qreal k)
{
	current_clip_region = QRegion(0, 0, (1.0-k)*width, (1.0-k)*height);
	current_rect = QRect(k*width, k*height, width, height);
}

void NextEffect_Leave::calculateRegion_ToTopRight(qreal k)
{
	current_clip_region = QRegion(k*width, 0, (1.0-k)*width, (1.0-k)*height);
	current_rect = QRect(-k*width, k*height, width, height);
}
