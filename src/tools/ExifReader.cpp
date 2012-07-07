/******************************************************************************
	ExifReader.cpp: description
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


#include "ExifReader.h"
#include <cstdio>
#include <libexif/exif-data.h>
#include <QtCore/QObject>
namespace PhotoKit {

typedef struct {
	int tag;
	QString name;
} tag_info;


//TODO: not translated?
static const tag_info idf0_tags[] = {
	{EXIF_TAG_MAKE, 			QObject::tr("Manufacturer")}, //QObject::tr()
	{EXIF_TAG_MODEL,			QObject::tr("Model")},
	{EXIF_TAG_SOFTWARE,			QObject::tr("Software")},
	{EXIF_TAG_COPYRIGHT,		QObject::tr("Copyright")},
};

static const tag_info idf_exif_tags[] = {
	{EXIF_TAG_EXIF_VERSION,			QObject::tr("Exif Verion")},
	{EXIF_TAG_FNUMBER,				QObject::tr("F-Number")},
	{EXIF_TAG_EXPOSURE_TIME,		QObject::tr("Exposure time")},
	{EXIF_TAG_EXPOSURE_MODE,		QObject::tr("Exposure mode")},
	{EXIF_TAG_FOCAL_LENGTH,			QObject::tr("Focal length")},
	{EXIF_TAG_FLASH,				QObject::tr("Flash")},
	{EXIF_TAG_ISO_SPEED_RATINGS,	QObject::tr("ISO Speed Ratings")},
	{EXIF_TAG_DATE_TIME_ORIGINAL,	QObject::tr("Date and Time (Original)")},
	{EXIF_TAG_METERING_MODE,		QObject::tr("Metering mode")},
	{EXIF_TAG_WHITE_BALANCE,		QObject::tr("White balance")}
};

static const tag_info idf_gps_tags[] = {
	{EXIF_TAG_GPS_LATITUDE_REF, 	QObject::tr("North or South")},
	{EXIF_TAG_GPS_LATITUDE,			QObject::tr("Latitude")},
	{EXIF_TAG_GPS_LONGITUDE_REF,	QObject::tr("East or West")},
	{EXIF_TAG_GPS_LONGITUDE,		QObject::tr("Longitude")},
	{EXIF_TAG_GPS_ALTITUDE_REF,		QObject::tr("Altitude refrence")},
	{EXIF_TAG_GPS_ALTITUDE,			QObject::tr("Altitude")},
	{EXIF_TAG_GPS_IMG_DIRECTION_REF,QObject::tr("Image Direction Refrence")},
	{EXIF_TAG_GPS_IMG_DIRECTION,	QObject::tr("Image Direction")}
};


void read_exif_entry(ExifEntry *ee, void* info)
{
	char v[1024];
//	strncpy(t, exif_tag_get_title_in_ifd(ee->tag, exif_entry_get_ifd(ee)), sizeof(t));
//	strncpy(t, exif_tag_get_title_in_ifd(ee->tag, *((ExifIfd*)ifd)), sizeof(t));
	//trim t
/*	printf("[%s] (%s): %s\n  %s\n"
			, exif_tag_get_name_in_ifd(ee->tag, *((ExifIfd*)ifd))
			, exif_tag_get_title_in_ifd(ee->tag, *((ExifIfd*)ifd))
			, exif_entry_get_value(ee, v, sizeof(v))
			, exif_tag_get_description_in_ifd(ee->tag, *((ExifIfd*)ifd))
			);*/
	((ExifReader::TagInfo*)info)->insert(exif_tag_get_title_in_ifd(ee->tag, exif_entry_get_ifd(ee))
						, exif_entry_get_value(ee, v, sizeof(v)));
}

void read_exif_content(ExifContent *ec, void *user_data)
{
	ExifIfd ifd = exif_content_get_ifd(ec);
	if (ifd == EXIF_IFD_COUNT)
		fprintf(stderr, "No EXIF data. exif_content_get_ifd error");
	exif_content_foreach_entry(ec, read_exif_entry, user_data);
}

class ExifReaderPrivate
{
public:
	ExifReaderPrivate():data(0){}
	~ExifReaderPrivate(){
		if (data)
			exif_data_unref(data);
		data = 0;
	}

	ExifData* data;
	QString path;
};

