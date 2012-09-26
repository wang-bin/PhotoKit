/******************************************************************************
	Button: graphicsitem based button with animation support
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


#include "Button.h"

#include <QCursor>
#include <QtGui/QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneResizeEvent>
#include <QIcon>
#include <QTextDocument>
#include "ezlog.h"
#include "BaseItem.h"
#include "OutlineGlowItem.h"
/*hover/press to highlight*/

namespace PhotoKit {

class ButtonPrivate
{
public:
	ButtonPrivate():checkable(false), checked(false), autoRepeat(false), autoExclusive(false),
		down(false), pressed(false){}
	uint checkable :1;
	uint checked :1;
	uint autoRepeat :1;
	uint autoExclusive :1;
	uint down :1;
	uint pressed :1;
};

class ButtonBackground : public BaseItem
{
public:
    Button::ButtonType type;
	Button::ButtonShape mShape;
    bool highlighted;
    bool pressed;
    QSize logicalSize;
	Button *button;

	ButtonBackground(Button::ButtonType type, Button::ButtonShape shape, bool highlighted, bool pressed, QSize logicalSize, Button *parent):
		BaseItem(parent),button(parent)
    {
		//keep parent transform
		//setFlag(QGraphicsItem::ItemIgnoresTransformations);
        setFlag(QGraphicsItem::ItemClipsChildrenToShape);
        mShape = shape;
        this->type = type;
        this->highlighted = highlighted;
        this->pressed = pressed;
        this->logicalSize = logicalSize;
		useSharedImage(QString(__FILE__) + "color" + button->color().rgb() + "type" + static_cast<int>(type)
					   + "shape" + static_cast<int>(shape) + "hi" + highlighted + pressed + + "w" + logicalSize.width() + +"h" + logicalSize.height());
    }

protected:
#if 0
	virtual QRectF boundingRect() const {return button->boundingRect();}
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
		painter->setRenderHint(QPainter::SmoothPixmapTransform);
		painter->setRenderHint(QPainter::Antialiasing);
		painter->setPen(Qt::NoPen);

		QRectF scaledRect = boundingRect();
		QColor c = button->color();
		QLinearGradient outlinebrush(0, 0, 0, scaledRect.height());
		QLinearGradient brush(0, 0, 0, scaledRect.height());
		brush.setSpread(QLinearGradient::PadSpread);
		QColor highlight(245, 245, 245, 168);
		QColor shadow(0, 0, 0, 123);
		QColor sunken(c.red() - 33, c.green() -33, c.blue() -40, 66);//(220, 220, 220, 99);
		QColor normal1(c.red(), c.green(), c.blue(), 123);  //(255, 255, 245, 123);
		QColor normal2(c.red() - 3, c.green() -5, c.blue() -10, 66);//(255, 255, 235, 66);
/*
		if (this->mShape == Button::RectShape){
			normal1 = QColor(200, 170, 160, 121);
			normal2 = QColor(100, 40, 0, 121);//  (50, 10, 0, 50);
		}
*/
		if (pressed) {
			outlinebrush.setColorAt(0.0f, shadow);
			outlinebrush.setColorAt(1.0f, highlight);
			brush.setColorAt(0.0f, sunken);
			painter->setPen(Qt::NoPen);
		} else {
			outlinebrush.setColorAt(1.0f, shadow);
			outlinebrush.setColorAt(0.0f, highlight);
			brush.setColorAt(0.0f, normal1);
			if (!this->highlighted)
				brush.setColorAt(1.0f, normal2);
			painter->setPen(QPen(outlinebrush, 1));
		}
		painter->setBrush(brush);

		if (button->buttonShape() == Button::RectShape)
			painter->drawRect(0, 0, scaledRect.width(), scaledRect.height());
		else
			painter->drawRoundedRect(0, 0, scaledRect.width(), scaledRect.height(), 10, 90, Qt::RelativeSize);

		if (button->type() == Button::Text || button->type() == Button::Icon)
			return;

