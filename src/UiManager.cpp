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
#include <QFileDialog>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenu>
#include <QMessageBox>
#include <QAction>
#include "tools/ConfigDialog.h"
#include "network/WeiboDialog.h"
#include "PhotoKitView.h"
#include "ReflectEffectItem.h"
#include "SlideDisplay.h"
#include "SlidePlayControl.h"
#include "tools/Tools.h"
#include "ThumbItem.h"
#include "ThumbTask.h"
#include "Config.h"

namespace PhotoKit {

UiManager* UiManager::mInstance = 0;
ThumbItem* UiManager::lastHoverThumb = 0;
UiManager::PageType UiManager::page = UiManager::ThumbPage;

UiManager::UiManager(QObject *parent) :
	QObject(parent),mToolTip(0),mView(0),mThumbTask(new ThumbTask),mThumbsCount(0)
{
	connect(mThumbTask->watcher(), SIGNAL(resultReadyAt(int)), this, SLOT(updateThumbItemAt(int)));
	connect(mThumbTask->watcher(), SIGNAL(finished()), this, SLOT(updateDisplayedThumbList()));
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
	if (mThumbPageRoot) {
		delete mThumbPageRoot;
		mThumbPageRoot = 0;
    }
	if (mPlayPageRoot) {
		delete mPlayPageRoot;
		mPlayPageRoot = 0;
	}
}

void UiManager::init(PhotoKitView *view)
{
	mView = view;
	mThumbPageRoot = new QGraphicsWidget;
	mThumbPageRoot->setAcceptHoverEvents(true);
	mView->scene()->addItem(mThumbPageRoot);
	mPlayPageRoot = new SlideDisplay;
	mPlayPageRoot->hide();
	mView->scene()->addItem(mPlayPageRoot);
	mPlayControl = new SlidePlayControl(this);
	mPlayPageRoot->setPlayControl(mPlayControl);

	gotoPage(ThumbPage);

}

QGraphicsItem* UiManager::thumbPageRootItem()
{
	return mThumbPageRoot;
}

bool UiManager::isSliding() const
{
	return mPlayControl->isRunning();
}

ToolTip *UiManager::toolTipItem()
{
	return mToolTip;
}

void UiManager::updateFixedItems()
{
	//QRectF r(mView->visibleSceneRect());
    //mBottomBar->resize(QSizeF(r.width(), 100));
    //mBottomBar->setPos(r.bottomLeft().x(), r.bottomLeft().y() - mBottomBar->rect().height());
   // mBottomBar->setPos(0, mView->viewport()->height() - mBottomBar->rect().height());
}

void UiManager::showImagesFromThumb(const QString &dir, bool yes)
{
    //ThumbTask or direct show
    if (yes) {
        mThumbTask->createThumbsFromDirs(QStringList() << dir);
    }
}

void UiManager::showImagesFromThumb(const QStringList &paths, bool yes)
{
    //ThumbTask or direct show
    if (yes) {
        mThumbTask->createThumbs(paths);
    }
}

void UiManager::clearThumbs()
{
	QMessageBox::StandardButton res = QMessageBox::question(0, "", tr("Clear will not delete the image. Continue?"), QMessageBox::Ok | QMessageBox::No);
	if (res != QMessageBox::Ok)
		return;
	//mView->scene()->removeItem(mThumbPageRoot);
	QList<QGraphicsItem*> cs = mThumbPageRoot->childItems();
	foreach(QGraphicsItem* c, cs)
		mView->scene()->removeItem(c);
	mView->scene()->setSceneRect(mView->scene()->itemsBoundingRect());
	ThumbRecorder::displayedThumbs().clear();
	mThumbsCount = 0;
}

void UiManager::addImages()
{
	QStringList paths = QFileDialog::getOpenFileNames(0, tr("Select images"), QDir::homePath(), Tools::imageNameFilters().join(" "));
	if (paths.isEmpty())
		return;
	showImagesFromThumb(paths, true);
}

void UiManager::startSlide()
{
	mPlayControl->setDirection(SlidePlayControl::Forward);
	mPlayControl->setCurrentImage(mPlayPageRoot->imagePath());
	qDebug("slide begin: %s", qPrintable(mPlayPageRoot->imagePath()));
	mPlayControl->start();
}

void UiManager::stopSlide()
{
	mPlayControl->stop();
}

void UiManager::showCurrentImageInfo()
{
	//TODO: flip to show backside
	QString info(mPlayPageRoot->imagePath());
	QImage image(mPlayPageRoot->imagePath());
	info += "\n";
	info += tr("Depth") + ": " + QString::number(image.depth()) + "\n";
	info += tr("Width") + ": " +  QString::number(image.width()) + "\n";
	info += tr("Height") + ": " + QString::number(image.height()) + "\n";

	QMessageBox::information(0, tr("Image infomation"), info);
}

void UiManager::shareToWeibo()
{
	WeiboDialog w;
	w.setImage(mPlayPageRoot->imagePath());
	w.setUser(Config::weiboUser);
	w.setPassword(Config::weiboPasswd);
	w.exec();
}

void UiManager::setup()
{
	ConfigDialog d;
	d.exec();
}

void UiManager::addImagesFromDir()
{
	QString dir = QFileDialog::getExistingDirectory();
	if (!dir.isEmpty()) {
		showImagesFromThumb(dir);
	}
}

void UiManager::updateThumbItemAt(int index)
{
	int show_index = index + mThumbsCount;
	qDebug("updateing thumb at %d. show on %d", index, show_index);
	int col = show_index / Config::thumbRows;
	int row = show_index % Config::thumbRows;

    ThumbInfo info(mThumbTask->thumbInfoAt(index));
	ThumbItem *item = new ThumbItem(info.thumb, mThumbPageRoot);
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

void UiManager::updateDisplayedThumbList()
{
	mPlayControl->setImages(ThumbRecorder::displayedThumbs()); //current record
	mThumbsCount = ThumbRecorder::displayedThumbs().size();
	qDebug("total %d", mThumbsCount);
}

void UiManager::popupMenu(const QPoint& pos)
{
	QMenu menu;
	if (page == UiManager::ThumbPage) {
		menu.addAction(tr("Clear"), this, SLOT(clearThumbs()));
		menu.addAction(tr("Add images"), this, SLOT(addImages()));
		menu.addAction(tr("Add dir"), this, SLOT(addImagesFromDir()));
	} else if (page == UiManager::PlayPage) {
		menu.addAction(tr("Information"), this, SLOT(showCurrentImageInfo()));
		menu.addAction(tr("Weibo share"), this, SLOT(shareToWeibo()));
		if (mPlayControl->isRunning())
			menu.addAction(tr("Stop slide"), this, SLOT(stopSlide()));
		else
			menu.addAction(tr("Start slide"), this, SLOT(startSlide()));
	}
	menu.addAction(tr("Setup"), this, SLOT(setup()));
	menu.exec(pos);
}

void UiManager::gotoPage(PageType pageType, const QString& image)
{
	if (page == pageType)
		return;
	page = pageType;
	if (page == ThumbPage) {
		mPlayPageRoot->hide();
		mThumbPageRoot->show();
		mView->scene()->setSceneRect(mView->scene()->itemsBoundingRect().adjusted(-Config::contentHMargin, -Config::contentVMargin, Config::contentHMargin, Config::contentVMargin));
		Tools::showTip(tr("Right click to show menu.\n"
						  "Press a picture to zoom\n"
						  "Double click a picture to show large image\n"
						  "Move mouse to see moving effect\n"
						  "Two finger touch to zoom all images"));

	} else if (page == PlayPage) {
		mThumbPageRoot->hide();
		mPlayPageRoot->setImagePath(image);
		mPlayPageRoot->show();
		mView->scene()->setSceneRect(mPlayPageRoot->boundingRect().adjusted(-32, -32, 32, 32));
		Tools::showTip(tr("Right click to show menu.\n"
						  "Double click to go back"));
	}
}

} //namespace PhotoKit

