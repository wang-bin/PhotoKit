/******************************************************************************
	NextEffect_Sector: Sector expending effect.
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

#ifndef NEXTEFFECT_SECTOR_H
#define NEXTEFFECT_SECTOR_H

#include "nexteffect.h"

static const EffectId SectorClockwise = 36, SectorAntiColckwise = SectorClockwise + 1; //37

class Q_EXPORT NextEffect_Sector : public NextEffect
{
public:
	NextEffect_Sector();
	virtual bool prepare();
	virtual bool prepareFrameAt(int frame);

private:
	virtual bool isEndFrame(int frame);
	QPoint getIntersectPointByAngel(double degree);

	bool clock_wise;
	int edges;
	double angle; //angle0 is not needed
	double delta;
	QPoint p;
	QPoint v_next, v_current;
	//d-list
	QList<QPoint> v;
	QList<double> a;
	double a_next, a_current;
	double a_topleft, a_topright, a_bottomleft, a_bottomright;

	QPolygon polygon;
};

#endif // NEXTEFFECT_SECTOR_H
