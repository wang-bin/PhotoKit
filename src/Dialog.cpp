/******************************************************************************
	Dialog.cpp: description
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


#include "Dialog.h"
#include "Dialog_p.h"
#include <QtCore/QEventLoop>
#include <QtCore/QTimeLine>
#include <QGraphicsLinearLayout>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItemAnimation>
#include "ItemAnimation.h"
#include "TransformMachine.h"
#include "Button.h"
#include "ezlog.h"
//TODO: event filter for view
namespace PhotoKit {
//TODO: multipage

DialogPrivate::DialogPrivate()
	:okBtn(0),scene(0),animation(new ItemAnimation),width(0)
{
	titleBar = new QGraphicsWidget;
	buttonBar = new QGraphicsWidget;
	buttonBar->setContentsMargins(4, 6, 4, 1);
	central = new QGraphicsWidget;
	titleBar->setMaximumHeight(33);
	buttonBar->setMaximumHeight(33);
	animation->setDuration(618);
	animation->transformMachine()->setRotationAt(0, 90, 0, 0);
	animation->transformMachine()->setRotationAt(1, 0, 0, 0);
	//animation->transformMachine()->setStartTransform(QTransform().rotate(90, Qt::XAxis));
}

QSizeF DialogPrivate::size() const {
	//qDebug("%f %f", central->contentsRect().width(), width);
	qreal w = qMax(central->contentsRect().width(), width); //?contentsRect().width(), titleBar->size() is not the value of resize()?
	qreal h =  buttonBar->contentsRect().height() + central->contentsRect().height() + titleBar->contentsRect().height();
	return QSizeF(w, h);
}

void DialogPrivate::setupUi(Dialog* ui) {
	titleBar->setParent(ui);
	buttonBar->setParent(ui);
	central->setParent(ui);
	titleBar->setPos(11, 2);
	ui->setTransform(QTransform().rotate(90, Qt::XAxis));
	animation->setItem(ui);

	central->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

	okBtn = new Button("<p style='color:white;font-size:16px'>" + QObject::tr("Ok") + "</p>", Button::RoundedRectShape, buttonBar);
	buttonBar->resize(okBtn->boundingRect().size());
	okBtn->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
	okBtn->resize(100, 33);
	buttonBar->resize(okBtn->boundingRect().size());
	okBtn->setColor(Qt::black);

	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	layout->setPreferredHeight(222);
	layout->setSpacing(8);
	layout->addItem(titleBar);
	layout->addItem(central);
	layout->addStretch(8);
	layout->addItem(buttonBar);
	ui->setLayout(layout);
}

DialogPrivate::~DialogPrivate() {
	delete okBtn;
	delete animation;
}

Dialog::Dialog(QGraphicsScene *scene, QGraphicsItem *parent) :
	QGraphicsWidget(parent),d_ptr(new DialogPrivate)
{
	//setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable); //to recieve mouse move event

	setAcceptsHoverEvents(true);
	setZValue(22);
	//setWindowFlags(Qt::Dialog);
	Q_D(Dialog);
	d->setupUi(this);
	connect(d->okBtn, SIGNAL(clicked()), SLOT(accept()));

	d->scene = scene;
	d->scene->addItem(this);
	//grabMouse();

	setFlag(QGraphicsItem::ItemIsPanel);
	setPanelModality(QGraphicsItem::SceneModal);

}

Dialog::Dialog(DialogPrivate &p, QGraphicsScene *scene, QGraphicsItem *parent)
	:QGraphicsWidget(parent),d_ptr(&p)
{
	//setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable); //to recieve mouse move event

	setAcceptsHoverEvents(true);
	setZValue(22);
	//setWindowFlags(Qt::Dialog);
	Q_D(Dialog);
	d->setupUi(this);
	connect(d->okBtn, SIGNAL(clicked()), SLOT(accept()));

	d->scene = scene;
	d->scene->addItem(this);
	//grabMouse();

	setFlag(QGraphicsItem::ItemIsPanel);
	setPanelModality(QGraphicsItem::SceneModal);
/*
	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
	effect->setBlurRadius(3);
	effect->setXOffset(2);
	setGraphicsEffect(effect);*/
}

Dialog::~Dialog()
{
	scene()->removeItem(this);
	if (d_ptr) {
		delete d_ptr;
		d_ptr = 0;
	}
}

QGraphicsWidget* Dialog::titleBar()
{
	Q_D(Dialog);
	return d->titleBar;
}

QGraphicsWidget* Dialog::centralWidget()
{
	Q_D(Dialog);
	return d->central;
}

QGraphicsWidget* Dialog::buttonBar()
{
	Q_D(Dialog);
	return d->buttonBar;
}


void Dialog::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	QLinearGradient g(0, 0, 0, size().height());
	g.setSpread(QLinearGradient::PadSpread);
	g.setColorAt(0, QColor(168, 168, 222, 200));
	g.setColorAt(0.618, QColor(222, 222, 255, 188));
	g.setColorAt(1, QColor(168, 168, 222, 200));
	painter->setPen(QPen(Qt::gray, 1));
	painter->setBrush(g);
	painter->setClipRect(boundingRect());
	painter->drawRoundedRect(boundingRect(), 22, 22, Qt::AbsoluteSize);
}

void Dialog::flipShow()
{
	Q_D(Dialog);
	d->animation->start();
}

void Dialog::exec()
{
	Q_D(Dialog);
	show();
	d->loop.exec();
}

void Dialog::accept()
{
	Q_D(Dialog);
	d->animation->transformMachine()->timeLine()->setDirection(QTimeLine::Backward);
	d->animation->start();

	connect(d->animation, SIGNAL(finished()), this, SLOT(close()));
	connect(d->animation, SIGNAL(finished()), &d->loop, SLOT(quit()));
}

} //namespace PhotoKit
