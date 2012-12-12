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


#ifndef PHOTOKIT_TEXTEDIT_H
#define PHOTOKIT_TEXTEDIT_H

#include <QGraphicsTextItem>
#include <QLinearGradient>
namespace PhotoKit {

class TextEdit : public QGraphicsTextItem
{
	Q_OBJECT
public:
	enum EchoMode { Normal, NoEcho, Password, PasswordEchoOnEdit };
	explicit TextEdit(QGraphicsItem *parent = 0);
	//virtual ~TextEdit(); //father's non-virtual!

	EchoMode echoMode() const;
    void setEchoMode(EchoMode mode);

    void setSingleLine(bool yes);
    bool isSingleLine() const;

	QString text() const; //the real plain string user inputed. e.g. in Password mode, not "***"

	void resize(qreal width, qreal height);
	void resize(const QSizeF& size);
	QSizeF size() const;
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void submit(); //press return in single line mode
private slots:
    void storeString();//(int position, int charsRemoved, int charsAdded);
    //void storeString(int position, int charsRemoved, int charsAdded);
protected:
    virtual void keyPressEvent(QKeyEvent *event);
	//virtual void keyReleaseEvent(QKeyEvent *event);
    //virtual void inputMethodEvent(QInputMethodEvent *event);
private:
    bool single_line;
	EchoMode mEchoMode;
	qreal mWidth, mHeight;
	QLinearGradient mGradient;
};

} //namespace PhotoKit

#endif // PHOTOKIT_TEXTEDIT_H
