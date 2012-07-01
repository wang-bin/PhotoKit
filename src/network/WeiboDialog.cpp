/******************************************************************************
	WeiboDialog.cpp: description
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


#include "WeiboDialog.h"
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>
#include <QStackedWidget>
#include <QToolTip>
#include <QScrollArea>
#include "Config.h"
#include "weiboapi.h"

namespace PhotoKit {
WeiboDialog::WeiboDialog(QWidget *parent) :
    QDialog(parent)
{
	setWindowTitle(tr("Send weibo with an image"));
	mApi = new WeiboApi(this);
	connect(mApi, SIGNAL(error(QString)), SLOT(doError(QString)));
	connect(mApi, SIGNAL(loginOk()), SLOT(sendWeiboWithPicture()));

	//mLoginPage = new QWidget;
	mUserEdit = new QLineEdit;
	mPasswdEdit = new QLineEdit;
	mPasswdEdit->setEchoMode(QLineEdit::Password);

	QHBoxLayout *hb_user = new QHBoxLayout;
	hb_user->addWidget(new QLabel(tr("User")));
	hb_user->addWidget(mUserEdit);
	QHBoxLayout *hb_pass = new QHBoxLayout;
	hb_pass->addWidget(new QLabel(tr("Password")));
	hb_pass->addWidget(mPasswdEdit);
/*
	QVBoxLayout *vb_login = new QVBoxLayout;
	vb_login->addLayout(hb_user);
	vb_login->addLayout(hb_pass);
	mLoginPage->setLayout(vb_login);
*/
//	mWeiboPage = new QWidget;
	mStatusEdit = new QTextEdit;
/*
	QVBoxLayout *vb_w = new QVBoxLayout;
	vb_w->addWidget(new QLabel(tr("New weibo")));
	vb_w->addWidget(mStatusEdit);
	mWeiboPage->setLayout(vb_w);

	mContent = new QStackedWidget;
	mContent->addWidget(mLoginPage);
	mContent->addWidget(mWeiboPage);
	mContent->setCurrentWidget(mWeiboPage);
*/
	mOk = new QPushButton(tr("Ok"));
	mCancel = new QPushButton(tr("Cancel"));
	connect(mOk, SIGNAL(clicked()), SLOT(loginOrSend()));
	connect(mCancel, SIGNAL(clicked()), SLOT(reject()));

	QHBoxLayout *hb = new QHBoxLayout;
	hb->addWidget(mOk);
	hb->addWidget(mCancel);

	QVBoxLayout *vb = new QVBoxLayout;
	vb->setSizeConstraint(QLayout::SetMaximumSize);
	vb->addWidget(new QLabel(tr("New weibo")));
	vb->addWidget(mStatusEdit);
	vb->addLayout(hb_user);
	vb->addLayout(hb_pass);
	//vb->addWidget(mContent);
	//vb->addLayout(hb);

	QWidget *content = new QWidget;
	content->setMinimumWidth(width() - 40);
	content->setLayout(vb);
	QScrollArea *area = new QScrollArea;
	area->setWidget(content);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(area);
	layout->addLayout(hb);
	setLayout(layout);
}

void WeiboDialog::setUser(const QString &user)
{
	mUser = user;
	mUserEdit->setText(mUser);
}

void WeiboDialog::setPassword(const QString &passwd)
{
	mPasswd = passwd;
	mPasswdEdit->setText(mPasswd);
}

void WeiboDialog::setImage(const QString &path)
{
	mImage = path;
}

void WeiboDialog::doError(const QString &error)
{
	QMessageBox::warning(0, tr("Error"), error);
}

void WeiboDialog::loginOrSend()
{/*
	if (mContent->currentWidget() == mWeiboPage)
		sendWeiboWithPicture();
	else
		login();
*/
	login();
}

void WeiboDialog::login()
{
	if (mUserEdit->text().isEmpty() || mPasswdEdit->text().isEmpty())
		QMessageBox::warning(0, tr("Error"), tr("User name and password can't be empty"));
	mApi->setUSer(mUserEdit->text());
	mApi->setPassword(mPasswdEdit->text());
	mApi->login();
}

void WeiboDialog::sendOk()
{
	QToolTip::showText(QPoint(), tr("Send weibo successfully"));
}

void WeiboDialog::sendWeiboWithPicture()
{
	QSettings cfg(Config::configPath, QSettings::IniFormat);
	cfg.setIniCodec("UTF-8");
	Config::weiboUser = mUserEdit->text();
	Config::weiboPasswd = mPasswdEdit->text();
	cfg.setValue("weiboUser", qCompress(Config::weiboUser.toAscii()));
	cfg.setValue("weiboPasswd", qCompress(Config::weiboPasswd.toAscii()));
	QString text = mStatusEdit->toPlainText();
	if (text.isEmpty())
		QMessageBox::warning(0, tr("Error"), tr("Weibo can't be empty"));
	if (text.size() > 140)
		QMessageBox::warning(0, tr("Error"), tr("Weibo caontent is limited to 140. now are %1").arg(text.size()));
	mApi->updateStatusWithPicture(text, mImage);
	connect(mApi, SIGNAL(ok()), SLOT(accept()));
}

} //namespace PhotoKit
