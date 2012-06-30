/******************************************************************************
	NextEffect_Blinds: Blinds effect.
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

#include "nexteffect_blinds.h"
#include <time.h>
#include "nexteffectfactory.h"

REGISTER_EFFECT(BlindsBottomToTop, Blinds)
REGISTER_EFFECT(BlindsLeftToRight, Blinds)
REGISTER_EFFECT(BlindsRightToLeft, Blinds)
REGISTER_EFFECT(BlindsTopToBottom, Blinds)

bool NextEffect_Blinds::prepare()
{
	NextEffect::prepare(); //Important!!!
	if (effect_type == BlindsBottomToTop)
		calculateRegion_ptr = &NextEffect_Blinds::calculateRegion_BottomToTop;
	else if (effect_type == BlindsLeftToRight)
		calculateRegion_ptr = &NextEffect_Blinds::calculateRegion_LeftToRight;
	else if (effect_type == BlindsRightToLeft)
		calculateRegion_ptr = &NextEffect_Blinds::calculateRegion_RightToLeft;
	else if (effect_type == BlindsTopToBottom)
		calculateRegion_ptr = &NextEffect_Blinds::calculateRegion_TopToBottom;
	else
		calculateRegion_ptr = &NextEffect_Blinds::calculateRegion_TopToBottom; //calculateRegion_Random

	qsrand(time(0));
	leafs = qrand()%13 + 2;
	qDebug("leafs=%d", leafs);
	if (effect_type==BlindsRightToLeft || effect_type==BlindsRightToLeft) {
		leaf_width = width/leafs + 1; //Must plus 1! Or the final frame_image may have blanks
	} else {
		leaf_width = height/leafs + 1;
	}

	return true;
}

bool NextEffect_Blinds::prepareFrameAt(int frame)
{
	if (isEndFrame(frame))
		return false;
	(this->*calculateRegion_ptr)(k);
	current_clip_region = QRegion(0, 0, width, height) - next_clip_region;
	return true;
}

void NextEffect_Blinds::calculateRegion_BottomToTop(qreal k)
{
	next_clip_region = QRegion();
	for (int i=0;i<leafs;next_clip_region += QRegion(0, (i+++1.0-k)*leaf_width, width, leaf_width*k));
}

//Have some bugs! Pictures are not show completely
void NextEffect_Blinds::calculateRegion_LeftToRight(qreal k)
{
	next_clip_region = QRegion();
	for (int i=0;i<leafs;next_clip_region += QRegion(i++*leaf_width, 0, leaf_width*k, height));
}

void NextEffect_Blinds::calculateRegion_RightToLeft(qreal k)
{
	next_clip_region = QRegion();
	for (int i=0;i<leafs;next_clip_region += QRegion((i+++1.0-k)*leaf_width, 0, leaf_width*k, height));
}

void NextEffect_Blinds::calculateRegion_TopToBottom(qreal k)
{
	next_clip_region = QRegion();
	for (int i=0;i<leafs;next_clip_region += QRegion(0, i++*leaf_width, width, leaf_width*k));
}
