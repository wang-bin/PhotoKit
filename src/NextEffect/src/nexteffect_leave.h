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

#ifndef NEXTEFFECT_LEAVE_H
#define NEXTEFFECT_LEAVE_H

#include "nexteffect.h"

static const EffectId LeaveToBottom = 24, LeaveToLeft = LeaveToBottom + 1, LeaveToRight = LeaveToLeft + 1
			, LeaveToTop = LeaveToRight + 1, LeaveToBottomLeft = LeaveToTop + 1
			, LeaveToBottomRight = LeaveToBottomLeft + 1, LeaveToTopLeft = LeaveToBottomRight + 1
			, LeaveToTopRight = LeaveToTopLeft + 1; //31

class Q_EXPORT NextEffect_Leave : public NextEffect
{
public:
	virtual bool prepare();
	virtual bool prepareFrameAt(int frame);
	virtual bool hasType(EffectId t) const {
		return t==LeaveToBottom || t==LeaveToBottomLeft
				|| t==LeaveToBottomRight || t==LeaveToLeft
				|| t==LeaveToRight || t==LeaveToTop
				|| t==LeaveToTopLeft || t==LeaveToTopRight;
	}

private:
	//void calculateRegion_Random(qreal k); //default
	void calculateRegion_ToBottom(qreal k);
	void calculateRegion_ToBottomLeft(qreal k);
	void calculateRegion_ToBottomRight(qreal k);
	void calculateRegion_ToLeft(qreal k);
	void calculateRegion_ToRight(qreal k);
	void calculateRegion_ToTop(qreal k);
	void calculateRegion_ToTopLeft(qreal k);
	void calculateRegion_ToTopRight(qreal k);

	typedef void (NextEffect_Leave::*calculateRegion_Func)(qreal);
	calculateRegion_Func calculateRegion_ptr;
};

#endif // NEXTEFFECT_LEAVE_H
