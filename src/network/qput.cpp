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

#include "qput.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslConfiguration>
#include <QtCore/QCoreApplication>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QMapIterator>
#include <QtCore/QList>

namespace PhotoKit {
QPut::QPut(QObject *parent)
	:QObject(parent)
{
	init();
}

QPut::QPut(const QUrl& pUrl, QObject *parent)
	:QObject(parent),mUrl(pUrl)
{
	init();
}

QPut::~QPut()
{
}

void QPut::reset()
{
	mTextPart.clear();
	mDataPart.clear();
	mData.clear();
	mUrl.clear();
	mSuccess = false;
}

//body should be url encoded
void QPut::addTextPart(const QByteArray &name, const QByteArray &body)
{
    mTextPart += "--" + mBoundary + "\r\n"; //can be multiple text part?
    mTextPart += "Content-Disposition: form-data; name=\"" + name + "\"\r\n\r\n";
    mTextPart += body;
    mTextPart += "\r\n";
}

void QPut::addDataPart(const QByteArray &mine, const QByteArray &name, const QByteArray &data, const QString& fileName)
{
	mDataPart += "--" + mBoundary + "\r\n";
    mDataPart += "Content-Disposition: form-data; name=\"" + name + "\"";
    if (!fileName.isEmpty())
        mDataPart += "; filename=\"" + QUrl::toPercentEncoding(fileName) + "\"";
    mDataPart += "\r\n";
    mDataPart += "Content-Type: " + mine + "\r\n";
    mDataPart += "Content-Transfer-Encoding: binary\r\n";
    mDataPart += "\r\n";
    mDataPart += data;
    mDataPart += "\r\n";
}

void QPut::upload()
{
    mData = mTextPart + mDataPart;
    mData.append("--" + mBoundary + "--\r\n");

    qDebug("form:\n%s", mData.constData());
    QNetworkRequest request(mUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + mBoundary);
    qDebug("sending data to %s", qPrintable(mUrl.path()));
    QNetworkReply *reply = mNetwork->post(request, mData);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(DoReplyError()));
    connect(reply, SIGNAL(finished()), this, SLOT(DoFinished()));
}

void QPut::post()
{
    QByteArray data(mUrl.encodedQuery());
    QNetworkRequest request(mUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    qDebug("post data %s to %s", data.constData(), mUrl.toString().toLocal8Bit().constData());
    QNetworkReply *reply = mNetwork->post(request, data);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(DoReplyError()));
    connect(reply, SIGNAL(finished()), this, SLOT(DoFinished()));
}

void QPut::setData(const QByteArray &pData)
{
    mData = pData;
}

void QPut::setUrl(const QUrl &pUrl)
{
    mUrl = pUrl;
}

void QPut::DoReplyError()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	QString err = reply->errorString();
	emit fail(err);
	qWarning("Network error: %s", qPrintable(err));
	mSuccess = false;
	//qApp->exit(1);
}

void QPut::abort()
{
	//mNetwork->Accessible
}

void QPut::DoFinished()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	QNetworkReply::NetworkError error = reply->error();
	QByteArray res = reply->readAll();
	mSuccess = (error == QNetworkReply::NoError);
	if (mSuccess) {
		emit ok(res);
	} else {
		emit fail(reply->errorString());
	}

	QNetworkRequest r = reply->request();

	foreach(QByteArray h, r.rawHeaderList())
		qDebug("Head [%s] = %s", h.constData(), r.rawHeader(h).constData());

	qDebug("Network finished. Result: %s", res.trimmed().constData());
	reply->deleteLater();
	//qApp->exit(!mSuccess);
}

void QPut::init()
{
	reset();
	mNetwork = new QNetworkAccessManager(this);
	//Form-based File Upload in HTML. http://www.ietf.org/rfc/rfc1867.txt
	//“boundary”是用来隔开表单中不同部分数据的。“boundary”一般随机产生, 也可以简单的用“-------------”来代替。
	mBoundary = "---------------------------";
	mBoundary += QByteArray::number(QDateTime::currentMSecsSinceEpoch());
	if (mBoundary.size() > 70)
		mBoundary = mBoundary.left(70);
}
}//namespace PhotoKit
