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

#include "nexteffect_sector.h"
#include <cmath>
#include <time.h>
#include "nexteffectfactory.h"

#define PI 3.14159265

REGISTER_EFFECT(SectorClockwise, Sector)
REGISTER_EFFECT(SectorAntiColckwise, Sector)

NextEffect_Sector::NextEffect_Sector()
{
}

bool NextEffect_Sector::prepare()
{
	polygon.clear();
	a_topright = atan2((qreal)height, (qreal)width) * 180.0/PI;
	a_topleft = atan2((qreal)height, -(qreal)width) * 180.0/PI;
	a_bottomleft = atan2(-(qreal)height, -(qreal)width) * 180.0/PI + 360;
	a_bottomright = atan2(-(qreal)height, (qreal)width) * 180.0/PI + 360;

	qDebug("%.1f, %.1f, %.1f, %.1f", a_topright, a_topleft, a_bottomleft, a_bottomright);

	v.clear();
	a.clear();
	NextEffect::prepare();
	srand(time(0));
	angle = rand()%361;
	qDebug("Angle0=%.1f", angle);
	v_current = getIntersectPointByAngel(angle);
	a_current = angle;
	delta = 360.0/frames_total * speed;
	edges = 3;
	polygon.append(QPoint(width*0.5, height*0.5));
	polygon.append(v_current);
	polygon.append(v_current);

	//anti-clockwise
	if (angle>=a_bottomright || angle<a_topright) {
		v<<QPoint(width, 0)<<QPoint(0, 0)<<QPoint(0, height)<<QPoint(width, height);
		a<<a_topright<<a_topleft<<a_bottomleft<<a_bottomright;
	} else if (angle>=a_topright && angle<=a_topleft) {
		v<<QPoint(0, 0)<<QPoint(0, height)<<QPoint(width, height)<<QPoint(width, 0);
		a<<a_topleft<<a_bottomleft<<a_bottomright<<a_topright;
	} else if (angle>=a_topleft && angle<a_bottomleft) {
		v<<QPoint(0, height)<<QPoint(width, height)<<QPoint(width, 0)<<QPoint(0, 0);
		a<<a_bottomleft<<a_bottomright<<a_topright<<a_topleft;
	} else {
		v<<QPoint(width, height)<<QPoint(width, 0)<<QPoint(0, 0)<<QPoint(0, height);
		a<<a_bottomright<<a_topright<<a_topleft<<a_bottomleft;
	}

	v_next = v.takeFirst();
	a_next = a.takeFirst();
	v.append(v_next);
	a.append(a_next);

	return true;
}


bool NextEffect_Sector::prepareFrameAt(int frame)
{
	if (isEndFrame(frame))
		return false;
	if (finished)
		return true;
	angle += delta;
	if (angle>360)
		angle -= 360;
	polygon.pop_back();
	//qDebug("next=%.1f a=%.1f", a_next, angle);
	//0-360=-360
	double dc = angle-a_current, dn = a_next-angle;
	if (dc>180)
		dc -= 360;
	else if (dc<-180)
		dc += 360;
	if (dn>180)
		dn -= 360;
	else if (dn<-180)
		dn += 360;

	if (dc*dn<0) {
		//qDebug("change %d", a.size());
		v_current = v_next;
		a_current = a_next;
		v_next = v.takeFirst();
		a_next = a.takeFirst();
		polygon.append(v_current);
	}
	polygon.append(getIntersectPointByAngel(angle));

	//qDebug("points=%d", polygon.size());
	next_clip_region = QRegion(polygon);
	current_clip_region = QRegion(0, 0, width, height) - next_clip_region;
	return true;
}

bool NextEffect_Sector::isEndFrame(int frame)
{
	current_frame = frame;
	k = speed*(qreal)current_frame/(qreal)frames_total;
	if (finished)
		return true;

	//here k is only used as the stopping condition
	if (k>=1.0) {
		k = 1.0; //It's important
		//to fill the whole rect, or some region will be blank due to machine error
		next_clip_region = QRegion(0, 0, width, height);
		current_clip_region = QRegion(0, 0, width, height) - next_clip_region;
		finished = true;
	}
	return false;
}


QPoint NextEffect_Sector::getIntersectPointByAngel(double degree)
{
	while (degree>360)
		degree -= 360;
	QPoint p;
	double r = degree * PI/180.0;
	if (degree>=a_bottomright || degree<a_topright) {
		p.setX(width);
		p.setY(0.5*width*(1.0-tan(r)));
	} else if (degree>=a_topright && degree<=a_topleft) {
		p.setX(0.5*height*(1.0+tan(PI/2-r)));
		p.setY(0);
	} else if (degree>=a_topleft && degree<a_bottomleft) {
		p.setX(0);
		p.setY(0.5*width*(1.0+tan(r-PI)));
	} else {
		p.setX(0.5*height*(1-tan(PI*3/2-r)));
		p.setY(height);
	}
	//qDebug("%.1f: (%d, %d)", degree, p.x(), p.y());
	return p;
}
