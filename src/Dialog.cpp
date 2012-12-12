/******************************************************************************
	Dialog: widget with dialog options
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
#include <QGraphicsTransform>
#include "ItemAnimation.h"
#include "TransformMachine.h"
#include "Button.h"
#include "ezlog.h"
//TODO: event filter for view

#define NO_ROT3D 0
namespace PhotoKit {
//TODO: multipage

DialogPrivate::DialogPrivate()
	:scene(0),animation(new ItemAnimation),width(0),result(Dialog::Rejected)
{
	titleBar = new QGraphicsWidget;
	buttonBar = new QGraphicsWidget;
	buttonBar->setContentsMargins(4, 6, 4, 1);
	central = new QGraphicsWidget;
	titleBar->setMaximumHeight(33);
	buttonBar->setMaximumHeight(33);
	animation->setDuration(618);
#if NO_ROT3D
	animation->transformMachine()->setRotationAt(0, 90, 0, 0);
	animation->transformMachine()->setRotationAt(1, 0, 0, 0);
	animation->transformMachine()->setStartTransform(QTransform().rotate(90, Qt::XAxis));
#else
	timeline = new QTimeLine(618);
	timeline->setEasingCurve(QEasingCurve::OutQuint);
#endif
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
#if NO_ROT3D
	ui->setTransform(QTransform().rotate(90, Qt::XAxis));
	animation->setItem(ui);
#else
	ui->connect(timeline, SIGNAL(valueChanged(qreal)), ui, SLOT(rotate3D(qreal)));
#endif
	central->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);


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
	delete animation;
	delete timeline;
}

Dialog::Dialog(QGraphicsScene *scene, QGraphicsItem *parent) :
	QGraphicsWidget(parent),d_ptr(new DialogPrivate)
{
	//setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable); //to recieve mouse move event

	setAcceptHoverEvents(true);
	setZValue(22);
	Q_D(Dialog);
	d->setupUi(this);
	ezlog_debug();
#if !NO_ROT3D
	QGraphicsRotation *rot = new QGraphicsRotation;
	rot->setOrigin(QVector3D(this->size().width(), 0, 0));
	rot->setAxis(Qt::YAxis);
	rot->setAngle(90);
#if 0
	QMatrix4x4 m;
	rot.applyTo(&m);
	setTransform(m.toAffine());
#else
	QList<QGraphicsTransform*> transforms;
	transforms.append(rot);
	setTransformations(transforms);
#endif
#endif //NO_ROT3D
	d->scene = scene;
	d->scene->addItem(this);

	//setPanelModality(QGraphicsItem::SceneModal); //will block input. i need text editing

}

Dialog::Dialog(DialogPrivate &p, QGraphicsScene *scene, QGraphicsItem *parent)
	:QGraphicsWidget(parent),d_ptr(&p)
{
	//setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable); //to recieve mouse move event

	setAcceptHoverEvents(true);
	setZValue(22);
	Q_D(Dialog);
	d->setupUi(this);
#if !NO_ROT3D
	QGraphicsRotation *rot = new QGraphicsRotation;
	rot->setOrigin(QVector3D(this->size().width(), 0, 0));
	rot->setAxis(Qt::YAxis);
	rot->setAngle(90);
#if 0
	QMatrix4x4 m;
	rot.applyTo(&m);
	setTransform(m.toAffine());
#else
	QList<QGraphicsTransform*> transforms;
	transforms.append(rot);
	setTransformations(transforms);
#endif
#endif //NO_ROT3D
	d->scene = scene;
	d->scene->addItem(this);

	//setPanelModality(QGraphicsItem::SceneModal); //will block input. i need text editing
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
	QLinearGradient g(0, 0, 0, size().height()); //TODO: member var
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
#if NO_ROT3D
	d->animation->start();
#else
	d->timeline->start();
#endif
}

int Dialog::exec()
{
	Q_D(Dialog);
	flipShow(); //?
	d->loop.exec();
	emit finished(d->result);
	return d->result;
}

void Dialog::accept()
{
	Q_D(Dialog);
	d->result = Dialog::Accepted;
#if NO_ROT3D
	d->animation->transformMachine()->timeLine()->setDirection(QTimeLine::Backward);
	d->animation->start();
	connect(d->animation, SIGNAL(finished()), this, SLOT(close()));
	connect(d->animation, SIGNAL(finished()), &d->loop, SLOT(quit()));
#else
	d->timeline->setDirection(QTimeLine::Backward);
	d->timeline->start();
	connect(d->timeline, SIGNAL(finished()), this, SLOT(close()));
	connect(d->timeline, SIGNAL(finished()), &d->loop, SLOT(quit()));
#endif //NO_ROT3D
	emit accepted();
	emit finished(d->result);
}

void Dialog::reject()
{
	Q_D(Dialog);
	d->result = Dialog::Rejected;
#if NO_ROT3D
	d->animation->transformMachine()->timeLine()->setDirection(QTimeLine::Backward);
	d->animation->start();
	connect(d->animation, SIGNAL(finished()), this, SLOT(close()));
	connect(d->animation, SIGNAL(finished()), &d->loop, SLOT(quit()));
#else
	d->timeline->setDirection(QTimeLine::Backward);
	d->timeline->start();
	connect(d->timeline, SIGNAL(finished()), this, SLOT(close()));
	connect(d->timeline, SIGNAL(finished()), &d->loop, SLOT(quit()));
#endif //NO_ROT3D
	emit rejected();
	emit finished(d->result);

}

void Dialog::rotate3D(qreal step)
{
#if !NO_ROT3D
	qreal angle = 90.0*(1.0 - step);
	QGraphicsRotation *rot = new QGraphicsRotation;
	rot->setOrigin(QVector3D(this->size().width(), 0, 0));
	rot->setAxis(Qt::YAxis);
	rot->setAngle(angle);
#if 0
	QMatrix4x4 m;
	rot.applyTo(&m);
	setTransform(m.toAffine());
#else
	QList<QGraphicsTransform*> transforms;
	transforms.append(rot);
	setTransformations(transforms);
#endif
#endif //NO_ROT3D
}

} //namespace PhotoKit
