/******************************************************************************
	WeiboBox.cpp: description
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


#include "WeiboBox.h"
#include <QtCore/QSettings>
#include <QTextDocument>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include "TextEdit.h"
#include "Dialog_p.h"
#include "Button.h"
#include "tools/Tools.h"
#include "weiboapi.h"
#include "Config.h"
#include "ezlog.h"

namespace PhotoKit {

class WeiboBoxPrivate : public DialogPrivate
{
public:
	WeiboBoxPrivate() {
		api = new WeiboApi;
		QGraphicsTextItem *title = new QGraphicsTextItem(titleBar);
		title->setDefaultTextColor(QColor(250, 128, 10));
		QFont f;
		f.setPixelSize(23);
		title->setFont(f);
		title->setPlainText(QObject::tr("New weibo (Sina) with current image"));
		qreal w = qApp->desktop()->width()/2 - 44;
		statusEdit = new TextEdit(central);
		statusEdit->setDefaultTextColor(Qt::blue);
		statusEdit->document()->setDocumentMargin(22);

		qreal x = 11, y = 22;
		statusEdit->setPos(x, y);
		statusEdit->document()->setTextWidth(qApp->desktop()->width()/2 - 8);
		//statusEdit->setPlainText("Write something...");
		qreal h = qApp->desktop()->height()*2/3;
		statusEdit->resize(w, h);
		y += statusEdit->size().height() + 8;
		f.setBold(true);
		QGraphicsTextItem *user = new QGraphicsTextItem(central);
		user->setFont(f);
		user->document()->setDocumentMargin(8);
		user->setDefaultTextColor(QColor(33, 33, 33));
		user->setPlainText(QObject::tr("User"));
		user->setPos(x, y);
		userEdit = new TextEdit(central);
		userEdit->setPos(x + 120, y);
		userEdit->resize(w - 120, 66);
		userEdit->setDefaultTextColor(Qt::black);
		userEdit->document()->setDocumentMargin(8);
		userEdit->document()->setTextWidth(qApp->desktop()->width()/2 - 8);
		y += 66 + 2;
		QGraphicsTextItem *passwd = new QGraphicsTextItem(central);
		passwd->setFont(f);
		passwd->document()->setDocumentMargin(8);
		passwd->setDefaultTextColor(QColor(33, 33, 33));
		passwd->setPlainText(QObject::tr("Password"));
		passwd->setPos(x, y);
		passwdEdit = new TextEdit(central);
		passwdEdit->setPos(x + 120, y);
		passwdEdit->resize(w - 120, 66);
		passwdEdit->setDefaultTextColor(Qt::black);
		passwdEdit->document()->setDocumentMargin(8);
		statusEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
		userEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
		passwdEdit->setTextInteractionFlags(Qt::TextEditorInteraction);

		userEdit->setFont(f);
		passwdEdit->setFont(f);
		passwdEdit->setEchoMode(TextEdit::Password);
	}
	~WeiboBoxPrivate() {
		if (api) {
			delete api;
			api = 0;
		}
	}

	void setupUi(WeiboBox *ui) {
		QObject::connect(api, SIGNAL(error(QString)), ui, SLOT(doError(QString)));
		QObject::connect(api, SIGNAL(loginOk()), ui, SLOT(sendWeiboWithPicture()));
		ui->resize(qApp->desktop()->width()/2 - 11, qApp->desktop()->height() -22);
	}

	//Button *ok, *cancel; //left side of screen
	TextEdit *userEdit, *passwdEdit;
	TextEdit *statusEdit;
	QString image;

	WeiboApi *api;
};

WeiboBox::WeiboBox(QGraphicsScene *scene, QGraphicsItem *parent) :
	Dialog(*new WeiboBoxPrivate, scene, parent)
{
	Q_D(WeiboBox);
	d->WeiboBoxPrivate::setupUi(this);
}

WeiboBox::~WeiboBox()
{

}

void WeiboBox::setUser(const QString &user)
{
	Q_D(WeiboBox);
	d->userEdit->setPlainText(user);
}

void WeiboBox::setPassword(const QString &passwd)
{
	Q_D(WeiboBox);
	d->passwdEdit->setPlainText(passwd);
	//d->passwdEdit->setData(0, passwd);
}

void WeiboBox::setImage(const QString &path)
{
	Q_D(WeiboBox);
	d->image = path;
}

void WeiboBox::doError(const QString &error)
{
	Tools::showError(3000);
}

void WeiboBox::loginOrSend()
{/*
	if (mContent->currentWidget() == mWeiboPage)
		sendWeiboWithPicture();
	else
		login();
*/
	login();
}

void WeiboBox::login()
{
	Q_D(WeiboBox);
	if (d->userEdit->text().isEmpty() || d->passwdEdit->text().isEmpty())
		QMessageBox::warning(0, tr("Error"), tr("User name and password can't be empty"));
	d->api->setUSer(d->userEdit->toPlainText());
	d->api->setPassword(d->passwdEdit->data(0).toString());
	d->api->login();
}

void WeiboBox::sendOk()
{
	accept();
	Tools::showOk(3000);
}

void WeiboBox::sendWeiboWithPicture()
{
	Q_D(WeiboBox);
	QSettings cfg(Config::configPath, QSettings::IniFormat);
	cfg.setIniCodec("UTF-8");
	Config::weiboUser = d->userEdit->text();
	Config::weiboPasswd = d->passwdEdit->text();
	qDebug("%s %s", qPrintable(Config::weiboUser), qPrintable(Config::weiboPasswd));
	cfg.setValue("weiboUser", qCompress(Config::weiboUser.toAscii()));
	cfg.setValue("weiboPasswd", qCompress(Config::weiboPasswd.toAscii()));
	QString text = d->statusEdit->text();
	if (text.isEmpty())
		QMessageBox::warning(0, tr("Error"), tr("Weibo can't be empty"));
	if (text.size() > 140)
		QMessageBox::warning(0, tr("Error"), tr("Weibo caontent is limited to 140. now are %1").arg(text.size()));

	d->api->updateStatusWithPicture(text, d->image);
	connect(d->api, SIGNAL(ok()), SLOT(sendOk()));//TODO: ok message
}

} //namespace PhotoKit
