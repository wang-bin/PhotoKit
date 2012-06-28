/******************************************************************************
	UiManager.h: description
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


#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
class QGraphicsWidget;
class QGraphicsItem;
namespace PhotoKit {
class ToolTip;
class PhotoKitView;
class ThumbItem;
class ThumbTask;
class ToolBar;
class UiManager : public QObject
{
	Q_OBJECT
public:
	enum PageType {
		CategoryPage, WallPage, PlayPage, InfoPage, ConfigPage, HelpPage
	};
	static UiManager* instance();
    virtual ~UiManager();
	void init(PhotoKitView *view);
    QGraphicsItem* rootItem();
    ToolTip* toolTipItem();
    void updateFixedItems();

    void showImagesFromThumb(const QString& dir, bool yes = true);
    void showImagesFromThumb(const QStringList& paths, bool yes = true);

    static ThumbItem *lastHoverThumb;
signals:
	
public slots:
    void updateThumbItemAt(int index);

private:
	explicit UiManager(QObject *parent = 0);

	static UiManager *mInstance;
    QGraphicsWidget *mRoot; //mWallRoot
    ToolBar *mBottomBar;
    ToolTip *mToolTip;
	PhotoKitView *mView;
    ThumbTask *mThumbTask;
};
} //namespace PhotoKit
#endif // UIMANAGER_H
