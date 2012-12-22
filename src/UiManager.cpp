/******************************************************************************
	UiManager: the manager.
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
#include <QAction>
#include "TextEdit.h"
#include "tools/ImageInfoDialog.h"
#include "Button.h"
#include "BaseItem.h"
#include "DemoItemAnimation.h"
#include "tools/ExifReader.h"
#include "network/WeiboDialog.h"
#include "network/GoogleImageSearcher.h"
#include "PhotoKitView.h"
#include "ReflectEffectItem.h"
#include "score.h"
#include "SlideDisplay.h"
#include "SlidePlayControl.h"
#include "tools/Tools.h"
#include "tools/ToolTip.h"
#include "ThumbItem.h"
#include "ThumbRecorder.h"
#include "ThumbTask.h"
#include "Config.h"
#include "ezlog.h"

namespace PhotoKit {
static QString help;
static QString thumbpage_help;
static QString playpage_help;
static QString about;
static QString CLEAR_CACHE;
static QString CLEAR;
static QString ADDIMAGES;
static QString ADDDIRS;
static QString SETUP;
static QString HELP;
static QString QUIT;
static QString START_STOP_SLIDE;
static QString STOP_SLIDE;
static QString WEIBO_SHARE;
static QString BACK;
static QString HELP_TEXT;
static QString IMAGE_INFO;
static QString GOOGLE_SEARCH;
static QString SEARCH;
static QString ABOUT_QT;

static qreal X0 = 0;
static qreal Y0 = 0;
static const QString kThumbPageMenu("thumbPageMenu");
static const QString kPlayPageMenu("playPageMenu");
static const QString kConfigMenu("ConfigMenu");
static const QString kOkCancelMenu("OkCancel");
static const QString kBackMenu("GoBack");
static const QString kSearchPageMenu("SearchPageMenu");

static void initTranslation() {
	SEARCH = QObject::tr("Search");
	GOOGLE_SEARCH = QObject::tr("Google");
    WEIBO_SHARE = QObject::tr("Weibo share");
    BACK = QObject::tr("Back");
    START_STOP_SLIDE = QObject::tr("Play/Stop slide");
    STOP_SLIDE = QObject::tr("Stop slide");
    CLEAR_CACHE = QObject::tr("Clear cache");
    CLEAR = QObject::tr("Clear");
	ADDIMAGES = QObject::tr("Add images");
	ADDDIRS = QObject::tr("Add dirs");
	SETUP = QObject::tr("Setup");
	HELP = QObject::tr("Help");
	QUIT = QObject::tr("Quit");
	IMAGE_INFO = QObject::tr("Image info");
    ABOUT_QT = QObject::tr("About Qt");
    help = QObject::tr("PRESS ME TO HIDE\n"
                                  "Press a picture to zoom\n"
                                  "Double click a picture to show large image and double click aagin to go back\n"
                                  "Move mouse to see moving effect\n"
								  "Two finger touch to zoom");
    thumbpage_help = QObject::tr("PRESS ME TO HIDE\n"
                                            "Press a picture to zoom\n"
                                            "Double click a picture to show large image"
                                            "Move mouse to see moving effect\n"
											"Two finger touch to zoom");
    playpage_help = QObject::tr("PRESS ME TO HIDE\n"
                                           "You can share the current picture to sina weibo\n"
                                           "Double click to go back\n"
										   "Two finger touch to zoom");

    about = QString("PhotoKit %1<br>Copyright (C) 2012 Wang Bin (wbsecg1@gmail.com)\n").arg(PHOTOKIT_VERSION_STR_LONG);
	HELP_TEXT = "<p>" + about + "</p></p>" + QObject::tr("PRESS ME TO HIDE") + "</p>"
			+ "<p>" + QObject::tr("Press a picture to zoom") + "</p>"
			+ "<p>" + QObject::tr("Double click a picture to show large image and double click again to go back") + "</p>"
			+ "<p>" + QObject::tr("Two finger touch to zoom") + "</p>"
			+ "<p>" + QObject::tr("Long press(right click) then move to zoom") + "</p>";
}
UiManager* UiManager::mInstance = 0;
ThumbItem* UiManager::lastHoverThumb = 0;
UiManager::PageType UiManager::page = UiManager::ThumbPage;

UiManager::UiManager(QObject *parent) :
	QObject(parent),mView(0),mThumbTask(new ThumbTask),mThumbsCount(0)
  ,mSearchInput(0),mGoogleSearcher(0),mSearchImageIndex(0)
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
	}/*
	if (mThumbPageRoot) {
		delete mThumbPageRoot;
		mThumbPageRoot = 0;
	}
	if (mPlayPageRoot) {
		delete mPlayPageRoot;
		mPlayPageRoot = 0;
	}
	if (mSearchPageRoot) {
		delete mSearchPageRoot;
		mSearchPageRoot = 0;
	}*/
}

