/******************************************************************************
	QPut: make post easy
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

#ifndef PHOTOKIT_QPUT_H
#define PHOTOKIT_QPUT_H

#include <QtNetwork/QSslError>
#include <QtCore/QUrl>
class QNetworkAccessManager;

namespace PhotoKit {
class QPut : public QObject
{
	Q_OBJECT
public:
	QPut(QObject* parent = 0);
	explicit QPut(const QUrl &pUrl, QObject* parent = 0);
    ~QPut();

	void reset(); //call it before a new post
	//equals setSnapshotData(pData) then start()
    void addTextPart(const QByteArray& name, const QByteArray& body);
    void addDataPart(const QByteArray& mine, const QByteArray& name, const QByteArray& data, const QString& fileName = QString());
    void upload();
    void post();

    void setData(const QByteArray& pData);
    void setUrl(const QUrl& pUrl);

signals:
	void fail(const QString& error);
	void ok(const QByteArray& replyData);
public slots:
	void abort();
private slots:
	void DoFinished();
	void DoReplyError();

private:
	void init();

	QNetworkAccessManager *mNetwork;
    QUrl mUrl;
	bool mSuccess;
    QByteArray mData;
    QByteArray mTextPart, mDataPart;
    QByteArray mBoundary;
};
} //namespace PhotoKit
#endif // PHOTOKIT_QPUT_H
