/******************************************************************************
	NextEffect_Enter: Fly in effect.
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

#include "nexteffect_enter.h"
#include "nexteffectfactory.h"


REGISTER_EFFECT(EnterFromBottom, Enter)
REGISTER_EFFECT(EnterFromBottomLeft, Enter)
REGISTER_EFFECT(EnterFromBottomRight, Enter)
REGISTER_EFFECT(EnterFromLeft, Enter)
REGISTER_EFFECT(EnterFromRight, Enter)
REGISTER_EFFECT(EnterFromTop, Enter)
REGISTER_EFFECT(EnterFromTopLeft, Enter)
REGISTER_EFFECT(EnterFromTopRight, Enter)

bool NextEffect_Enter::prepare()
{
	NextEffect::prepare(); //Important!!!
	if (effect_type == EnterFromBottom)
		calculateRegion_ptr = &NextEffect_Enter::calculateRegion_FromBottom;
	else if (effect_type == EnterFromBottomLeft)
		calculateRegion_ptr = &NextEffect_Enter::calculateRegion_FromBottomLeft;
	else if (effect_type == EnterFromBottomRight)
		calculateRegion_ptr = &NextEffect_Enter::calculateRegion_FromBottomRight;
	else if (effect_type == EnterFromLeft)
		calculateRegion_ptr = &NextEffect_Enter::calculateRegion_FromLeft;
	else if (effect_type == EnterFromRight)
		calculateRegion_ptr = &NextEffect_Enter::calculateRegion_FromRight;
	else if (effect_type == EnterFromTop)
		calculateRegion_ptr = &NextEffect_Enter::calculateRegion_FromTop;
	else if (effect_type == EnterFromTopLeft)
		calculateRegion_ptr = &NextEffect_Enter::calculateRegion_FromTopLeft;
	else if (effect_type == EnterFromTopRight)
		calculateRegion_ptr = &NextEffect_Enter::calculateRegion_FromTopRight;
	else
		calculateRegion_ptr = &NextEffect_Enter::calculateRegion_FromTop; //calculateRegion_Random

	return true;
}

bool NextEffect_Enter::prepareFrameAt(int frame)
{
	if (isEndFrame(frame))
		return false;
	(this->*calculateRegion_ptr)(k);
	current_clip_region = QRegion(0, 0, width, height) - next_clip_region;
	return true;
}

void NextEffect_Enter::calculateRegion_FromBottom(qreal k)
{
	next_clip_region = QRegion(0, height*(1.0-k), width, height*k);
	next_rect = QRect(0, height*(k-1.0), width, height);
}

/*
 ?...................
  .    next_rect    .
  .                 .
  +++++++............
  +     +           .
  +     +           .
  +++++++............
*/
void NextEffect_Enter::calculateRegion_FromBottomLeft(qreal k)
{
	next_clip_region = QRegion(0, height*(1.0-k), width*k, height*k);
	next_rect = QRect(width*(1.0-k), height*(k-1.0), width, height);
}

/*
  ...................
  .    next_rect    .
  .          0+++++++
  .           +     +
  .           +     +
  ............+++++++
*/
void NextEffect_Enter::calculateRegion_FromBottomRight(qreal k)
{
	next_clip_region = QRegion(width*(1.0-k), height*(1.0-k), width*k, height*k);
	next_rect = QRect(width*(k-1.0), height*(k-1.0), width, height);

}

void NextEffect_Enter::calculateRegion_FromLeft(qreal k)
{
	next_clip_region = QRegion(0, 0, width*k, height);
	next_rect = QRect(width*(1.0-k), 0, width, height);
}


void NextEffect_Enter::calculateRegion_FromRight(qreal k)
{
	next_clip_region = QRegion(width*(1.0-k), 0, width*k, height);
	next_rect = QRect(width*(k-1.0), 0, width, height);
}

void NextEffect_Enter::calculateRegion_FromTop(qreal k)
{
	next_clip_region = QRegion(0, 0, width, height*k);
	next_rect = QRect(0, height*(1.0-k), width, height);
}

/*
  +++++++............
  +     +           .
  +     +           .
  +++++++           .
  .                 .
  .    next_rect    .
  ...................
*/
void NextEffect_Enter::calculateRegion_FromTopLeft(qreal k)
{
	next_clip_region = QRegion(0, 0, width*k, height*k);
	next_rect = QRect(width*(1.0-k), height*(1.0-k), width, height);
}

/*
 ?............+++++++
  .           +     +
  .           +     +
  .           +++++++
  .                 .
  .    next_rect    .
  ...................
*/
void NextEffect_Enter::calculateRegion_FromTopRight(qreal k)
{
	next_clip_region = QRegion(width*(1.0-k), 0, width*k, height*k);
	next_rect = QRect(width*(k-1.0), height*(1.0-k), width, height);
}