void UiManager::init(PhotoKitView *view)
{
	mView = view;
    mThumbPageRoot = new BaseItem;
    mCurrentPageRoot = mThumbPageRoot;
	mThumbPageRoot->setAcceptHoverEvents(true);
	X0 = qMin<qreal>(qMax<qreal>(qApp->desktop()->width() - (Config::thumbRows+1)*(2*(Config::thumbMargin + Config::thumbBorder)
			+ Config::thumbSpacing + Config::thumbItemWidth), 0.5*Config::thumbItemWidth), Config::contentHMargin);
	Y0 = qMin<qreal>(qMax<qreal>((qreal)qApp->desktop()->height() - ((qreal)Config::thumbRows + 1)*((Config::thumbBorder
            + Config::thumbMargin)*2 + Config::thumbSpacing + Config::thumbItemHeight), - 0.5*Config::thumbItemHeight), Config::contentVMargin);
	Y0 = qMax<qreal>(Y0, 0.5*(qApp->desktop()->height() - (Config::thumbRows+2)*(Config::thumbItemHeight
			+ 2*(Config::thumbMargin + Config::thumbBorder) + Config::thumbSpacing)));
	qDebug("************%f, %f", X0, Y0);
    //content can't move if setPos?
    //mThumbPageRoot->setPos(Config::contentHMargin, qMax<qreal>(Config::thumbItemHeight, y));//contentVMargin); //TODO: ensure see the reflection
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#else
	mThumbPageRoot->translate(-X0, Y0); //no this api in Qt5
#endif //QT_VERSION_CHECK(5, 0, 0)
	mThumbPageRoot->setPos(X0, Y0);
    mThumbPageRoot->setTransform(QTransform().scale(0.5, 0.5));
	mView->scene()->addItem(mThumbPageRoot);
	mPlayPageRoot = new SlideDisplay;
    //mPlayPageRoot->setPos(0, 0);
	mPlayPageRoot->hide();
	mView->scene()->addItem(mPlayPageRoot);
	mPlayControl = new SlidePlayControl(this);
	mPlayPageRoot->setPlayControl(mPlayControl);

	mSearchPageRoot = new BaseItem;
	mSearchPageRoot->setPos(0, Y0);
	mView->scene()->addItem(mSearchPageRoot);

    createMenus(); //before gotoPage
	showMenu(kThumbPageMenu);

    gotoPage(ThumbPage);
	//mView->setInitialPos(X0, Y0);
	mView->setAnimationDuration(1618);
	//mView->smartTransform(X0, Y0, 0.5, 1, 0, 0, 0, 0, 0);
	mView->smartTransform(0, 0, 0.5, 1, 0, 0, 0, 0, 0);

}

BaseItem* UiManager::currentPageRootItem()
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
	mThumbTask->createThumbsFromDirs(QStringList() << dir, yes);

}

void UiManager::showImagesFromThumb(const QStringList &paths, bool yes)
{
    //ThumbTask or direct show
	mThumbTask->createThumbs(paths, yes);
}

//TODO: move to setup
void UiManager::clearThumbs()
{
	//TODO: custume message box. Use MessageBox
	//Tools::showTip(tr("Clear will not delete the image. Continue?"), true, 2000);
	mThumbTask->stop();
	//mView->scene()->removeItem(mThumbPageRoot);
	QList<QGraphicsItem*> cs = mThumbPageRoot->childItems();
	foreach(QGraphicsItem* c, cs) {
		mView->scene()->removeItem(c);
		delete c;
	}
    mView->scene()->setSceneRect(mView->scene()->itemsBoundingRect());
	ThumbRecorder::instance()->clearDisplay();
	ThumbRecorder::instance()->save(); //
	mThumbsCount = 0;
}

//DO NOT use QFileDialog's static member. creating thumbs will block the screen after seleting. we must delete the dialog immediatly
void UiManager::addImages()
{
	QString dir = "/"; //QDir::homePath() is an invalid path on n900, why?
	QStringList paths;
	QFileDialog *d = new QFileDialog(0, tr("Select images"), dir, Tools::imageNameFilters().join(" "));
	d->setFileMode(QFileDialog::ExistingFiles);
	//d->setOption(QFileDialog::DontUseNativeDialog, false);
    if (d->exec() == QDialog::Accepted) {
		paths = d->selectedFiles();
		delete d;
	} else {
		delete d;
		return;
	}

	//QStringList paths = QFileDialog::getOpenFileNames(0, tr("Select images"), dir, Tools::imageNameFilters().join(" "));
	if (paths.isEmpty())
		return;
	showImagesFromThumb(paths, true);
}

