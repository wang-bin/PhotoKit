/******************************************************************************
	ToolTip.cpp: description
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

namespace PhotoKit {
//TODO: add glow
static int count = 0;
ToolTip* ToolTip::instance = 0;
ToolTip::ToolTip(const QString& text, QGraphicsScene* scene, QGraphicsItem *parent) :
	QGraphicsObject(parent),mTextChanged(false),mScene(scene)
{
	mMargin = 16;
	mTextItem = new QGraphicsTextItem(this);
	mTextItem->document()->setTextWidth(qApp->desktop()->width()/2);
	mTextItem->setDefaultTextColor(Qt::white);
	mTextItem->document()->setDocumentMargin(mMargin);
	setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setZValue(100);
    mScene->addItem(this);
    setText(text);
}

QRectF ToolTip::boundingRect() const
{
    return QRectF(0, 0, mWidth + 2*mMargin, mHeight + 2*mMargin);
}

void ToolTip::setText(const QString &text)
{/*
    mText = text;
    //mFont.setFamily(mFontName);
    mFont.setPixelSize(16);
    mFont.setWeight(QFont::Black);
    QFontMetrics fm(mFont);
    mTextFlag = Qt::AlignLeft | Qt::TextExpandTabs;
    QSize textsize = fm.size(mTextFlag, text);

    mWidth = textsize.width();
    mHeight = textsize.height();
*/
	mTextItem->setPlainText(text);
	mWidth = mTextItem->document()->size().width();
	mHeight = mTextItem->document()->size().height();
}

void ToolTip::showText(const QString &text, QGraphicsScene* scene, int msshow)
{
    if (!instance) {
        instance = new ToolTip(text, scene); //more then one scene?
    } else {
        instance->hide();
        instance->setText(text);
    }

    qsrand(QDateTime::currentMSecsSinceEpoch());
    QRect r = instance->boundingRect().toRect();
    static int margin = 55;
    int w = qApp->desktop()->width(), h = qApp->desktop()->height();
    int x = qrand()%qMax(w - 2*margin - r.width(), 1) + margin;
    int y = qrand()%qMax(h - 2*margin - r.height(), 1) + margin;
    instance->setPos(x, y); //TODO: random
    instance->show();
    QTimer::singleShot(msshow, instance, SLOT(done()));
    count++;
}

void ToolTip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
    QLinearGradient g(0, 0, 0, mHeight + 2*mMargin);
    g.setSpread(QLinearGradient::PadSpread);
	g.setColorAt(0, QColor(168, 168, 77, 168));
    g.setColorAt(0.618, QColor(255, 255, 22, 111));
	g.setColorAt(1, QColor(168, 168, 66, 168));
    painter->setPen(QPen(Qt::gray, 1));
    painter->setBrush(g);
	painter->setClipRect(boundingRect());
    painter->drawRoundedRect(boundingRect(), 10, 80, Qt::RelativeSize);
/*
	if (mText.isEmpty())
		return;
    painter->setPen(QColor(Qt::white).dark(2));
    painter->setFont(mFont);
	painter->drawText(QRectF(mMargin, mMargin, mWidth, mHeight), mTextFlag, mText);*/
}

void ToolTip::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    count--;
    setVisible(false);
    event->accept();
}

void ToolTip::done()
{
    if (--count == 0)
        setVisible(false);
}

} //namespace PhotoKit
