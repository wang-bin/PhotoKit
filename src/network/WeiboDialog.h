/******************************************************************************
	WeiboDialog: QGraphicsItem based weibo dialog
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


#ifndef PHOTOKIT_WEIBOBOX_H
#define PHOTOKIT_WEIBOBOX_H

#include "Dialog.h"

namespace PhotoKit {
class WeiboDialogPrivate;
class WeiboDialog : public Dialog
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(WeiboDialog)
public:
	explicit WeiboDialog(QGraphicsScene *scene, QGraphicsItem *parent = 0);
	~WeiboDialog();

	void setUser(const QString& user);
	void setPassword(const QString& passwd);
	void setImage(const QString& path);

	//void showLogin();

public slots:
	void doError(const QString& error);
    //void loginOrSend();
	//void cancel();
	void login();
	void sendWeiboWithPicture();
    void loginDone();

private slots:
	void sendOk();

};

} //namespace PhotoKit

#endif // PHOTOKIT_WEIBOBOX_H
