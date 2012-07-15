/******************************************************************************
	ToolTip: tooltip to show image or text
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


#include "ToolTip.h"
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QLinearGradient>
#include <QtCore/qmath.h>
#include <QDateTime>
#include <QApplication>
#include <QDesktopWidget>
#include <QTextDocument>
#include "ezlog.h"

namespace PhotoKit {
//TODO: add glow
static int count = 0;
ToolTip* ToolTip::instance = 0;
bool ToolTip::isText = true;
ToolTip::ToolTip(const QString& text, QGraphicsScene* scene, QGraphicsItem *parent) :
	QGraphicsObject(parent),mTextChanged(false),mScene(scene)
{
	mMargin = 16;
	mTextItem = new QGraphicsTextItem(this);
	mTextItem->setDefaultTextColor(Qt::white);
	mTextItem->document()->setDocumentMargin(mMargin);
	setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setZValue(100);
    mScene->addItem(this);
    setText(text);
}

ToolTip::ToolTip(const QImage& image, QGraphicsScene* scene, QGraphicsItem *parent) :
	QGraphicsObject(parent),mTextChanged(false),mScene(scene)
{
    mTextItem = new QGraphicsTextItem(this);
    mTextItem->setDefaultTextColor(Qt::white);
    mTextItem->document()->setDocumentMargin(mMargin);
	setFlag(QGraphicsItem::ItemIgnoresTransformations);
	setZValue(100);
	mScene->addItem(this);
	setImage(image);
}

QRectF ToolTip::boundingRect() const
{
    return QRectF(0, 0, mWidth + 2*mMargin, mHeight + 2*mMargin);
}

void ToolTip::setText(const QString &text)
{
	isText = true;
	mTextItem->setHtml(text);
	mWidth = mTextItem->document()->size().width();
	mHeight = mTextItem->document()->size().height();
}

void ToolTip::setImage(const QImage &image)
{
	isText = false;
	mImage = image;
	mWidth = image.width();
	mHeight = image.height();
}

void ToolTip::showText(const QString &text, QGraphicsScene* scene, int msshow)
{
    if (!instance) {
        instance = new ToolTip(text, scene); //more then one scene?
    } else {
		instance->hide(); //count--?
        instance->setText(text);
    }
    int w = qApp->desktop()->width(), h = qApp->desktop()->height();
	int x = (w - instance->boundingRect().width())*0.5;
	int y = (h - instance->boundingRect().height())*0.5;
	instance->setPos(x, y);
    instance->show();
    QTimer::singleShot(msshow, instance, SLOT(done()));
	count++;
}

void ToolTip::showImage(const QImage &image, QGraphicsScene *scene, int msshow)
{
	if (!instance) {
		instance = new ToolTip(image, scene); //more then one scene?
	} else {
		instance->hide();
		instance->setImage(image);
	}

	int w = qApp->desktop()->width(), h = qApp->desktop()->height();
	instance->setPos((w - image.width())/2, (h - image.height())/2); //TODO: random
	instance->show();
	QTimer::singleShot(msshow, instance, SLOT(done()));
	count++;
}

void ToolTip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!isText) {
		mTextItem->hide();
		painter->drawImage(QPointF(), mImage);
		return;
	}
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(44, 44, 44, 234));
	painter->setClipRect(boundingRect());
	painter->drawRoundedRect(boundingRect(), 12, 12, Qt::AbsoluteSize);
	mTextItem->show();
}

void ToolTip::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setVisible(false);
    event->accept();
}

void ToolTip::done()
{
    if (--count == 0)
        setVisible(false);
}

} //namespace PhotoKit