		if (button->buttonType() == Button::ArrowUp || button->buttonType() == Button::ArrowDown) {
			float xOff = scaledRect.width() / 2;
			float yOff = scaledRect.height() / 2;
			float sizey = 1.5;//scaledRect.height() - 10;// * matrix.m22();
			float sizex = 3;// * sizey;// * matrix.m11();
			if (button->type() == Button::ArrowUp)
				sizey *= -1;
			QPainterPath path;
			path.moveTo(xOff, yOff + (5 * sizey));
			path.lineTo(xOff - (4 * sizex), yOff - (3 * sizey));
			path.lineTo(xOff + (4 * sizex), yOff - (3 * sizey));
			path.lineTo(xOff, yOff + (5 * sizey));
			painter->drawPath(path);
		}
	}

#else
    QImage *createImage(const QMatrix &matrix) const
    {
        QRect scaledRect;
        scaledRect = matrix.mapRect(QRect(0, 0, logicalSize.width(), logicalSize.height()));
        //qDebug("type=%d w=%d h=%d %d %d", type, scaledRect.width(), scaledRect.height(), highlighted, pressed);
        QImage *image = new QImage(scaledRect.width(), scaledRect.height(), QImage::Format_ARGB32_Premultiplied);
        image->fill(QColor(0, 0, 0, 0).rgba());
        QPainter painter(image);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);


		QColor c = button->color().lighter();

        QLinearGradient outlinebrush(0, 0, 0, scaledRect.height());
        QLinearGradient brush(0, 0, 0, scaledRect.height());
        brush.setSpread(QLinearGradient::PadSpread);
		QColor highlight(245, 245, 245, 168);
		QColor shadow(0, 0, 0, 123);
        QColor sunken(qMax(c.red() - 33, c.red()), qMax(c.green() -33, c.green()), qMax(c.blue() -40, c.blue()), 66);//(220, 220, 220, 99);
		QColor normal1(c.red(), c.green(), c.blue(), 123);  //(255, 255, 245, 123);
        QColor normal2(qMax(c.red() - 3, c.red()), qMax(c.green() -5, c.green()), qMax(c.blue() -10, c.blue()), 66);//(255, 255, 235, 66);
/*
        if (this->mShape == Button::RectShape){
			normal1 = QColor(200, 170, 160, 121);
			normal2 = QColor(100, 40, 0, 121);//  (50, 10, 0, 50);
        }
*/
        if (pressed) {
            outlinebrush.setColorAt(0.0f, shadow);
            outlinebrush.setColorAt(1.0f, highlight);
            brush.setColorAt(0.0f, sunken);
            painter.setPen(Qt::NoPen);
        } else {
            outlinebrush.setColorAt(1.0f, shadow);
            outlinebrush.setColorAt(0.0f, highlight);
            brush.setColorAt(0.0f, normal1);
            if (!this->highlighted)
                brush.setColorAt(1.0f, normal2);
            painter.setPen(QPen(outlinebrush, 1));
        }
        painter.setBrush(brush);

		if (mShape == Button::RectShape)
            painter.drawRect(0, 0, scaledRect.width(), scaledRect.height());
        else
            painter.drawRoundedRect(0, 0, scaledRect.width(), scaledRect.height(), 10, 90, Qt::RelativeSize);

		if (type == Button::Text || type == Button::Icon)
            return image;

		if (type == Button::ArrowUp || type == Button::ArrowDown) {

            float xOff = scaledRect.width() / 2;
            float yOff = scaledRect.height() / 2;
            float sizex = 3.0f * matrix.m11();
            float sizey = 1.5f * matrix.m22();
            if (this->type == Button::ArrowUp)
                sizey *= -1;
            QPainterPath path;
            path.moveTo(xOff, yOff + (5 * sizey));
            path.lineTo(xOff - (4 * sizex), yOff - (3 * sizey));
            path.lineTo(xOff + (4 * sizex), yOff - (3 * sizey));
            path.lineTo(xOff, yOff + (5 * sizey));
            painter.drawPath(path);
        }
        return image;
    }
#endif
};


void Button::init()
{
	d_ptr = new ButtonPrivate;
    setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setFlag(QGraphicsItem::ItemIgnoresTransformations); //do not draw text/image out the shape
    mBgOn = 0;
    mBgOff = 0;
    mBgHighlight = 0;
    mBgDisabled = 0;
	mGlow = 0;
    mTextItem = 0;
    mIconItem = 0;
	mColor = QColor(88, 88, 88);

	setAcceptHoverEvents(true);
    //setCursor(Qt::PointingHandCursor);
	resize(QSizeF(180, 33));

}

