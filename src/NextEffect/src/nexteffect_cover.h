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

#ifndef NEXTEFFECT_COVER_H
#define NEXTEFFECT_COVER_H

#include "nexteffect.h"

static const EffectId CoverFromTop = 7, CoverFromBottom = CoverFromTop + 1
			, CoverFromLeft = CoverFromBottom + 1, CoverFromRight = CoverFromLeft + 1
			, CoverFromTopLeft = CoverFromRight + 1, CoverFromBottomLeft = CoverFromTopLeft + 1
			, CoverFromTopRight = CoverFromBottomLeft + 1, CoverFromBottomRight = CoverFromTopRight + 1;

class Q_EXPORT NextEffect_Cover : public NextEffect
{
public:
	virtual bool prepareFrameAt(int frame);
	virtual bool hasType(EffectId t) const {
		return t==CoverFromBottom || t==CoverFromBottomLeft
				|| t==CoverFromBottomRight || t==CoverFromLeft
				|| t==CoverFromRight || t==CoverFromTop
				|| t==CoverFromTopLeft || t==CoverFromTopRight;
	}

private:
	//void calculateRegion_Random(qreal k); //default
	//typedef void (NextEffect_Cover::*calculateRegion_Func)(qreal);
	//calculateRegion_Func calculateRegion_ptr;
};


#endif // NEXTEFFECT_COVER_H
