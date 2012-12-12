/******************************************************************************
	TextEdit: QGraphicsTextItem based editor.
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


#include "TextEdit.h"
#include <QtGui/QPainter>
#include <QLinearGradient>
#include <QTextDocument>
#include <QInputMethodEvent>
#include <QTextCursor>
#include "ezlog.h"

namespace PhotoKit {

TextEdit::TextEdit(QGraphicsItem *parent) :
    QGraphicsTextItem(parent),single_line(false),mEchoMode(Normal)
{
	setTextInteractionFlags(Qt::TextEditorInteraction);
	document()->setDocumentMargin(8);
	//document()->setTextWidth();
	QFont f;
	f.setPixelSize(22);
	setFont(f);

    //connect(document(), SIGNAL(contentsChange(int,int,int)), SLOT(storeString(int,int,int)));
    connect(document(), SIGNAL(contentsChanged()), SLOT(storeString()));
}
/*
TextEdit::~TextEdit()
{
	if (mGradient) {
		delete mGradient;
		mGradient = 0;
	}
}*/

TextEdit::EchoMode TextEdit::echoMode() const
{
	return mEchoMode;
}

void TextEdit::setEchoMode(EchoMode mode)
{
	mEchoMode = mode;
}

void TextEdit::setSingleLine(bool yes)
{
    single_line = yes;
}

bool TextEdit::isSingleLine() const
{
    return single_line;
}

QString TextEdit::text() const
{
	if (mEchoMode == Password) {
		QString stored =  data(0).toString();
		//set text not by setPlainText()
		if (!stored.isEmpty())
			return stored;
		return toPlainText();
	}
	return toPlainText();
}

void TextEdit::resize(const QSizeF &size)
{
    resize(size.width(), size.height());
}

void TextEdit::resize(qreal width, qreal height)
{
	mWidth = width;
	mHeight = height;
	setTextWidth(mWidth);
	mGradient.setStart(0, 0);
	mGradient.setFinalStop(mWidth, mHeight);
	mGradient.setColorAt(0, QColor(180, 212, 234, 148));
	mGradient.setColorAt(0.618, QColor(220, 230, 240, 168));
	mGradient.setColorAt(1, QColor(211, 222, 238, 180));
}

QSizeF TextEdit::size() const
{
	return QSizeF(mWidth, mHeight);
}

QRectF TextEdit::boundingRect() const
{
	return QRectF(0, 0, mWidth, mHeight);
}

void TextEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen(Qt::gray);
	pen.setWidthF(2.71828);
	painter->setPen(pen);
	painter->setBrush(mGradient);
	painter->drawRoundedRect(boundingRect(), 8, 8);
	QGraphicsTextItem::paint(painter, option, widget);
}

//NOT PRESS. TODO: postion
void TextEdit::storeString()//(int position, int charsRemoved, int charsAdded) //keyReleaseEvent(QKeyEvent *event)
{
	//qDebug("%d %d %d", position, charsRemoved, charsAdded);
	if (mEchoMode == Password) {
		int len = toPlainText().size();
		QString stored = data(0).toString();
		if (stored.size() > len)
			stored = stored.left(len);
		else if (stored.size() == len)
			return;
		else
			stored = stored + toPlainText().right(len - stored.size());
		setData(0, stored);
		setPlainText(QString(len, '*')); //cusor will set to begin
		QTextCursor cursor(textCursor());
		cursor.setPosition(len);
		setTextCursor(cursor); //FIXME: can't move cursor!
	}
    //qDebug("string %s", qPrintable(text()));
	//QGraphicsTextItem::keyReleaseEvent(event);
}

void TextEdit::keyPressEvent(QKeyEvent *event)
{
    if (single_line && event->key() == Qt::Key_Return) {
        emit submit();
        event->accept();
    } else {
        QGraphicsTextItem::keyPressEvent(event);
    }
}


/*
void TextEdit::storeString(int position, int charsRemoved, int charsAdded)
{qDebug("changetext");
    if (mEchoMode == Password) {
        QString str = toPlainText();
        QString stored = data(0).toString();
        QString ct = str.mid(position, charsAdded);
        if (ct == stored.mid(position, charsAdded))
            return;
        stored.replace(position, charsRemoved, str.mid(position, charsAdded));
        setData(0, stored);
        setPlainText(QString(stored.length(), '*'));
        qDebug("%s", qPrintable(stored));
    }
}
*/
/*
void TextEdit::inputMethodEvent(QInputMethodEvent *event)
{
	//qDebug("input method: %s", qPrintable(event->commitString()));
	QGraphicsTextItem::inputMethodEvent(event);
}
*/
} //namespace PhotoKit
