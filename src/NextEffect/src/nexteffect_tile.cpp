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

#include "nexteffect_tile.h"
#include <time.h>
#include <cmath>
#include "nexteffectfactory.h"


REGISTER_EFFECT(Tile, Tile)

NextEffect_Tile::NextEffect_Tile()
{
	effect_type = Tile;
}

bool NextEffect_Tile::prepare()
{
	NextEffect::prepare();
	rows = cols = 32; //
	nodes_per_frame = rows*cols/frames_total+1; //make sure that no blanks at the last frame

	w = width/rows+1; //make sure that no blanks at the last frame
	h = height/cols+1;//make sure that no blanks at the last frame
	nodes.clear();
	for(int i=0;i<rows;++i)
		for(int j=0;j<cols;++j)
			nodes.append(QPoint(i*w, j*h));
			//nodes.push_back(QPoint(i*w, j*h));

	next_clip_region = QRegion(); //Important!
	return true;
}

bool NextEffect_Tile::prepareFrameAt(int frame)
{
	if (frame>frames_total || nodes.empty())
		return false;
	current_frame = frame;

	srand(time(0));
	int index = 0;
	for(int i=0;i<nodes_per_frame;++i) {
		if (nodes.empty())
			break;
		index = rand()%(nodes.size());
		QPoint p = nodes.takeAt(index);//nodes.at(index);
		next_clip_region += QRegion(p.x(), p.y(), w, h);
		//nodes.erase(nodes.begin()+index);
	}
	current_clip_region = QRegion(0, 0, width, height) - next_clip_region;
	return true;
}
