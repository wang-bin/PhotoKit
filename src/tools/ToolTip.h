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


#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QGraphicsWidget>
//TODO: fadein/out
namespace PhotoKit {
class ToolTip : public QGraphicsObject
{
	Q_OBJECT
public:
	//static void showText(const QPointF& pos, const QString text, int msec);

	explicit ToolTip(const QString& text, QGraphicsScene* scene, QGraphicsItem *parent = 0);
	explicit ToolTip(const QImage& image, QGraphicsScene* scene, QGraphicsItem *parent = 0);
	static void showText(const QString& text, QGraphicsScene* scene, int msshow = 8000);
	static void showImage(const QImage& image, QGraphicsScene* scene, int msshow = 8000);
    virtual QRectF boundingRect() const;

    void setText(const QString& text);
	void setImage(const QImage& image);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void done();
private:
	static bool isText;
    static ToolTip* instance;
	bool mTextChanged;
    QGraphicsScene *mScene;
	QString mText;
	QImage mImage;
    qreal mWidth, mHeight;
    qreal mMargin;
    int mTextFlag;
    QFont mFont;
	QGraphicsTextItem *mTextItem;
};

} //namespace PhotoKit
#endif // TOOLTIP_H