Button::Button(ButtonType type, ButtonShape shape, QGraphicsItem * parent, Qt::WindowFlags) :
	QObject(0),BaseItem(parent),prepared(false),mType(type),mShape(shape),mState(OFF)
{
    init();
}

Button::Button(const QString& text, ButtonShape shape, QGraphicsItem * parent, Qt::WindowFlags) :
	QObject(0),BaseItem(parent),prepared(false),mType(Text),mShape(shape),mState(OFF)
{
    init();
    setText(text);
}

Button::Button(const QPixmap& icon, ButtonShape shape, QGraphicsItem * parent, Qt::WindowFlags) :
	QObject(0),BaseItem(parent),prepared(false),mType(Icon),mShape(shape),mState(OFF)
{
    init();
    setIcon(icon);
}

Button::~Button()
{
	if (d_ptr) {
		delete d_ptr;
		d_ptr = 0;
	}
}

Button::ButtonType Button::buttonType() const
{
	return mType;
}

void Button::setButtonType(ButtonType type)
{
	mType = type;
	prepairBackgrounds();
}

void Button::setCheckable(bool checkable)
{
	Q_D(Button);
	if (d->checkable == checkable)
		return;

	d->checkable = checkable;
	d->checked = false;
}


bool Button::isCheckable() const
{
	Q_D(const Button);
	return d->checkable;
}

void Button::setChecked(bool checked)
{
	Q_D(Button);
	if (!d->checkable || d->checked == checked) {
		//if (!d->blockRefresh)
		//	checkStateSet();
		return;
	}
/*
	if (!checked && d->queryCheckedButton() == this) {
		// the checked button of an exclusive or autoexclusive group cannot be  unchecked
#ifndef QT_NO_BUTTONGROUP
		if (d->group ? d->group->d_func()->exclusive : d->autoExclusive)
			return;
		if (d->group)
			d->group->d_func()->detectCheckedButton();
#else
		if (d->autoExclusive)
			return;
#endif
	}
*/
/*	QPointer<Button> guard(this);

	d->checked = checked;
	if (!d->blockRefresh)
		checkStateSet();
	d->refresh();

	if (guard && checked)
		d->notifyChecked();
	if (guard)*/
	emit toggled(checked);
	setState(checked ? ON : OFF);
}

bool Button::isChecked() const
{
	Q_D(const Button);
	return d->checked;
}


void Button::setDown(bool down)
{
	Q_D(Button);
	if (d->down == down)
		return;
	d->down = down;
	//d->refresh();
	//if (d->autoRepeat && d->down)
	//	d->repeatTimer.start(d->autoRepeatDelay, this);
	//else
	//	d->repeatTimer.stop();
	if (mState == DISABLED)
		return;
	if (down) {
		if (mState == HIGHLIGHT || mState == OFF)
			setState(ON);
	} else {
		setState(OFF);
	}
}

bool Button::isDown() const
{
	Q_D(const Button);
	return d->down;
}

void Button::resize(const QSizeF &size)
{
	//if (mShape == RoundedRectShape)
        logicalSize = QSize(size.width(), size.height());
	//else
	//    logicalSize = QSize(int((size.width() / 2.0f) - 5), int(size.height() * 1.5f));

	prepairBackgrounds();
	setText(mText);
    //setIcon(mIcon);
}

void Button::resize(qreal width, qreal height)
{
    resize(QSizeF(width, height));
}

qreal Button::width() const
{
	return logicalSize.width();
}

qreal Button::height() const
{
	return logicalSize.height();
}

void Button::setText(const QString &text)
{
    if (text.isEmpty())
        return;
    if (mTextItem) {
		if (mText != text)
            mTextItem->setHtml(mText);
        QSizeF s = mTextItem->document()->size();
        qreal x = qMax<qreal>((logicalSize.width() - s.width())/2, 2);
        qreal y = qMax<qreal>((logicalSize.height() - s.height())/2, 2);
        mTextItem->setPos(x, y);
        return;
    }
    mText = text;
    mTextItem = new QGraphicsTextItem(this);
	mTextItem->setDefaultTextColor(Qt::white);
    mTextItem->setHtml(mText);
    QSizeF s = mTextItem->document()->size();
    qreal x = qMax<qreal>((logicalSize.width() - s.width())/2, 2);
    qreal y = qMax<qreal>((logicalSize.height() - s.height())/2, 2);
    mTextItem->setPos(x, y);
    mTextItem->setZValue(zValue() + 2);

}

