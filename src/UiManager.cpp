/******************************************************************************
	UiManager.cpp: description
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


#include "UiManager.h"
#include <QAction>
#include <QIcon>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QDesktopWidget>
#include "Button.h"
#include "PhotoKitView.h"
#include "ReflectEffectItem.h"
#include "ToolBar.h"
#include "ToolTip.h"
#include "ThumbItem.h"
#include "ThumbTask.h"
#include "Config.h"

namespace PhotoKit {

UiManager* UiManager::mInstance = 0;
ThumbItem* UiManager::lastHoverThumb = 0;
UiManager::UiManager(QObject *parent) :
    QObject(parent),mToolTip(0),mView(0),mThumbTask(new ThumbTask)
{
    connect(mThumbTask->watcher(), SIGNAL(resultReadyAt(int)), this, SLOT(updateThumbItemAt(int)));
}

UiManager* UiManager::instance()
{
	if (!mInstance) {
		mInstance = new UiManager;
	}
	return mInstance;
}

UiManager::~UiManager()
{
    //do not delete view;
    if (mThumbTask) {
        delete mThumbTask;
        mThumbTask = 0;
    }
    if (mToolTip) {
         delete mToolTip ;
        mToolTip = 0;
    }
    if (mRoot) {
        delete mRoot;
        mRoot = 0;
    }
}

void UiManager::init(PhotoKitView *view)
{
	mView = view;
    mRoot = new QGraphicsWidget;
    mRoot->setAcceptHoverEvents(true);
    mView->scene()->addItem(mRoot);
	//mToolTip = new ToolTip;
	//mToolTip->setPos(100, 0);
	mView->scene()->addItem(mToolTip);
    mBottomBar = new ToolBar;
    mView->scene()->addItem(mBottomBar);
    updateFixedItems();
    QRectF r = mView->visibleSceneRect();
   // mBottomBar->resize(QSizeF(qApp->desktop()->rect().width(), 66));
   //     mBottomBar->setPos(r.bottomLeft().x(), r.bottomLeft().y() - mBottomBar->rect().height());
    //mBottomBar->setPos(0, mView->viewport()->height() - mBottomBar->rect().height());
   // QPixmap icon(":/icons/add.png");
	//Button *bt = new Button(icon, mBottomBar);
	//bt->resize(mBottomBar->rect().height(), mBottomBar->rect().height());

}

QGraphicsItem* UiManager::rootItem()
{
    return mRoot;
}


ToolTip *UiManager::toolTipItem()
{
	return mToolTip;
}

void UiManager::updateFixedItems()
{
    QRectF r(mView->visibleSceneRect());
    //mBottomBar->resize(QSizeF(r.width(), 100));
    //mBottomBar->setPos(r.bottomLeft().x(), r.bottomLeft().y() - mBottomBar->rect().height());
   // mBottomBar->setPos(0, mView->viewport()->height() - mBottomBar->rect().height());
}

void UiManager::showImagesFromThumb(const QString &dir, bool yes)
{
    //ThumbTask or direct show
    if (yes) {
        mThumbTask->createThumbsFromDir(dir);
    }
}

void UiManager::showImagesFromThumb(const QStringList &paths, bool yes)
{
    //ThumbTask or direct show
    if (yes) {
        mThumbTask->createThumbs(paths);
    }
}

void UiManager::updateThumbItemAt(int index)
{
    qDebug("updateing thumb at %d", index);
    int col = index / Config::thumbRows;
    int row = index % Config::thumbRows;

    ThumbInfo info(mThumbTask->thumbInfoAt(index));
    ThumbItem *item = new ThumbItem(info.thumb, mRoot);
    item->setOriginImage(info.path);
    item->setPos(col * (Config::thumbItemWidth + (Config::thumbBorder + Config::thumbMargin)*2 + Config::thumbSpacing)
                 + Config::thumbBorder + Config::thumbMargin + (Config::thumbItemWidth - item->boundingRect().width())/2
                 , row * (Config::thumbItemHeight + (Config::thumbBorder + Config::thumbMargin)*2 + Config::thumbSpacing)
                 + Config::thumbBorder + Config::thumbMargin + (Config::thumbItemHeight - item->boundingRect().height())/2);
    //addItem(item);
    if (row == Config::thumbRows - 1) {
        new ReflectEffectItem(item, ReflectEffectItem::MirrorBottom);
    }
    //scene rect is importance.
    mView->scene()->setSceneRect(mView->scene()->itemsBoundingRect().adjusted(-Config::contentHMargin, -Config::contentVMargin, Config::contentHMargin, Config::contentVMargin));
}
} //namespace PhotoKit