//DO NOT use QFileDialog's static member. creating thumbs will block the screen after seleting. we must delete the dialog immediatly
void UiManager::addImagesFromDir()
{
	QString dir;
    QFileDialog *d = new QFileDialog(0, tr("Select images"), dir, Tools::imageNameFilters().join(" "));
	//d->setOption(QFileDialog::ShowDirsOnly);
    d->setFileMode(QFileDialog::Directory); //DirectoryOnly is deprecated from 4.5, setOption(ShowDirsOnly, true) instead
    if (d->exec() == QDialog::Accepted) {
		dir = d->directory().absolutePath();
		delete d;
	} else {
		delete d;
		return;
	}
	//QString dir = QFileDialog::getExistingDirectory(0, tr("Select a directory contains images"), "/");
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
	mView->setCanTransform(false);
	//TODO: flip to show backside. line height
	ImageInfoDialog m(mView->scene());
	m.setPos(qApp->desktop()->width()/2, 2);
	m.resize(qApp->desktop()->size()/2);
	m.setWindowTitle(tr("Image infomation"));
	//m.setText(info);

	QString path = mPlayPageRoot->imagePath();
	if (mPlayControl->isRunning())
		path = mPlayControl->currentImage();
	QString info(path);
	QImage image(path);
	info += "<p style='font-size:16px;'><span style='font-weight:bold;color:black'>" + tr("Size") + ": </span>" + QString::number(QFile(mPlayPageRoot->imagePath()).size()) + "byte</p>";
	info += "<p style='font-size:16px;'><span style='font-weight:bold;color:black'>" + tr("Depth") + ": </span>" + QString::number(image.depth()) + "</p>";
	info += "<p style='font-size:16px;'><span style='font-weight:bold;color:black'>" + tr("Width") + ": </span>" +  QString::number(image.width()) + "</p>";
	info += "<p style='font-size:16px;'><span style='font-weight:bold;color:black'>" + tr("Height") + ": </span>" + QString::number(image.height()) + "</p>";

	m.setBaseImageInfo(info);

	ExifReader exif;
	exif.loadFile(path);
	if (exif.hasData()) {
		info = "";
		ExifReader::TagInfo tags = exif.getIFD0Brief();
		if (exif.hasIFD0()) {
			QMap<QString, QString>::ConstIterator it;
			for (it = tags.begin(); it != tags.end(); ++it) {
				info += "<p style='font-size:16px;'><span style='font-weight:bold;color:black'>" + it.key() + ": </span>" + it.value() + "</p>";
			}
		}
		if (exif.hasIFDExif()) {
			tags = exif.getExifBrief();
			QMap<QString, QString>::ConstIterator it;
			for (it = tags.begin(); it != tags.end(); ++it) {
				info += "<p style='font-size:16px;'><span style='font-weight:bold;color:black'>" + it.key() + ": </span>" + it.value() + "</p>";
			}
		}
		m.setExifInfo(info);

		if (exif.hasIFDGPS()) {
			info = "";
			tags = exif.getGpsBrief();
			QMap<QString, QString>::ConstIterator it;
			for (it = tags.begin(); it != tags.end(); ++it) {
				info += "<p style='font-size:16px;'><span style='font-weight:bold;color:black'>" + it.key() + ": </span>" + it.value() + "</p>";
			}
			m.setGPSInfo(info);
		}
	}


	m.showBaseInfo();
	m.exec();
	//QMessageBox::information(0, tr("Image infomation"), info);
	mView->setCanTransform(true);
}

void UiManager::shareToWeibo()
{
	mView->setCanTransform(false);
	WeiboDialog w(mView->scene());
	w.setPos(qApp->desktop()->width()/2, 2);
	w.setImage(mPlayPageRoot->imagePath());
	w.setUser(Config::weiboUser);
	w.setPassword(Config::weiboPasswd);
    connect(mOk, SIGNAL(clicked()), &w, SLOT(sendWeiboWithPicture()));
	connect(mCancel, SIGNAL(clicked()), &w, SLOT(reject()));
	connect(&w, SIGNAL(finished(int)), this, SLOT(okCancelFinish()));
	w.exec();
	mView->setCanTransform(true);
}

void UiManager::showHelp()
{
	Tools::showTip("<span style='font-size:20px;'>" + HELP_TEXT + "</span>", true);
}

void UiManager::showAbout()
{
    Tools::showTip(about, true);
}

void UiManager::updateThumbItemAt(int index)
{
	int show_index = index + mThumbsCount;
	ezlog_debug("updateing thumb at %d. show on %d", index, show_index);
	int col = show_index / Config::thumbRows;
	int row = show_index % Config::thumbRows;

    ThumbInfo info(mThumbTask->thumbInfoAt(index));
	mPlayControl->addImage(info.path);
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
	//if not 0, 0, the scene will move right
	//TODO: itemsBoundingRect is slow. need a fast solution
	//qDebug("root children w=%d, scenew=%d", mCurrentPageRoot->childItems().size(), mView->scene()->items().size());
	mView->scene()->setSceneRect(QRectF(mView->scene()->sceneRect() | item->boundingRect())  //mView->scene()->itemsBoundingRect()
								 .adjusted(0, 0, Config::contentHMargin + X0, Config::contentVMargin));
	//mView->scene()->setSceneRect(mCurrentPageRoot->childrenBoundingRect().adjusted(-Config::contentHMargin, -Config::contentVMargin, Config::contentHMargin, Config::contentVMargin));
	//mView->scene()->setSceneRect(mView->scene()->.adjusted(-Config::contentHMargin, -Config::contentVMargin, Config::contentHMargin, Config::contentVMargin));
}

