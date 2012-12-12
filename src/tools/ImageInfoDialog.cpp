/******************************************************************************
	ImageInfoDialog: a dialog to display image information
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


#include "ImageInfoDialog.h"
#include <QTextDocument>
#include <QGraphicsTextItem>
#include <QApplication>
#include <QDesktopWidget>
#include "Dialog_p.h"
#include "Button.h"
namespace PhotoKit {

class ImageInfoDialogPrivate : public DialogPrivate
{
public:
	friend class ImageInfoDialog;
	ImageInfoDialogPrivate() {
		text = new QGraphicsTextItem;
		text->setDefaultTextColor(Qt::blue);
		text->document()->setDocumentMargin(22);
		//text->document()->setPageSize(QSizeF(qApp->desktop()->width()/2, qApp->desktop()->height() - 22));

		base = new Button("<p style='color:white;font-size:16px'>" + QObject::tr("Base info") + "</p>", Button::RectShape);
		exif = new Button("<p style='color:white;font-size:16px'>EXIF</p>", Button::RectShape);
		gps = new Button("<p style='color:white;font-size:16px'>GPS</p>", Button::RectShape);
		base->setColor(Qt::black);
		exif->setColor(Qt::black);
		gps->setColor(Qt::black);
		base->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
		exif->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
		gps->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
		base->resize(100, 33);
		exif->resize(88, 33);
		gps->resize(88, 33);
		base->setData(0, 0); //signal slot
		exif->setData(0, 1);
		gps->setData(0, 2);

		okBtn = new Button("<p style='color:white;font-size:16px'>" + QObject::tr("Ok") + "</p>", Button::RoundedRectShape, buttonBar);
		buttonBar->resize(okBtn->boundingRect().size());
		okBtn->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
		okBtn->resize(100, 33);
		buttonBar->resize(okBtn->boundingRect().size());
		okBtn->setColor(Qt::black);
	}
	~ImageInfoDialogPrivate() {
		delete okBtn;
	}

	void setupUi(ImageInfoDialog* ui) {
		QObject::connect(okBtn, SIGNAL(clicked()), ui, SLOT(accept()));
		text->setParentItem(central);
		base->setParentItem(titleBar);
		exif->setParentItem(titleBar);
		gps->setParentItem(titleBar);
		base->setPos(0, 0);
		exif->setPos(base->width(), 0);
		gps->setPos(base->width() + exif->width(), 0);
		width = base->width() + exif->width() + gps->width() + 22;
		titleBar->resize(width, base->height());

		QObject::connect(base, SIGNAL(clicked()), ui, SLOT(showInfo()));
		QObject::connect(exif, SIGNAL(clicked()), ui, SLOT(showInfo()));
		QObject::connect(gps, SIGNAL(clicked()), ui, SLOT(showInfo()));
	}

	Button *okBtn;
	QGraphicsTextItem* text;
	Button *base, *exif, *gps;
	QString info[3];
};

ImageInfoDialog::ImageInfoDialog(QGraphicsScene *scene, QGraphicsItem *parent) :
	Dialog(*new ImageInfoDialogPrivate, scene, parent)
{
	Q_D(ImageInfoDialog);
	d->ImageInfoDialogPrivate::setupUi(this);
}

ImageInfoDialog::~ImageInfoDialog()
{/*
	if (d_ptr) {
		delete d_ptr;
		d_ptr = 0;
	}*/
}

void ImageInfoDialog::setBaseImageInfo(const QString &text)
{
	Q_D(ImageInfoDialog);
	d->info[0] = text;
}

void ImageInfoDialog::setExifInfo(const QString &text)
{
	Q_D(ImageInfoDialog);
	d->info[1] = text;
}

void ImageInfoDialog::setGPSInfo(const QString &text)
{
	Q_D(ImageInfoDialog);
	d->info[2] = text;
}

void ImageInfoDialog::showInfo()
{
	Q_D(ImageInfoDialog);
	Button *b = qobject_cast<Button*>(sender());
	d->text->document()->setTextWidth(size().width());
	d->text->setHtml(d->info[b->data(0).toInt()]);
	centralWidget()->resize(d->text->document()->size());
	prepareGeometryChange();
	resize(d->size());
}

void ImageInfoDialog::showBaseInfo()
{
	Q_D(ImageInfoDialog);
	d->text->document()->setTextWidth(size().width());
	d->text->setHtml(d->info[0]);
	centralWidget()->resize(d->text->document()->size());
	prepareGeometryChange();
	resize(d->size());

	//flipShow();
}

} //namespace PhotoKit