ExifReader::ExifReader(const QString& fileName)
	:d(new ExifReaderPrivate)
{
	if (!fileName.isEmpty())
		loadFile(fileName);
}

ExifReader::~ExifReader()
{
	if (d) {
		delete d;
		d = 0;
	}
}

bool ExifReader::hasData() const
{
	return d->data != 0;
}


bool ExifReader::hasIFD(IFD ifd) const
{
	if ((ExifIfd)ifd >= EXIF_IFD_COUNT)
		return false;
	ExifContent *c = d->data->ifd[(ExifIfd)ifd];
	return c->count > 0;
}

bool ExifReader::hasIFD0() const
{
	return hasIFD(IFD_0);
}

bool ExifReader::hasIFD1() const
{
	return hasIFD(IFD_1);
}

bool ExifReader::hasIFDExif() const
{
	return hasIFD(EXIF);
}

bool ExifReader::hasIFDGPS() const
{
	return hasIFD(GPS);
}

bool ExifReader::hasIFDInteroperability() const
{
	return hasIFD(Interoperability);
}

void ExifReader::loadFile(const QString &fileName)
{
	d->path = fileName;
	if (!d->data)
		exif_data_unref(d->data);
	d->data = exif_data_new_from_file(fileName.toLocal8Bit().constData());
	if (!d->data)
		qDebug("No exif data");
}

ExifReader::TagInfo ExifReader::dumpAll() const
{
	TagInfo info;
	exif_data_foreach_content(d->data, read_exif_content, &info);
	return info;
}

ExifReader::TagInfo ExifReader::getIFD0Brief() const
{
	TagInfo info;
	ExifContent *c = d->data->ifd[EXIF_IFD_0];
	if(c->count == 0)
		return info;
	QByteArray v(1024, 0);
	int s = sizeof(idf0_tags)/sizeof(tag_info);
	for(int i = 0; i < s; ++i) {
		v.fill(0);
		ExifEntry *entry = exif_content_get_entry(c, (ExifTag)idf0_tags[i].tag);
		exif_entry_get_value(entry, v.data(), v.size());
		v.trimmed();
		info.insert(idf0_tags[i].name, QString::fromUtf8(v.constData())); //libexif use utf8
		//info.insert(QString::fromUtf8(exif_tag_get_title_in_ifd(entry->tag, exif_entry_get_ifd(entry))), QString::fromUtf8(v.constData()));
	}
	return info;
}

ExifReader::TagInfo ExifReader::getExifBrief() const
{
	TagInfo info;
	ExifContent *c = d->data->ifd[EXIF_IFD_EXIF];
	if(c->count == 0)
		return info;
	QByteArray v(1024, 0);
	int s = sizeof(idf_exif_tags)/sizeof(tag_info);
	for(int i = 0; i < s; ++i) {
		v.fill(0);
		ExifEntry *entry = exif_content_get_entry(c, (ExifTag)idf_exif_tags[i].tag);
		exif_entry_get_value(entry, v.data(), v.size());
		v.trimmed();
		info.insert(idf_exif_tags[i].name,  QString::fromUtf8(v.constData()));
		//info.insert(QString::fromUtf8(exif_tag_get_title_in_ifd(entry->tag, exif_entry_get_ifd(entry))), QString::fromUtf8(v.constData()));
	}
	return info;
}

ExifReader::TagInfo ExifReader::getGpsBrief() const
{
	TagInfo info;
	ExifContent *c = d->data->ifd[EXIF_IFD_GPS];
	if(c->count == 0)
		return info;
	QByteArray v(1024, 0);
	int s = sizeof(idf_gps_tags)/sizeof(tag_info);
	for(int i = 0; i < s; ++i) {
		v.fill(0);
		ExifEntry *entry = exif_content_get_entry(c, (ExifTag)idf_gps_tags[i].tag);
		exif_entry_get_value(entry, v.data(), v.size());
		v.trimmed();
		info.insert(idf_gps_tags[i].name,  QString::fromUtf8(v.constData()));
		//crash
		//info.insert(QString::fromUtf8(exif_tag_get_title_in_ifd(entry->tag, exif_entry_get_ifd(entry))), QString::fromUtf8(v.constData()));
	}
	return info;
}

} //namespace PhotoKit