void UiManager::updateDisplayedThumbList()
{
	mPlayControl->setImages(*ThumbRecorder::instance()->displayedThumbs()); //current record
	mThumbsCount = ThumbRecorder::instance()->displayedThumbs()->size();
    qDebug("total %d", mThumbsCount);
}

void UiManager::gotoPage(PageType pageType, const QString& image)
{
	if (page == pageType)
		return;
	if (pageType == ThumbPage) {
        mCurrentPageRoot = mThumbPageRoot;
		mThumbPageRoot->show();		
		//TODO: itemsBoundingRect is slow. need a fast solution
		QRectF r = mView->scene()->itemsBoundingRect()
				.adjusted(0, 0, Config::contentHMargin + X0, Config::contentVMargin);
		mView->scene()->setSceneRect(r.translated(-r.x(), -r.y()));
        if (mPlayControl->isRunning()) {
            mPlayControl->stop();
        }
		if (page == PlayPage) {
			mPlayPageRoot->smoothScale(1, 0.2, ItemAnimation::FadeOut);
			hideMenu(kPlayPageMenu);
		} else if (page == SearchPage) {
			mSearchInput->hide();
			mSearchPageRoot->hide(); //fadeout
			if (UiManager::lastHoverThumb)
				UiManager::lastHoverThumb->zoom(ThumbItem::ZoomOut);
			hideMenu(kSearchPageMenu);
		}
		showMenu(kThumbPageMenu);
	} else if (pageType == PlayPage) {
        mCurrentPageRoot = mPlayPageRoot;
		mThumbPageRoot->hide();
		mPlayPageRoot->setImagePath(image);
        mPlayPageRoot->smoothScale(4, 1, ItemAnimation::FadeIn);
		hideMenu(kThumbPageMenu);
		showMenu(kPlayPageMenu);
        //mPlayPageRoot->setPos(mView->mapToScene(QPoint())); //keep it center.
        //the following line will let it keep center. why?
		mView->scene()->setSceneRect(qApp->desktop()->rect());//mPlayPageRoot->boundingRect().adjusted(-32, -32, 32, 32));
	} else if (pageType == SearchPage) {
		mCurrentPageRoot->hide();
		mCurrentPageRoot = mSearchPageRoot;
		mCurrentPageRoot->show();
		hideMenu(kThumbPageMenu);
		showMenu(kSearchPageMenu);
		if (!mSearchInput) {
			mSearchInput = new TextEdit;
            mSearchInput->setSingleLine(true);
			mSearchInput->setFlag(QGraphicsItem::ItemIgnoresTransformations);
			mSearchInput->resize(qApp->desktop()->width()*2/3, 50);
			mSearchInput->setPos(qApp->desktop()->width()/3, 8);
			mView->scene()->addItem(mSearchInput);
            connect(mSearchInput, SIGNAL(submit()), SLOT(searchGoogleImage()));
		}
		mSearchInput->show();
		if (UiManager::lastHoverThumb)
			UiManager::lastHoverThumb->zoom(ThumbItem::ZoomOut); //TODO: keep zoom
		//TODO: itemsBoundingRect is slow. need a fast solution
		QRectF r = mView->scene()->sceneRect()  //mView->scene()->itemsBoundingRect()//mCurrentPageRoot->childrenBoundingRect()
				.adjusted(0, 0, Config::contentHMargin + X0, Config::contentVMargin);
		if (mCurrentPageRoot->childItems().isEmpty())
			r = qApp->desktop()->rect();
		mView->scene()->setSceneRect(r);//.translated(-r.x(), -r.y()));
	}
	page = pageType;
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
	QString menuText(button->text()); //plain text
    if (menuText == QUIT) {
		mThumbTask->stop();
		ThumbRecorder::instance()->save();
		qApp->quit();
    } else if (menuText == SETUP) {
		if (page == ThumbPage)
			hideMenu(kThumbPageMenu);
		else if (page == PlayPage)
			hideMenu(kPlayPageMenu);
		showMenu(kBackMenu);
		showMenu(kConfigMenu);
		//TODO: cancel the setup
        connect(mBack, SIGNAL(clicked()), this, SLOT(hideConfigMenu()));
        if (score->hasQueuedMovies()){
			score->playQue();
		}
		return;
	} else if (menuText == CLEAR_CACHE) {
        QDir thumbDir(Config::thumbDir);
        QStringList files = thumbDir.entryList(QDir::Files | QDir::Hidden);
		bool ok = true;
        foreach(QString f, files) {
			ok &= QFile::remove(thumbDir.filePath(f));
        }
		QFile::remove(Config::thumbRecordFile); //ok &=
		if (ok)
			Tools::showOk(1000);
		else
			Tools::showError(1000);
        return;
	} else if (menuText == HELP) {
		showHelp();
	}
	//page depended menu items
	if (page == ThumbPage) {
        //playReverse()
        if (button->buttonType() == Button::ArrowUp) {
			score->queueMovie(kThumbPageMenu + " -collapse");
            button->setButtonType(Button::ArrowDown);
        } else if (button->buttonType() == Button::ArrowDown) {
			score->queueMovie(kThumbPageMenu);
            button->setButtonType(Button::ArrowUp);
		} else if (menuText == GOOGLE_SEARCH) {
			gotoPage(SearchPage);
			return;
		} else if (menuText == ADDIMAGES) {
            addImages();
        } else if (menuText == ADDDIRS) {
            addImagesFromDir();
		} else if (menuText == CLEAR) {
			clearThumbs();
        } else if (menuText == ABOUT_QT) {
            qApp->aboutQt();
        }
        else {
			score->queueMovie(kThumbPageMenu + " -shake");
        }
	} else if (page == PlayPage) {
        if (menuText == BACK) {
            gotoPage(ThumbPage);
            return;
        } else if (menuText == START_STOP_SLIDE) {
            if (mPlayControl->isRunning())
                stopSlide();
            else
                startSlide();
        } else if (menuText == WEIBO_SHARE) {
			hideMenu(kPlayPageMenu);
			showMenu(kOkCancelMenu); //reverse when dialog is done
            shareToWeibo();
		} else if (menuText == IMAGE_INFO) {
			showCurrentImageInfo();
		}
        //playReverse()
        if (button->buttonType() == Button::ArrowUp) {
			score->queueMovie(kPlayPageMenu + " -collapse");
            button->setButtonType(Button::ArrowDown);
        } else if (button->buttonType() == Button::ArrowDown) {
			score->queueMovie(kPlayPageMenu);
            button->setButtonType(Button::ArrowUp);
        } else {
			score->queueMovie(kPlayPageMenu + " -shake");
        }
	} else if (page == SearchPage) {
		if (menuText == BACK) {
			gotoPage(ThumbPage);
			return;
		} else if (menuText == SEARCH) {
			searchGoogleImage();
		}
	}

    if (score->hasQueuedMovies()){
        score->playQue();
    }

}

