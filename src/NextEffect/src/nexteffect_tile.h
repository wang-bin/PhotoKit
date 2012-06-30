/******************************************************************************
	NextEffect_Tile: Tile spreading effect.
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

#ifndef NEXTEFFECT_TILE_H
#define NEXTEFFECT_TILE_H

#include "nexteffect.h"
//#include <list>

static const EffectId Tile = 38;

class Q_EXPORT NextEffect_Tile : public NextEffect
{
public:
	NextEffect_Tile();
	virtual bool prepare();
	virtual bool prepareFrameAt(int frame);

private:
	int nodes_per_frame, w, h;
	int rows, cols;
	//std::vector<QPoint> nodes;
	QList<QPoint> nodes;
};

#endif // NEXTEFFECT_TILE_H
