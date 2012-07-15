/******************************************************************************
	UiManager: the manager
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


#ifndef PHOTOKIT_UIMANAGER_H
#define PHOTOKIT_UIMANAGER_H

#include <QObject>
#include "network/GoogleImageSearcher.h"
#include "score.h"
class QGraphicsView;
class QGraphicsWidget;
class QGraphicsItem;
class QMenu;
class QPoint;
class QTransform;
namespace PhotoKit {
//class ToolTip;
class PhotoKitView;
class ThumbItem;
class ThumbTask;
class ToolBar;
class SlideDisplay;
class SlidePlayControl;
class BaseItem;
class Button;
class Score;
class TextEdit;
class GoogleImageSearcher;
class UiManager : public QObject
{
	Q_OBJECT
public:
	enum PageType {
		CategoryPage, ThumbPage, PlayPage, SearchPage, InfoPage, ConfigPage
	};
	static UiManager* instance();
    virtual ~UiManager();
	void init(PhotoKitView *view);  //call createMenus
	BaseItem* currentPageRootItem();
    BaseItem* thumbPageRootItem();
    SlideDisplay* playPageItem() {return mPlayPageRoot;}
    QGraphicsView *view();
	bool isSliding() const;
    void updateFixedItems();

    void showImagesFromThumb(const QString& dir, bool yes = true);
    void showImagesFromThumb(const QStringList& paths, bool yes = true);

	void gotoPage(PageType pageType, const QString& image = QString());

	void tryMoveCenter(QGraphicsItem* item);

    static ThumbItem *lastHoverThumb;
	static PageType page;
signals:
	
public slots:
	//void transformItem(const QTransform& t);
	void clearThumbs();
	void addImagesFromDir();
	void addImages();
	void startSlide();
	void stopSlide();
	void showCurrentImageInfo();
	void shareToWeibo();
	void showHelp();
    void showAbout();

    void updateThumbItemAt(int index);
	void updateDisplayedThumbList();

private slots:
	void clickMenuItem(); //check sender's button text
	void okCancelFinish();
	void hideConfigMenu();
	void searchGoogleImage();
	void showOnlineImage(const ImageBaseInfo& image);

private:
	explicit UiManager(QObject *parent = 0);
	void createMenus(); //called by init()
	void createLeftMenuTopInMovie(Button *item, int i, bool hideOnFinished, Movie* movieIn, Movie *movieCollapse, Movie* movieOut, Movie *movieShake = 0);
	void createConfigMenuMovie(Button *item, int i, Movie* movieIn, Movie* movieOut);
	void createOkCancelMovie(Button *item, int index, Movie* movieIn, Movie *movieOut); //index 0: from top
    void createBackButtonMovie(Button *item, Movie* movieIn, Movie *movieOut);
	void showMenu(const QString& menu);
    void hideMenu(const QString& menu);

	static UiManager *mInstance;
	BaseItem *mCurrentPageRoot;
	BaseItem *mThumbPageRoot, *mSearchPageRoot;
    SlideDisplay *mPlayPageRoot;
	SlidePlayControl *mPlayControl;
	ToolBar *mBottomBar;
	PhotoKitView *mView;
    ThumbTask *mThumbTask;
	int mThumbsCount;
	Score *score;

	Button *mOk, *mCancel; //for connecting signals and slots with dialogs
    Button *mBack;
	TextEdit *mSearchInput;
	GoogleImageSearcher *mGoogleSearcher;
	int mSearchImageIndex;
};
} //namespace PhotoKit
#endif // PHOTOKIT_UIMANAGER_H