void UiManager::okCancelFinish()
{
	hideMenu(kOkCancelMenu);
	if (page == PlayPage) {
		showMenu(kPlayPageMenu);
	} else if (page == ThumbPage) {
		showMenu(kThumbPageMenu);
	}
    disconnect(this, SLOT(okCancelFinish()));
}

void UiManager::hideConfigMenu()
{
	hideMenu(kBackMenu);
	if (page == PlayPage) {
		showMenu(kPlayPageMenu);
	} else if (page == ThumbPage) {
		showMenu(kThumbPageMenu);
	}
	hideMenu(kConfigMenu);
    disconnect(this, SLOT(hideConfigMenu())); //
}

void UiManager::searchGoogleImage()
{
	mSearchImageIndex = 0;
	UiManager::lastHoverThumb = 0; //TODO: 2 page is diffrent
	//clear previous images
	QList<QGraphicsItem*> cs = mSearchPageRoot->childItems();
	foreach(QGraphicsItem* c, cs) {
		mView->scene()->removeItem(c);
		delete c;
	}
	if (!mGoogleSearcher) {
		mGoogleSearcher = new GoogleImageSearcher(this);
		connect(mGoogleSearcher, SIGNAL(imageReady(ImageBaseInfo)), this, SLOT(showOnlineImage(ImageBaseInfo)));
	}
	mGoogleSearcher->setNameFilter(mSearchInput->text());
	while (mGoogleSearcher->canFetchMore())
		mGoogleSearcher->fetchMore();

	Tools::showTip(tr("Please wait"), true, 3000);
}

void UiManager::showOnlineImage(const ImageBaseInfo &image)
{
	int col = mSearchImageIndex / Config::thumbRows;
	int row = mSearchImageIndex % Config::thumbRows;
	mSearchImageIndex++;
	//TODO: if too small
	qreal w = image.thumbWidth, h = image.thumbHeight;
	if (w > Config::thumbItemWidth) {
		h = Config::thumbItemWidth * h/w; //set image width to mMaxWidth;
		w = Config::thumbItemWidth;
	}
	if (h > Config::thumbItemHeight) {
		w = Config::thumbItemHeight * w/h; //set image height to mMaxHeight;
		h = Config::thumbItemHeight;
	}
	ThumbItem *item = new ThumbItem(mSearchPageRoot);
	item->resize(w, h);
	item->setOnlineImage(true);
	//show item first then download
	item->setPos(col * (Config::thumbItemWidth + (Config::thumbBorder + Config::thumbMargin)*2 + Config::thumbSpacing)
				 + Config::thumbBorder + Config::thumbMargin + (Config::thumbItemWidth - item->boundingRect().width())/2
				 , row * (Config::thumbItemHeight + (Config::thumbBorder + Config::thumbMargin)*2 + Config::thumbSpacing)
				 + Config::thumbBorder + Config::thumbMargin + (Config::thumbItemHeight - item->boundingRect().height())/2);
	//addItem(item);
	if (row == Config::thumbRows - 1) {
		new ReflectEffectItem(item, ReflectEffectItem::MirrorBottom);
	}
	//TODO: itemsBoundingRect is slow. need a fast solution
	mView->scene()->setSceneRect(QRectF(mView->scene()->sceneRect() | item->boundingRect())  //mView->scene()->itemsBoundingRect() //mCurrentPageRoot->childrenBoundingRect()
								 .adjusted(-Config::contentHMargin, -Config::contentVMargin, Config::contentHMargin, Config::contentVMargin));
	item->setThumbPath(image.thumbPath);
	item->setOriginImage(image.path);
}

