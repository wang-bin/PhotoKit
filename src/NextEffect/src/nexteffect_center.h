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

#ifndef NEXTEFFECT_OPENCLOSE_H
#define NEXTEFFECT_OPENCLOSE_H

#include "nexteffect.h"

static const EffectId EllipseOpen = 5, EllipseClose = EllipseOpen + 1;

class Q_EXPORT NextEffect_Center : public NextEffect
{
public:
	virtual bool prepare();
	virtual bool prepareFrameAt(int frame);
	virtual bool hasType(EffectId t) const {
		return t==EllipseClose || t==EllipseOpen;
	}

protected:
	virtual bool isEndFrame(int frame);

private:
	bool prepareEllipseFromCenterFrameAt(int frame);
	bool prepareEllipseToCenterFrameAt(int frame);

	//bool prepareFrameAt_RectZoomOpen(int frame);

	typedef bool (NextEffect_Center::*prepareFrameAt_Func)(int);
	prepareFrameAt_Func func;
};

#endif // NEXTEFFECT_OPENCLOSE_H
