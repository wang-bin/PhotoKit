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

#ifndef NEXTEFFECT_MOVE_H
#define NEXTEFFECT_MOVE_H

#include "nexteffect.h"

static const EffectId MoveFromLeft = 32, MoveFromRight = MoveFromLeft + 1
			, MoveFromTop = MoveFromRight + 1, MoveFromBottom = MoveFromTop + 1; //35

class Q_EXPORT NextEffect_Move : public NextEffect
{
public:
	virtual bool prepare();
	virtual bool prepareFrameAt(int frame);
	virtual bool hasType(EffectId t) const {
		return t==MoveFromBottom || t==MoveFromLeft
				|| t==MoveFromRight || t==MoveFromTop;
	}

private:
	//void calculateRegion_Random(qreal k); //default
	void calculateRegion_FromBottom(qreal k);
	void calculateRegion_FromLeft(qreal k);
	void calculateRegion_FromRight(qreal k);
	void calculateRegion_FromTop(qreal k);

	typedef void (NextEffect_Move::*calculateRegion_Func)(qreal);
	calculateRegion_Func calculateRegion_ptr;
};

#endif // NEXTEFFECT_MOVE_H
