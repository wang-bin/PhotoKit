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
#include "TextEdit.h"
#include "Dialog_p.h"
#include "Button.h"
#include "tools/Tools.h"
#include "weiboapi.h"
#include "Config.h"
#include "ezlog.h"

namespace PhotoKit {

static QByteArray access_token;

class WeiboBoxPrivate : public DialogPrivate
{
public:
	WeiboBoxPrivate() {
		api = new WeiboApi;
		qreal x = 6;
		QGraphicsTextItem *title = new QGraphicsTextItem(titleBar);
		title->setPos(x, 12);
		title->setDefaultTextColor(Qt::white);
		QFont f;
		f.setPixelSize(23);
		f.setBold(true);
		title->setFont(f);
		title->setPlainText(QObject::tr("New weibo (Sina)"));
		qreal w = qApp->desktop()->width()/2 - 44;
		statusEdit = new TextEdit(central);
		statusEdit->setFont(f);
		statusEdit->setDefaultTextColor(Qt::blue);
		statusEdit->document()->setDocumentMargin(22);

		qreal y = 22;
		statusEdit->setPos(x, y);
		statusEdit->document()->setTextWidth(qApp->desktop()->width()/2 - 8);
		//statusEdit->setPlainText("Write something...");
        qreal h = qApp->desktop()->height()*0.618;
		statusEdit->resize(w, h);
		y += statusEdit->size().height() + 12;
        qreal hh = qMin<qreal>(qApp->desktop()->height()*0.314/2, 66); //TODO: calculate the line edit height
		f.setBold(true);
		QGraphicsTextItem *user = new QGraphicsTextItem(central);
        f.setPixelSize(20);
		user->setFont(f);
        user->document()->setDocumentMargin(4);
		user->setDefaultTextColor(QColor(33, 33, 33));
		user->setPlainText(QObject::tr("User"));
		user->setPos(x, y);
		userEdit = new TextEdit(central);
		userEdit->setPos(x + 120, y);
        userEdit->resize(w - 120, hh);
		userEdit->setDefaultTextColor(Qt::black);
        userEdit->document()->setDocumentMargin(11);
		userEdit->document()->setTextWidth(qApp->desktop()->width()/2 - 8);
        y += hh + 2;
		QGraphicsTextItem *passwd = new QGraphicsTextItem(central);
        passwd->setFont(f);
        passwd->document()->setDocumentMargin(4);
		passwd->setDefaultTextColor(QColor(33, 33, 33));
		passwd->setPlainText(QObject::tr("Password"));
		passwd->setPos(x, y);
		passwdEdit = new TextEdit(central);
		passwdEdit->setPos(x + 120, y);
        passwdEdit->resize(w - 120, hh);
		passwdEdit->setDefaultTextColor(Qt::black);
        passwdEdit->document()->setDocumentMargin(11);
		statusEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
		userEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
		passwdEdit->setTextInteractionFlags(Qt::TextEditorInteraction);

        f.setPixelSize(24);
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
        QObject::connect(api, SIGNAL(loginOk()), ui, SLOT(loginDone()));
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
	Q_UNUSED(error);
	Tools::showError(3000);
}

void WeiboBox::login()
{
	Q_D(WeiboBox);
	if (d->userEdit->text().isEmpty() || d->passwdEdit->text().isEmpty()) {
		Tools::showTip("<h2 style='color:red;text-align:center;'>" + tr("Error") + "</h2><p style='color:white;font-size:18px;'>"
					   + tr("User name and password can't be empty") + "</p>", true, 3000);
		return;
	}
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
    if (d->userEdit->text().isEmpty() && d->passwdEdit->text().isEmpty())
		Tools::showTip("<h2 style='color:red;text-align:center;'>" + tr("Error") + "</h2><p style='color:white;font-size:18px;'>"
					   + tr("User name and password can't be empty") + "</p>", true, 3000);
	//TODO: check whether the user changed. if yes, set empty token and login as new account
	QString userNow = d->userEdit->text();
	if (userNow != Config::weiboUser) {
		access_token.clear();
	}
	QString passwdNow = d->passwdEdit->text();
	d->api->setUSer(userNow);
	d->api->setPassword(passwdNow);
    d->api->setAccessToken(access_token);
	Config::weiboUser = userNow;
	Config::weiboPasswd = passwdNow;
	qDebug("%s %s", qPrintable(Config::weiboUser), qPrintable(Config::weiboPasswd));
	cfg.setValue("weiboUser", qCompress(Config::weiboUser.toAscii()));
	cfg.setValue("weiboPasswd", qCompress(Config::weiboPasswd.toAscii()));
	QString text = d->statusEdit->text();
	qDebug("content: %s", qPrintable(text));
	if (text.isEmpty()) {
		Tools::showTip("<h2 style='color:red;text-align:center;'>" + tr("Error") + "</h2><p style='color:white;font-size:18px;'>"
					   + tr("Weibo can't be empty") + "</p>", true, 3000);
		return;
	}
	if (text.size() > 140) {
		Tools::showTip("<h2 style='color:red;text-align:center;'>" + tr("Error") + "</h2><p style='color:white;font-size:18px;'>"
					   + tr("Weibo caontent is limited to 140. now are %1").arg(text.size()) + "</p>", true, 3000);
		return;
	}
	d->api->updateStatusWithPicture(text, d->image);
    connect(d->api, SIGNAL(sendOk()), SLOT(sendOk()));
}

void WeiboBox::loginDone()
{
    Q_D(WeiboBox);
    access_token = d->api->accessToken();
}

} //namespace PhotoKit
