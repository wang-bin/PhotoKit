/******************************************************************************
	Dialog.h: description
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


#ifndef PHOTOKIT_DIALOG_H
#define PHOTOKIT_DIALOG_H

#include <QGraphicsWidget>

//TODO: set effect

namespace PhotoKit {
class DialogPrivate;
class Dialog : public QGraphicsWidget
{
	Q_OBJECT
public:
	explicit Dialog(QGraphicsScene *scene, QGraphicsItem *parent = 0);
	virtual ~Dialog();
	//void setText(const QString& text);

	QGraphicsWidget* titleBar();
	QGraphicsWidget* centralWidget();
	QGraphicsWidget* buttonBar();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
	
public slots:
	void exec();
	void accept();

protected:
	Dialog(DialogPrivate& d, QGraphicsScene *scene, QGraphicsItem *parent = 0);

protected:
	Q_DECLARE_PRIVATE(Dialog)
	DialogPrivate* d_ptr;
};

} //namespace PhotoKit

#endif // PHOTOKIT_DIALOG_H
