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

#include <stdexcept>
#include <QGraphicsPixmapItem>
#include "ReflectEffectItem.h"
#include "ThumbItem.h"
#include "ThumbTask.h"
#include "Config.h"

BEGIN_NAMESPACE_PHOTOKIT

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

void PhotoKitScene::updateThumbItemAt(int index)
{
	qDebug("updateing thumb at %d", index);
	int col = index / Config::thumbRows;
	int row = index % Config::thumbRows;

    ThumbInfo info(ThumbTask::thumbInfoAt(index));
    ThumbItem *item = new ThumbItem(info.thumb);
    item->setOriginImage(info.path);
    item->setPos(col * (Config::thumbItemWidth + (Config::thumbBorder + Config::thumbMargin)*2 + Config::thumbSpacing)
				 + Config::thumbBorder + Config::thumbMargin + (Config::thumbItemWidth - item->boundingRect().width())/2
                 , row * (Config::thumbItemHeight + (Config::thumbBorder + Config::thumbMargin)*2 + Config::thumbSpacing)
				 + Config::thumbBorder + Config::thumbMargin + (Config::thumbItemHeight - item->boundingRect().height())/2);
	addItem(item);
    if (row == Config::thumbRows - 1) {
        new ReflectEffectItem(item);
    }
	setSceneRect(itemsBoundingRect().adjusted(-Config::contentHMargin, -Config::contentVMargin, Config::contentHMargin, Config::contentVMargin));
}

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


END_NAMESPACE_PHOTOKIT