QString Button::text() const
{
	if (mText.isEmpty())
		return "";
	return mTextItem->document()->toPlainText(); //NOT HTML
}

void Button::setIcon(const QIcon &icon)
{
    mIcon = icon.pixmap(logicalSize);
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(mIcon, this); //TODO
    pixmapItem->setZValue(zValue() + 2);
}

QPixmap Button::icon() const
{
    return mIcon;
}

void Button::setColor(const QColor &color)
{
	mColor = color;
	prepairBackgrounds();
}

QColor Button::color() const
{
    return mColor;
}

QRectF Button::boundingRect() const
{
    return QRectF(0, 0, this->logicalSize.width(), this->logicalSize.height());
}

void Button::setState(State state)
{
    mState = state;
    mBgOn->setRecursiveVisible(mState == ON);
    mBgOff->setRecursiveVisible(mState == OFF);
    mBgHighlight->setRecursiveVisible(mState == HIGHLIGHT);
    mBgDisabled->setRecursiveVisible(mState == DISABLED);
}

void Button::prepairBackgrounds()
{
    if (mBgOn) {
        delete mBgOn;
		mBgOn = 0;
	}
	if (mBgOff) {
        delete mBgOff;
		mBgOff = 0;
	}
	if (mBgHighlight) {
        delete mBgHighlight;
		mBgHighlight = 0;
	}
	if (mBgDisabled) {
        delete mBgDisabled;
		mBgDisabled = 0;
	}
	if (mGlow) {
		delete mGlow;
		mGlow = 0;
	}
	if (!mBgOn)
		mBgOn = new ButtonBackground(mType, mShape, true, true, logicalSize, this);
	if (!mBgOff)
		mBgOff = new ButtonBackground(mType, mShape, false, false, logicalSize, this);
	if (!mBgHighlight)
		mBgHighlight = new ButtonBackground(mType, mShape, true, false, logicalSize, this);
	if (!mBgDisabled)
		mBgDisabled = new ButtonBackground(mType, mShape, true, true, logicalSize, this);
/*
	mBgOn->type = mType;
	mBgOn->mShape = mShape;
	mBgOn->logicalSize = logicalSize;
	mBgOff->type = mType;
	mBgOff->mShape = mShape;
	mBgOff->logicalSize = logicalSize;
	mBgHighlight->type = mType;
	mBgHighlight->mShape = mShape;
	mBgHighlight->logicalSize = logicalSize;
	mBgDisabled->type = mType;
	mBgDisabled->mShape = mShape;
	mBgDisabled->logicalSize = logicalSize;
	mBgOn->update();
	mBgOff->update();
	mBgHighlight->update();
	mBgDisabled->update();*/
	setState(OFF);
/*
    mGlow = new OutlineGlowItem(this);
    //mGlow->setZValue(zValue() + 1);
    QSizeF s = boundingRect().size();
    mGlow->setSize(QSize((int)s.width(), (int)s.height()));
    mGlow->setShape(mBgOn->shape());
    mGlow->setGlowWidth(8);
	mGlow->setColor(QColor(Qt::green).lighter(134));*/
}

void Button::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	logicalSize = event->newSize().toSize();
	prepairBackgrounds();
	setText(mText);
	//setIcon
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() != Qt::LeftButton) {
		event->ignore();
		return;
	}

    if (mState == DISABLED)
        return;
	//setDown(true);
	if (mState == HIGHLIGHT || mState == OFF)
		setState(ON);
   // mGlow->show();
    //update();
    event->accept();
	emit pressed();
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (mState == ON)
        setState(OFF);
   // mGlow->hide();
    event->accept();
	emit clicked();
}

void Button::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (mState == DISABLED)
        return;
    if (mState == OFF)
        setState(HIGHLIGHT);
    event->accept();
}

void Button::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    event->accept();
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (mState == DISABLED)
        return;
    setState(OFF);
    event->accept();
}

} //namespace PhotoKit
