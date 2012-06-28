/******************************************************************************
	ToolTip.h: description
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


#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QGraphicsObject>
namespace PhotoKit {
class ToolTip : public QGraphicsObject
{
	Q_OBJECT
public:
	//static void showText(const QPointF& pos, const QString text, int msec);

	explicit ToolTip(QGraphicsItem *parent = 0);
	virtual QRectF boundingRect() const;

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void computeRect();

public slots:
	void showText(const QString& text);

private:
	bool mTextChanged;
	QString mText;
	QRectF mRect;
};

} //namespace PhotoKit
#endif // TOOLTIP_H
