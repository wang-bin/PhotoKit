/******************************************************************************
	PhotoKitScene.cpp: description
	Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>
	
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


#include "PhotoKitScene.h"

#include <cstdlib>
#include <stdexcept>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

namespace PhotoKit {

PhotoKitScene* PhotoKitScene::mpInstance = 0;
bool PhotoKitScene::mDestroyed = false;

PhotoKitScene& PhotoKitScene::instance()
{
	if (!mpInstance) {
		MakeInstance();
	}
	return *mpInstance;
}

PhotoKitScene::PhotoKitScene(QObject *parent) :
	QGraphicsScene(parent)
{
	mpInstance = this;
	qDebug("ctor scene");
	//setItemIndexMethod(QGraphicsScene::NoIndex);
	setBackgroundBrush(QBrush(QColor(66, 66, 66)));

}

/*
void PhotoKitScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug("move in scene");
	event->accept();
}*/

void PhotoKitScene::MakeInstance()
{
	if (!mpInstance) {
		if (mDestroyed) {
			mDestroyed = false;
			throw std::logic_error("Dead Reference Detected");
		}
		qDebug("instance scene");
		mpInstance = new PhotoKitScene();
		std::atexit(&DestroySingleton);
	}
}

void PhotoKitScene::DestroySingleton()
{

}


} //namespace PhotoKit
