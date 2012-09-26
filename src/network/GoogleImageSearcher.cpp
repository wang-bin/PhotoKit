/******************************************************************************
	GoogleImageSearcher: a wrapper for google image search api
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


#include "GoogleImageSearcher.h"
#include <QtCore/QRegExp>
#include <QtCore/QUrl>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtCore/QUrlQuery>
#endif //QT_VERSION_CHECK(5, 0, 0)
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include "ImageProvider_p.h"

namespace PhotoKit {

static const QByteArray GooglerImageSeachServer("https://ajax.googleapis.com/ajax/services/search/images");
static const int PageMax(8);

static QMap<int, QByteArray> GIParam;

class GoogleImageSearcherPrivate : public ImageProviderPrivate
{
public:
	GoogleImageSearcherPrivate():network(new QNetworkAccessManager),size(GoogleImageSearcher::MediumSize)
	{
		if (GIParam.isEmpty()) {
			GIParam.insert(GoogleImageSearcher::IconSize, "icon");
			GIParam.insert(GoogleImageSearcher::MediumSize, "small|medium|large|xlarge");
			GIParam.insert(GoogleImageSearcher::LargeSize, "xxlarge");
			GIParam.insert(GoogleImageSearcher::HugeSize, "huge");
		}
	}
	~GoogleImageSearcherPrivate() {
		if (network) {
			delete network;
			network = 0;
		}
	}

	QNetworkAccessManager *network;
	GoogleImageSearcher::SizeFilter size;

};

GoogleImageSearcher::GoogleImageSearcher(QObject *parent) :
	ImageProvider(*new GoogleImageSearcherPrivate, parent)
{
	Q_D(GoogleImageSearcher);
	connect(d->network, SIGNAL(finished(QNetworkReply*)), SLOT(parseReply(QNetworkReply*)));
}

void GoogleImageSearcher::setSizeFilter(SizeFilter size)
{
	Q_D(GoogleImageSearcher);
	d->size = size;
}

bool GoogleImageSearcher::canFetchMore() const
{
	Q_D(const GoogleImageSearcher);
	return d->page < PageMax;
}

void GoogleImageSearcher::fetchMore()
{
	if (!canFetchMore())
		return;

	Q_D(GoogleImageSearcher);
	QUrl url(GooglerImageSeachServer);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	QUrlQuery urlqurey;
	urlqurey.addQueryItem("v", "1.0");
	urlqurey.addQueryItem("q", QUrl::toPercentEncoding(QString(d->nameFilter).replace(' ', '+'))); //+
	urlqurey.addQueryItem("imgsz", GIParam[d->size]);
	urlqurey.addQueryItem("start", QString::number(d->page*8));
	urlqurey.addQueryItem("rsz", "8"); //rsz: 1-8, the number of results to return per page
	url.setQuery(urlqurey);
#else
	url.addQueryItem("v", "1.0");
	url.addQueryItem("q", QUrl::toPercentEncoding(QString(d->nameFilter).replace(' ', '+'))); //+
	url.addQueryItem("imgsz", GIParam[d->size]);
	url.addQueryItem("start", QString::number(d->page*8));
	url.addQueryItem("rsz", "8"); //rsz: 1-8, the number of results to return per page
#endif //QT_VERSION_CHECK(5.0.0)

	QNetworkRequest request(url);
	request.setRawHeader("Referer", "https://github.com/wang-bin/PhotoKit");
	d->network->get(request);

	d->page++;
}

void GoogleImageSearcher::reset()
{
	Q_D(GoogleImageSearcher);
	d->page = 0;
}

void GoogleImageSearcher::parseJson(const QByteArray &json)
{
	//qDebug("%s", json.constData());
	static QRegExp w("\"width\":\\s*\"(\\d+)\",");
	static QRegExp h("\"height\":\\s*\"(\\d+)\",");
	static QRegExp tw("\"tbWidth\":\\s*\"(\\d+)\",");
	static QRegExp th("\"tbHeight\":\\s*\"(\\d+)\",");
	//[^\"] not .*, because greedy is default. or setMinimal(true). What if contains '"'?
	static QRegExp url("\"url\":\\s*\"([^\"]+)\",");
	static QRegExp turl("\"tbUrl\":\\s*\"([^\"]+)\",");
	//QList<ImageBaseInfo> imgs;
	QList<QByteArray> results = json.split('}');
	foreach (QByteArray res, results) {
		if (!res.contains("GsearchResultClass"))
			break;
		QString r(res.append(','));
		ImageBaseInfo img;
		if (w.indexIn(r, 0) != -1)
			img.width = w.cap(1).toInt();
		else
			continue;
		if (h.indexIn(r, 0) != -1)
			img.height = h.cap(1).toInt();
		else
			continue;
		if (tw.indexIn(r, 0) != -1)
			img.thumbWidth = tw.cap(1).toInt();
		else
			continue;
		if (th.indexIn(r, 0) != -1)
			img.thumbHeight = th.cap(1).toInt();
		else
			continue;
		if (url.indexIn(r, 0) != -1)
			img.path = url.cap(1);
		 else
			continue;
		if (turl.indexIn(r, 0) != -1)
			img.thumbPath = turl.cap(1).replace("\\u003d", "="); //\u003d
		else
			continue;
		//qDebug("w: %d h: %d, tw: %d, th: %d", img.width, img.height, img.thumbWidth, img.thumbHeight);
		//qDebug("url: %s\nturl: %s",  qPrintable(img.path), qPrintable(img.thumbPath));
		emit imageReady(img);
	}
}

void GoogleImageSearcher::parseReply(QNetworkReply *reply)
{
	QNetworkReply::NetworkError error = reply->error();
	if (error != QNetworkReply::NoError) {
		qWarning("Network error: %s", qPrintable(reply->errorString()));
	}
	QByteArray data = reply->readAll();
	reply->deleteLater();

	parseJson(data);
}

} //namespace PhotoKit