//TODO: setData() data()
void UiManager::createMenus()
{
    //thumb page menus
	static Movie *thumbPageMenuMovieIn = score->insertMovie(kThumbPageMenu);
	static Movie *thumbPageMenuCollapse = score->insertMovie(kThumbPageMenu + " -collapse");
	static Movie *thumbPageMovieOut = score->insertMovie(kThumbPageMenu + " -out");
	static Movie *thumbPageMovieShake = score->insertMovie(kThumbPageMenu + " -shake");
	QStringList thumbPageMenuItems;
    thumbPageMenuItems << SETUP << ABOUT_QT << HELP << CLEAR << GOOGLE_SEARCH << ADDIMAGES << ADDDIRS << QUIT;
    Button *menuItem = 0;
	for (int i = 0; i < thumbPageMenuItems.size(); ++i) {
		menuItem = new Button("<p style='color:white;font-size:18px'>" + thumbPageMenuItems[i] + "</p>");
        mView->scene()->addItem(menuItem);
        connect(menuItem, SIGNAL(clicked()), SLOT(clickMenuItem()));
        createLeftMenuTopInMovie(menuItem, i, true, thumbPageMenuMovieIn, thumbPageMenuCollapse, thumbPageMovieOut, thumbPageMovieShake);
	}
    menuItem = new Button(Button::ArrowUp, Button::RectShape);
	menuItem->setColor(Qt::blue);
	connect(menuItem, SIGNAL(clicked()), SLOT(clickMenuItem()));
    mView->scene()->addItem(menuItem);
    createLeftMenuTopInMovie(menuItem, thumbPageMenuItems.size() + 1, false , thumbPageMenuMovieIn, thumbPageMenuCollapse, thumbPageMovieOut, thumbPageMovieShake);

    //play page menus
	static Movie *playPageMenuMovieIn = score->insertMovie(kPlayPageMenu);
	static Movie *playPageMovieCollapse = score->insertMovie(kPlayPageMenu + " -collapse");
	static Movie *playPageMovieOut = score->insertMovie(kPlayPageMenu + " -out");
	static Movie *playPageMovieShake = score->insertMovie(kPlayPageMenu + " -shake");
    QStringList playPageMenuItems;
	playPageMenuItems << SETUP << HELP << IMAGE_INFO << START_STOP_SLIDE << WEIBO_SHARE << BACK << QUIT;
    menuItem = 0;
    for (int i = 0; i < playPageMenuItems.size(); ++i) {
		menuItem = new Button("<p style='color:white;font-size:18px'>" + playPageMenuItems[i] + "</p>");
        mView->scene()->addItem(menuItem);
        connect(menuItem, SIGNAL(clicked()), SLOT(clickMenuItem()));
        createLeftMenuTopInMovie(menuItem, i, true, playPageMenuMovieIn, playPageMovieCollapse, playPageMovieOut, playPageMovieShake);
    }
    menuItem = new Button(Button::ArrowUp, Button::RectShape);
	menuItem->setColor(Qt::blue);
    connect(menuItem, SIGNAL(clicked()), SLOT(clickMenuItem()));
    mView->scene()->addItem(menuItem);
    createLeftMenuTopInMovie(menuItem, thumbPageMenuItems.size() + 1, false , playPageMenuMovieIn, playPageMovieCollapse, playPageMovieOut, playPageMovieShake);


	//Config menu. right side
	static Movie *configMenuMovieIn = score->insertMovie(kConfigMenu);
	static Movie *configMenuMovieOut = score->insertMovie(kConfigMenu + " -out");
	QStringList configMenuItems;
	configMenuItems << "OpenGL " + Config::glVersion << CLEAR_CACHE;
	menuItem = 0;
	for (int i = 0; i < configMenuItems.size(); ++i) {
		menuItem = new Button("<p style='color:white;font-size:18px'>" + configMenuItems[i] + "</p>");
		mView->scene()->addItem(menuItem);
		connect(menuItem, SIGNAL(clicked()), SLOT(clickMenuItem()));
		createConfigMenuMovie(menuItem, i, configMenuMovieIn, configMenuMovieOut);
	}

	static Movie *okCancelMenuMovieIn = score->insertMovie(kOkCancelMenu);
	static Movie *okCancelMenuMovieOut = score->insertMovie(kOkCancelMenu + " -out");
	mOk = new Button("<p style='color:white;font-size:24px'>" + tr("Ok") + "</p>");
	mOk->setColor(Qt::blue);
	mOk->resize(222, 55);
	mView->scene()->addItem(mOk);
	mCancel = new Button("<p style='color:white;font-size:24px'>" + tr("Cancel") + "</p>");
	mCancel->setColor(Qt::blue);
	mCancel->resize(222, 55);
	mView->scene()->addItem(mCancel);

	createOkCancelMovie(mOk, 0, okCancelMenuMovieIn, okCancelMenuMovieOut);
	createOkCancelMovie(mCancel, 1, okCancelMenuMovieIn, okCancelMenuMovieOut);

	static Movie *backMenuMovieIn = score->insertMovie(kBackMenu);
	static Movie *backMenuMovieOut = score->insertMovie(kBackMenu + " -out");
	mBack = new Button("<p style='color:white;font-size:24px'>" + BACK + "</p>");
    mBack->setColor(QColor(100, 40, 8));
    mBack->resize(222, 55);
    mView->scene()->addItem(mBack);

    createBackButtonMovie(mBack, backMenuMovieIn, backMenuMovieOut);

	//Config menu. right side
	static Movie *searchPageMenuMovieIn = score->insertMovie(kSearchPageMenu);
	static Movie *searchPageMenuMovieOut = score->insertMovie(kSearchPageMenu + " -out");
	QStringList searchPageMenuItems;
	searchPageMenuItems << SEARCH << BACK;
	menuItem = 0;
	for (int i = 0; i < searchPageMenuItems.size(); ++i) {
		menuItem = new Button("<p style='color:white;font-size:24px'>" + searchPageMenuItems[i] + "</p>");
		menuItem->setColor(Qt::blue);
		menuItem->resize(222, 55);
		mView->scene()->addItem(menuItem);
		connect(menuItem, SIGNAL(clicked()), SLOT(clickMenuItem()));
		createOkCancelMovie(menuItem, i, searchPageMenuMovieIn, searchPageMenuMovieOut);
	}
}

