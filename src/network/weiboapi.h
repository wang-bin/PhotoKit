/******************************************************************************
	WeiboApi: login, logout and upload api
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

#ifndef PHOTOKIT_WEIBOAPI_H
#define PHOTOKIT_WEIBOAPI_H

#include <QObject>
namespace PhotoKit {
class QPut;
class WeiboApi : public QObject
{
    Q_OBJECT
public:
    explicit WeiboApi(QObject *parent = 0);
    void setUSer(const QString& user);
    void setPassword(const QString& passwd);
    void setAccessToken(const QByteArray& token);
    QByteArray accessToken() const;

	void login();
	void logout();
	void updateStatusWithPicture(const QString& status, const QString& fileName);

signals:
	void error(const QString& error);
	void loginOk();
	void loginFail();
	void sendOk();
private slots:
	void parseOAuth2ReplyData(const QByteArray& data);
	void sendStatusWithPicture();
private:
    QPut *mPut;
	QString mUser, mPasswd;
	QByteArray mAccessToken;
	QByteArray mUid;
	QString mStatus, mFile;
};
} //namespace PhotoKit
#endif // PHOTOKIT_WEIBOAPI_H
