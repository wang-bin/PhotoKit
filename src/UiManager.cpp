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
#include "Button.h"
#include "BaseItem.h"
#include "DemoItemAnimation.h"
#include "tools/ConfigDialog.h"
#include "network/WeiboDialog.h"
#include "PhotoKitView.h"
#include "ReflectEffectItem.h"
#include "score.h"
#include "SlideDisplay.h"
#include "SlidePlayControl.h"
#include "tools/Tools.h"
#include "tools/ToolTip.h"
#include "ThumbItem.h"
#include "ThumbTask.h"
#include "Config.h"
#include "ezlog.h"

namespace PhotoKit {
static QString help;
static QString thumbpage_help;
static QString playpage_help;
static QString about;
static QString CLEAR;
static QString ADDIMAGES;
static QString ADDDIRS;
static QString SETUP;
static QString HELP;
static QString QUIT;
static qreal X0 = 0;
static qreal Y0 = 0;
static void initTranslation() {
	CLEAR = QObject::tr("Clear");
	ADDIMAGES = QObject::tr("Add images");
	ADDDIRS = QObject::tr("Add dirs");
	SETUP = QObject::tr("Setup");
	HELP = QObject::tr("Help");
	QUIT = QObject::tr("Quit");
    help = QObject::tr("PRESS ME TO HIDE\n"
                       "OR DISABLE TIPS IN SETUP WINDOW\n"
                                  "Right click(press and hold) to show menu.\n"
                                  "Press a picture to zoom\n"
                                  "Double click a picture to show large image and double click aagin to go back\n"
                                  "Move mouse to see moving effect\n"
                                  "Two finger touch to zoom(NOT TESTED)");
    thumbpage_help = QObject::tr("PRESS ME TO HIDE\n"
                                 "OR DISABLE TIPS IN SETUP WINDOW\n"
                                            "Right click(press and hold) to show menu.\n"
                                            "Press a picture to zoom\n"
                                            "Double click a picture to show large image"
                                            "Move mouse to see moving effect\n"
                                            "Two finger touch to zoom(NOT TESTED)");
    playpage_help = QObject::tr("PRESS ME TO HIDE\n"
                                "OR DISABLE TIPS IN SETUP WINDOW\n"
                                           "Right click(press and hold) to show menu.\n"
                                           "You can share the current picture to sina weibo\n"
                                           "Double click to go back\n"
                                           "Two finger touch to zoom(NOT TESTED)");

    about = QObject::tr("Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>");
}
UiManager* UiManager::mInstance = 0;
ThumbItem* UiManager::lastHoverThumb = 0;
UiManager::PageType UiManager::page = UiManager::ThumbPage;

UiManager::UiManager(QObject *parent) :
    QObject(parent),mView(0),mThumbTask(new ThumbTask),mThumbsCount(0)
{
    initTranslation();
	score = new Score();
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
    mThumbPageRoot = new BaseItem;
    mCurrentPageRoot = mThumbPageRoot;
	mThumbPageRoot->setAcceptHoverEvents(true);
	X0 = qMin<qreal>(qMax<qreal>(qApp->desktop()->width() - 3*(2*(Config::thumbMargin + Config::thumbBorder) + Config::thumbSpacing + Config::thumbItemWidth), 0.5*Config::thumbItemWidth), Config::contentHMargin);
	Y0 = qMin<qreal>(qMax<qreal>((qreal)qApp->desktop()->height() - ((qreal)Config::thumbRows + 1)*((Config::thumbBorder
            + Config::thumbMargin)*2 + Config::thumbSpacing + Config::thumbItemHeight), - 0.5*Config::thumbItemHeight), Config::contentVMargin);
    qDebug("************%f, %f", X0, Y0);
    //content can't move if setPos?
    //mThumbPageRoot->setPos(Config::contentHMargin, qMax<qreal>(Config::thumbItemHeight, y));//contentVMargin); //TODO: ensure see the reflection
    mThumbPageRoot->translate(X0, Y0);
    mThumbPageRoot->setTransform(QTransform().scale(0.5, 0.5));
	mView->scene()->addItem(mThumbPageRoot);
	mPlayPageRoot = new SlideDisplay;
	mPlayPageRoot->hide();
	mView->scene()->addItem(mPlayPageRoot);
	mPlayControl = new SlidePlayControl(this);
	mPlayPageRoot->setPlayControl(mPlayControl);

    gotoPage(ThumbPage);
    Tools::showTip(thumbpage_help);
    mView->setInitialPos(X0, Y0);
	mView->setAnimationDuration(1618);
    mView->smartTransform(X0, Y0, 0.5, 1, 0, 0, 0, 0, 0);

	createMenus();
	showMenu("createLeftMenuTopInMovie");
}

QGraphicsItem* UiManager::currentPageRootItem()
{
    return mCurrentPageRoot;
}

BaseItem* UiManager::thumbPageRootItem()
{
	return mThumbPageRoot;
}

QGraphicsView* UiManager::view()
{
    return static_cast<QGraphicsView*>(mView);
}

bool UiManager::isSliding() const
{
	return mPlayControl->isRunning();
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
	QString dir = "/"; //QDir::homePath() is an invalid path on n900, why?
	QStringList paths = QFileDialog::getOpenFileNames(0, tr("Select images"), dir, Tools::imageNameFilters().join(" "));
	if (paths.isEmpty())
		return;
	showImagesFromThumb(paths, true);
}


void UiManager::addImagesFromDir()
{
	QString dir = QFileDialog::getExistingDirectory(0, tr("Select a directory contains images"), "/");
	if (!dir.isEmpty()) {
		showImagesFromThumb(dir);
	}
}

void UiManager::startSlide()
{
	mPlayControl->setDirection(SlidePlayControl::Forward);
	mPlayControl->setCurrentImage(mPlayPageRoot->imagePath());
	ezlog_debug("slide begin: %s", qPrintable(mPlayPageRoot->imagePath()));
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
	info += tr("Size") + ": " + QString::number(QFile(mPlayPageRoot->imagePath()).size()) + "byte\n";
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

void UiManager::showHelp()
{
    Tools::showTip(help, true);
}

void UiManager::showAbout()
{
    Tools::showTip(about, true);
}

void UiManager::updateThumbItemAt(int index)
{
	int show_index = index + mThumbsCount;
	//ezlog_debug("updateing thumb at %d. show on %d", index, show_index);
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
	ezlog_debug("total %d", mThumbsCount);
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
	menu.addAction(tr("Help"), this, SLOT(showHelp()));
    //menu.addAction(tr("About"), this, SLOT(showAbout()));
    menu.addAction(tr("Exit"), qApp, SLOT(quit()));
	menu.exec(pos);
}

void UiManager::gotoPage(PageType pageType, const QString& image)
{
	if (page == pageType)
		return;
	page = pageType;
	if (page == ThumbPage) {
        mCurrentPageRoot = mThumbPageRoot;
		mThumbPageRoot->show();
        mView->scene()->setSceneRect(mView->scene()->itemsBoundingRect()
                .adjusted(-Config::contentHMargin, -Config::contentVMargin, Config::contentHMargin, Config::contentVMargin)
            );
        Tools::showTip(thumbpage_help);

		mPlayPageRoot->smoothScale(1, 0.2, ItemAnimation::FadeOut);
	} else if (page == PlayPage) {
        mCurrentPageRoot = mPlayPageRoot;
		mThumbPageRoot->hide();
		mPlayPageRoot->setImagePath(image);
		mPlayPageRoot->smoothScale(4, 1, ItemAnimation::FadeIn);
        mPlayPageRoot->setPos(mView->mapToScene(QPoint())); //keep it center.
		//mPlayPageRoot->show();
		mView->scene()->setSceneRect(mPlayPageRoot->boundingRect().adjusted(-32, -32, 32, 32));
        Tools::showTip(playpage_help);
	}
}

void UiManager::tryMoveCenter(QGraphicsItem *item)
{
	//QPointF p0 = item->scenePos();
}

//do animations and end other operations
void UiManager::clickMenuItem()
{
	//common menu items
	Button *button = qobject_cast<Button*>(sender());
	ezlog_debug();
	QString menuText(button->text()); //plain text
	ezlog_debug();
	if (menuText == tr("Quit")) {
		qApp->quit();
	} else if (menuText == tr("Setup")) {

	}

	//playReverse()
	if (button->buttonType() == Button::ArrowUp) {
		ezlog_debug();
		score->queueMovie("createLeftMenuTopInMovie -out");
		button->setButtonType(Button::ArrowDown);
	} else if (button->buttonType() == Button::ArrowDown) {
		score->queueMovie("createLeftMenuTopInMovie");
		button->setButtonType(Button::ArrowUp);
	} else {

		score->queueMovie("createLeftMenuTopInMovie -shake");
	}
	if (score->hasQueuedMovies()){
		score->playQue();
	}
	//page depended menu items
	if (page == ThumbPage) {

	} else if (page == PlayPage) {

	}

}

void UiManager::createMenus()
{
	QStringList thumbPageMenuItems;
	thumbPageMenuItems << CLEAR << ADDIMAGES << ADDDIRS << SETUP << HELP << QUIT;
	Button *thumbPageMenuItem = 0;
	for (int i = 0; i < thumbPageMenuItems.size(); ++i) {
		thumbPageMenuItem = new Button("<p style='color:white;font-size:16px'>" + thumbPageMenuItems[i] + "</p>");
		mView->scene()->addItem(thumbPageMenuItem);
		connect(thumbPageMenuItem, SIGNAL(clicked()), SLOT(clickMenuItem()));
		createLeftMenuTopInMovie(thumbPageMenuItem, i);
	}
	thumbPageMenuItem = new Button(Button::ArrowUp, Button::RectShape);
	connect(thumbPageMenuItem, SIGNAL(clicked()), SLOT(clickMenuItem()));
	mView->scene()->addItem(thumbPageMenuItem);
	createLeftMenuTopInMovie(thumbPageMenuItem, thumbPageMenuItems.size() + 1);
}

void UiManager::createLeftMenuTopInMovie(Button *item, int i)
{
	int xOffset = 15;
	int yOffset = 22;
	//static
	static Movie *movieIn = score->insertMovie("createLeftMenuTopInMovie");
	static Movie *movieOut = score->insertMovie("createLeftMenuTopInMovie -out");
	static Movie *movieShake = score->insertMovie("createLeftMenuTopInMovie -shake");

	item->setVisible(false);
	item->setZValue(10);
	qreal ih = item->sceneBoundingRect().height();
	qreal ihp = ih + 3;

	// create in-animation:
	DemoItemAnimation *anim = new DemoItemAnimation(item, DemoItemAnimation::ANIM_IN);
	anim->setDuration(float(1000 + (i * 20)));
	anim->setStartPos(QPointF(xOffset, -ih));
	anim->setPosAt(0.20, QPointF(xOffset, -ih));
	anim->setPosAt(0.50, QPointF(xOffset, (i * ihp) + yOffset + 22 + (10 * float(i / 4.0f))));
	anim->setPosAt(0.60, QPointF(xOffset, (i * ihp) + yOffset + 22));
	anim->setPosAt(0.70, QPointF(xOffset, (i * ihp) + yOffset + 22 + (5 * float(i / 4.0f))));
	anim->setPosAt(0.80, QPointF(xOffset, (i * ihp) + yOffset + 22));
	anim->setPosAt(0.90, QPointF(xOffset, (i * ihp) + yOffset + 22 + (2 * float(i / 4.0f))));
	anim->setPosAt(1.00, QPointF(xOffset, (i * ihp) + yOffset + 22));
	movieIn->append(anim);

	// create out-animation:
	anim = new DemoItemAnimation(item, DemoItemAnimation::ANIM_OUT);
	anim->setDuration(float(1000 + (i * 20)));
	anim->setStartPos(QPointF(xOffset,  (i * ihp) + yOffset + 22));
	anim->setPosAt(1.00, QPointF(xOffset, 0));
	anim->setPosAt(0.90, QPointF(xOffset, (i * ihp) + yOffset + 22 + (10 * float(i / 4.0f))));
	anim->setPosAt(0.80, QPointF(xOffset, (i * ihp) + yOffset + 22));
	anim->setPosAt(0.70, QPointF(xOffset, (i * ihp) + yOffset + 22 + (5 * float(i / 4.0f))));
	anim->setPosAt(0.60, QPointF(xOffset, (i * ihp) + yOffset + 22));
	anim->setPosAt(0.50, QPointF(xOffset, (i * ihp) + yOffset + 22 + (2 * float(i / 4.0f))));
	anim->setPosAt(0.20, QPointF(xOffset, (i * ihp) + yOffset + 22));
	movieOut->append(anim);
/*	anim = new DemoItemAnimation(item, DemoItemAnimation::ANIM_OUT);
	anim->hideOnFinished = true;
	anim->setDuration((700 + (30 * i)));
	anim->setStartPos(QPointF(xOffset, (i * ihp) + yOffset + 22));
	anim->setPosAt(0.60, QPointF(xOffset, 600 - ih - ih));
	anim->setPosAt(0.65, QPointF(xOffset + 20, 600 - ih));
	anim->setPosAt(1.00, QPointF(sw + iw, 600 - ih));
	movieOut->append(anim);
*/
	// create shake-animation:
	anim = new DemoItemAnimation(item);
	anim->setDuration(700 * 1);
	anim->setStartPos(QPointF(xOffset, (i * ihp) + yOffset + 22));
	anim->setPosAt(0.55, QPointF(xOffset, (i * ihp) + yOffset + 22 - i*2.0));
	anim->setPosAt(0.70, QPointF(xOffset - 10, (i * ihp) + yOffset + 22 - i*1.5));
	anim->setPosAt(0.80, QPointF(xOffset, (i * ihp) + yOffset + 22 - i*1.0));
	anim->setPosAt(0.90, QPointF(xOffset - 2, (i * ihp) + yOffset + 22 - i*0.5));
	anim->setPosAt(1.00, QPointF(xOffset, (i * ihp) + yOffset + 22));
	movieShake->append(anim);
}

void UiManager::showMenu(const QString &menu)
{
	score->queueMovie(menu);
	score->queueMovie(menu + " -out", Score::NEW_ANIMATION_ONLY);
	score->queueMovie(menu + " -buttons -out", Score::NEW_ANIMATION_ONLY);
	score->queueMovie("upndown -shake");
	score->queueMovie(menu + " -shake");
	if (score->hasQueuedMovies()){
		score->playQue();
	}
}
} //namespace PhotoKit