void UiManager::createLeftMenuTopInMovie(Button *item, int i, bool hideOnFinished, Movie *movieIn, Movie *movieCollapse, Movie *movieOut, Movie *movieShake)
{
    int xOffset = 40;
    int yOffset = 24;

	item->setVisible(false);
	item->setZValue(10);
	qreal ih = item->sceneBoundingRect().height();
	qreal ihp = ih + 3;

	// create in-animation:
    DemoItemAnimation *anim = new DemoItemAnimation(item, DemoItemAnimation::ANIM_IN);
    anim->setDuration(float(600 + (i * 20)));
	anim->setStartPos(QPointF(xOffset, -ih));
	anim->setPosAt(0.20, QPointF(xOffset, -ih));
	//anim->setPosAt(0.50, QPointF(xOffset, (i * ihp) + yOffset + 22 + (10 * float(i / 4.0f))));
	anim->setPosAt(0.60, QPointF(xOffset, (i * ihp) + yOffset + 22));
	//anim->setPosAt(0.70, QPointF(xOffset, (i * ihp) + yOffset + 22 + (5 * float(i / 4.0f))));
	anim->setPosAt(0.80, QPointF(xOffset, (i * ihp) + yOffset + 22));
	anim->setPosAt(0.90, QPointF(xOffset, (i * ihp) + yOffset + 22 + (2 * float(i / 4.0f))));
	anim->setPosAt(1.00, QPointF(xOffset, (i * ihp) + yOffset + 22));
	movieIn->append(anim);

    // create collapse-animation:
    anim = new DemoItemAnimation(item, DemoItemAnimation::ANIM_OUT, hideOnFinished);
    anim->setDuration(float(600 + (i * 20)));
	anim->setStartPos(QPointF(xOffset,  (i * ihp) + yOffset + 22));
    anim->setPosAt(1.00, QPointF(xOffset, yOffset));
	anim->setPosAt(0.90, QPointF(xOffset, (i * ihp) + yOffset + 22 + (10 * float(i / 4.0f))));
	anim->setPosAt(0.80, QPointF(xOffset, (i * ihp) + yOffset + 22));
	//anim->setPosAt(0.70, QPointF(xOffset, (i * ihp) + yOffset + 22 + (5 * float(i / 4.0f))));
	anim->setPosAt(0.60, QPointF(xOffset, (i * ihp) + yOffset + 22));
	//anim->setPosAt(0.50, QPointF(xOffset, (i * ihp) + yOffset + 22 + (2 * float(i / 4.0f))));
	anim->setPosAt(0.20, QPointF(xOffset, (i * ihp) + yOffset + 22));
    movieCollapse->append(anim);

    QPointF p(qApp->desktop()->rect().bottomRight());//(mView->mapToScene(mView->rect().bottomRight())); //view's rect not ready?
    qreal sw = p.x();
    qreal sh = p.y();
    qreal iw = item->sceneBoundingRect().width();
    anim = new DemoItemAnimation(item, DemoItemAnimation::ANIM_OUT);
	anim->hideOnFinished = true;
    anim->setDuration((600 + (30 * i)));
	anim->setStartPos(QPointF(xOffset, (i * ihp) + yOffset + 22));
    anim->setPosAt(0.60, QPointF(xOffset, sh - ih - ih));
    anim->setPosAt(0.65, QPointF(xOffset + 20, sh - ih));
    anim->setPosAt(1.00, QPointF(sw + iw, sh - ih));
	movieOut->append(anim);

	// create shake-animation:
	anim = new DemoItemAnimation(item);
    anim->setDuration(600);
	anim->setStartPos(QPointF(xOffset, (i * ihp) + yOffset + 22));
	anim->setPosAt(0.55, QPointF(xOffset, (i * ihp) + yOffset + 22 - i*2.0));
	anim->setPosAt(0.70, QPointF(xOffset - 10, (i * ihp) + yOffset + 22 - i*1.5));
	anim->setPosAt(0.80, QPointF(xOffset, (i * ihp) + yOffset + 22 - i*1.0));
	anim->setPosAt(0.90, QPointF(xOffset - 2, (i * ihp) + yOffset + 22 - i*0.5));
	anim->setPosAt(1.00, QPointF(xOffset, (i * ihp) + yOffset + 22));
	movieShake->append(anim);
}

