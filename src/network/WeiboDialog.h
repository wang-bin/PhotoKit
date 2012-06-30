/******************************************************************************
	WeiboDialog.h: description
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


#ifndef WEIBODIALOG_H
#define WEIBODIALOG_H

#include <QDialog>
class QPushButton;
class QLineEdit;
class QTextEdit;
class QStackedWidget;
namespace PhotoKit {
class WeiboApi;
class WeiboDialog : public QDialog
{
	Q_OBJECT
public:
	explicit WeiboDialog(QWidget *parent = 0);

	void setUser(const QString& user);
	void setPassword(const QString& passwd);
	void setImage(const QString& path);

	//void showLogin();
	
public slots:
	void doError(const QString& error);
	void loginOrSend();
	//void cancel();
	void login();
	void sendOk();
	void sendWeiboWithPicture();

private:
	QStackedWidget *mContent;
	QWidget *mLoginPage, *mWeiboPage;
	QPushButton *mOk, *mCancel;
	QLineEdit *mUserEdit, *mPasswdEdit;
	QTextEdit *mStatusEdit;
	QString mUser, mPasswd, mImage;

	WeiboApi *mApi;
};
} //namespace PhotoKit
#endif // WEIBODIALOG_H
