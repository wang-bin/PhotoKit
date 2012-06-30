/******************************************************************************
	NextEffect: Base class of all effect classes.
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

#include "nexteffect.h"
#include <qpainter.h>

NextEffect::NextEffect()
{
	finished = false;
	k = 0.0;
	speed = 1.0;
	current_frame = 0;
	frames_total = 17;
	current_image = next_image = frame_image = 0;
	width = height = 0;
}

NextEffect::~NextEffect()
{
	if (current_image) {
		delete current_image;
		current_image = 0;
	}
	if (next_image) {
		delete next_image;
		next_image = 0;
	}
	if (frame_image) {
		delete frame_image;
		frame_image = 0;
	}
}

bool NextEffect::prepare()
{
	finished = false;
	k = 0.0;
	current_frame = 0;
	next_rect = QRect(0, 0, width, height);
	current_rect = QRect(0, 0, width, height);
	return true;
}

bool NextEffect::prepareNextFrame()
{
	//current_frame++? do not paint frame 0?
	if (prepareFrameAt(++current_frame)) {
		renderBackground();
		return true;
	}
	return false;
}

void NextEffect::setSize(const QSize &s)
{
	width = s.width();
	height = s.height();
	//TODO: just black background
	if (current_image && current_image->size()!=QSize(width, height)) {
		/*if (current_image->width()>current_image->height())
			*current_image = current_image->scaledToWidth(width);
		else
			*current_image = current_image->scaledToHeight(height);*/
		*current_image = current_image->scaled(width, height); //4, 1/4
	}
	if (next_image && next_image->size()!=QSize(width, height)) {
		/*if (next_image->width()>next_image->height())
			*next_image = next_image->scaledToWidth(width);
		else
			*next_image = next_image->scaledToHeight(height);*/
		*next_image = next_image->scaled(width, height);
	}
	if (frame_image && frame_image->size()!=QSize(width, height)) {
		/*if (frame_image->width()>frame_image->height())
			*frame_image = frame_image->scaledToWidth(width);
		else
			*frame_image = frame_image->scaledToHeight(height);*/
		*frame_image = frame_image->scaled(width, height);
	}
}

//TODO: resize to comment rect
void NextEffect::setInitialPixmaps(const QImage &pixmapCurrent, const QImage &pixmapNext)
{
	/*
	if (width==0 || height==0) {
		width = qMax(pixmapCurrent.width(), pixmapNext.width());
		height = qMax(pixmapCurrent.height(), pixmapNext.height());
	}
	*/

	if (!current_image)
		current_image = new QImage(pixmapCurrent);
	else
		*current_image = pixmapCurrent.copy();
	if (!next_image)
		next_image = new QImage(pixmapNext);
	else
		*next_image = pixmapNext.copy();

	//do not always allocate a new memory
	//mNextImage = mNextImage.scaled(4*size(), Qt::IgnoreAspectRatio, Qt::FastTransformation).scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	/*
	if (!current_image) {
		current_image = new QPixmap(width, height);
	} else {
		if (pixmapCurrent.width()>width || pixmapCurrent.height()>height) //ignore, keep
			*current_image = pixmapCurrent.copy().scaled(4*width, 4*height).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		else
			*current_image = pixmapCurrent.copy();
	}
	if (!next_image)
		next_image = new QPixmap(width, height);
	else
		if (pixmapNext.width()>width || pixmapNext.height()>height)
			*next_image = pixmapNext.copy().scaled(4*width, 4*height).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		else
			*next_image = pixmapNext.copy();
	*/
	//Qt::black ?
	if (!frame_image)
		frame_image = new QImage(pixmapCurrent); //anti flicking//(pixmapNext.size());
	//frame_image->fill(Qt::black);

/*
	//draw at center
	QPainter p(current_image);
	//draw inside
	int dx = (width-pixmapCurrent.width())*0.5;
	int dy = (height-pixmapCurrent.height())*0.5;
	if (dx<0)
		dx = 0;
	if (dy<0)
		dy = 0;
	p.translate(dx, dy);
	p.drawPixmap(0, 0, pixmapCurrent);
	p.end();

	dx = (width-pixmapNext.width())*0.5;
	dy = (height-pixmapNext.height())*0.5;
	if (dx<0)
		dx = 0;
	if (dy<0)
		dy = 0;
	p.begin(next_image);
	p.translate(dx, dy);
	p.drawPixmap(0, 0, pixmapNext);
	p.end();
*/
	setSize(pixmapNext.size());
	//setSize(QSize(width, height));
	current_frame = 0;
}

bool NextEffect::isEndFrame(int frame)
{
/*
	k>=1.0 may result in the speed is too large. If we stop immediately, the final frame will never show.
	At this time ,we show the last frame, and tell the program that effect is finished;
*/
	if (finished)
		return true;
	//if (frame>frames_total)
	//	return true;
	current_frame = frame;
	k = speed*(qreal)current_frame/(qreal)frames_total;
	//qDebug("%f", k);

	if (k>=1.0) {
		k = 1.0; //It's important
		finished = true;
	}
	return false;
}

void NextEffect::renderBackground()
{
	QPainter p(frame_image);
/*
	Tell me why!
	if draw the next frame_image first, then it will flick for FromBottom, CoverBottom effect!
*/
	//actually we can just paint the next frame_image if no opacity changes
	p.setClipRegion(current_clip_region);
	p.drawImage(QRect(0, 0, width, height), *current_image, current_rect);

	p.setClipRegion(next_clip_region);
	p.drawImage(QRect(0, 0, width, height), *next_image, next_rect);
}