//TODO: viewport rect
void UiManager::createConfigMenuMovie(Button *item, int i, Movie *movieIn, Movie *movieOut)
{
	static qreal w = qApp->desktop()->width();
	int xOffset = w - item->width() - 22;
	int yOffset = 88;

	item->setVisible(false);
	item->setZValue(10);
	qreal ih = item->sceneBoundingRect().height();
	qreal ihp = ih + 8;

	// create in-animation:
	DemoItemAnimation *anim = new DemoItemAnimation(item, DemoItemAnimation::ANIM_IN);
	anim->setDuration(float(600 + (i * 20)));
	anim->setStartPos(QPointF(w + 22, yOffset + i*ihp));
	anim->setPosAt(1.00, QPointF(xOffset, yOffset + i*ihp));
	movieIn->append(anim);

	anim = new DemoItemAnimation(item, DemoItemAnimation::ANIM_OUT);
	anim->hideOnFinished = true;
	anim->setDuration((600 + (30 * i)));
	anim->setStartPos(QPointF(xOffset, yOffset + i*ihp));
	anim->setPosAt(1.00, QPointF(w + 22, yOffset + i*ihp));
	movieOut->append(anim);
}

void UiManager::createOkCancelMovie(Button *item, int index, Movie *movieIn, Movie *movieOut)
{
	if (index > 1)
		return;
	item->hide();
	item->setZValue(12);

	int xOffset = 40;
	qreal space = 24;
	qreal mh = qApp->desktop()->height()/2;
	qreal x0 = -item->width() -22;
	qreal y0 = index == 0 ? item->height() : qMax(qApp->desktop()->height() - 2*item->height(), mh + 2*item->height());
	DemoItemAnimation *anim = new DemoItemAnimation(item, DemoItemAnimation::ANIM_IN);
	anim->setDuration(600);
	anim->setStartPos(QPointF(x0, y0));
	anim->setPosAt(0.20, QPointF(xOffset, y0));
	anim->setPosAt(0.40, QPointF(xOffset, y0));
	if (index == 0) {
		anim->setPosAt(0.90, QPointF(xOffset, qMax<qreal>(mh - item->height()*2, 44)));
		anim->setPosAt(1.00, QPointF(xOffset, mh - item->height() - space/2));
	} else {
		anim->setPosAt(0.90, QPointF(xOffset, qMin(mh + item->height(), 2*mh - 44)));
		anim->setPosAt(1.00, QPointF(xOffset, mh + space/2));
	}
	movieIn->append(anim);

	anim = new DemoItemAnimation(item, DemoItemAnimation::ANIM_OUT);
	anim->setDuration(600);

	qreal y1 = index == 0 ? mh - item->height() - space/2 : mh + space/2;
	anim->setStartPos(QPointF(xOffset, y1));
	/*if (index == 0) {
		anim->setPosAt(0.20, QPointF(xOffset, qMax<qreal>(mh - item->height()*2, 44)));
	} else {
		anim->setPosAt(0.20, QPointF(xOffset, qMin(mh + item->height(), 2*mh - 44)));
	}*/
	anim->setPosAt(0.314, QPointF(xOffset, y0));
	anim->setPosAt(0.618, QPointF(xOffset, y0));
	anim->setPosAt(1.00, QPointF(x0, y0));
	movieOut->append(anim);
}


void UiManager::createBackButtonMovie(Button *item, Movie *movieIn, Movie *movieOut)
{
    createOkCancelMovie(item, 1, movieIn, movieOut);
}

void UiManager::showMenu(const QString &menu)
{
	score->queueMovie(menu);
    //score->queueMovie(menu + " -out", Score::NEW_ANIMATION_ONLY);
    //score->queueMovie(menu + " -buttons -out", Score::NEW_ANIMATION_ONLY);
    //score->queueMovie("upndown -shake");
	score->queueMovie(menu + " -shake");
	if (score->hasQueuedMovies()){
		score->playQue();
	}
}

void UiManager::hideMenu(const QString &menu)
{
    score->queueMovie(menu + " -out", Score::NEW_ANIMATION_ONLY);
    if (score->hasQueuedMovies()){
        score->playQue();
    }
}
} //namespace PhotoKit

