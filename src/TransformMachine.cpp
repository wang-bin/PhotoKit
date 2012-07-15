/******************************************************************************
	TransformMachine: transform and other value sequence generator
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


#include "TransformMachine.h"

#include <QtCore/qtimeline.h>
#include <QtCore/qpoint.h>
#include <QtCore/qpointer.h>
#include <QtCore/qpair.h>
#include <QtGui/qtransform.h>

namespace PhotoKit {


class TransformMachinePrivate
{
public:
	inline TransformMachinePrivate()
		: timeLine(0), step(0)
	{ }

public:
	QPointer<QTimeLine> timeLine;

    QPointF startPos;
	QTransform startTransform;

	qreal step;

	struct Pair {
		Pair(qreal a, qreal b) : step(a), value(b) {}
		bool operator <(const Pair &other) const
		{ return step < other.step; }
		bool operator==(const Pair &other) const
		{ return step == other.step; }
		qreal step;
		qreal value;
	};
	QList<Pair> xPosition;
	QList<Pair> yPosition;
	QList<Pair> xRotation;
	QList<Pair> yRotation;
	QList<Pair> zRotation;
	QList<Pair> verticalScale;
	QList<Pair> horizontalScale;
	QList<Pair> verticalShear;
	QList<Pair> horizontalShear;
	QList<Pair> xTranslation;
	QList<Pair> yTranslation;
	QList<Pair> zValue;

	qreal linearValueForStep(qreal step, QList<Pair> *source, qreal defaultValue = 0);
	void insertUniquePair(qreal step, qreal value, QList<Pair> *binList, const char* method);
};

qreal TransformMachinePrivate::linearValueForStep(qreal step, QList<Pair> *source, qreal defaultValue)
{
	if (source->isEmpty())
		return defaultValue;
	step = qMin<qreal>(qMax<qreal>(step, 0), 1);

	if (step == 1)
		return source->last().value;

	qreal stepBefore = 0;
	qreal stepAfter = 1;
	qreal valueBefore = source->first().step == 0 ? source->first().value : defaultValue;
	qreal valueAfter = source->last().value;

	// Find the closest step and value before the given step.
	for (int i = 0; i < source->size() && step >= source->at(i).step; ++i) {
		stepBefore = source->at(i).step;
		valueBefore = source->at(i).value;
	}

	// Find the closest step and value after the given step.
	for (int j = source->size() - 1; j >= 0 && step < source->at(j).step; --j) {
		stepAfter = source->at(j).step;
		valueAfter = source->at(j).value;
	}

	// Do a simple linear interpolation.
	return valueBefore + (valueAfter - valueBefore) * ((step - stepBefore) / (stepAfter - stepBefore));
}

void TransformMachinePrivate::insertUniquePair(qreal step, qreal value, QList<Pair> *binList, const char* method)
{
	if (step < 0.0 || step > 1.0) {
		qWarning("TransformMachine::%s: invalid step = %f", method, step);
		return;
	}

	Pair pair(step, value);

	QList<Pair>::iterator result = qBinaryFind(binList->begin(), binList->end(), pair);
	if (result != binList->end())
		result->value = value;
	else {
		*binList << pair;
		qSort(binList->begin(), binList->end());
	}
}

/*!
  Constructs an animation object with the given \a parent.
*/
TransformMachine::TransformMachine(QObject *parent)
	: QObject(parent), d(new TransformMachinePrivate)
{
}

/*!
  Destroys the animation object.
*/
TransformMachine::~TransformMachine()
{
	delete d;
}
/*!
  Returns the timeline object used to control the rate at which the animation
  occurs.

  \sa setTimeLine()
*/
QTimeLine *TransformMachine::timeLine() const
{
	return d->timeLine;
}

/*!
  Sets the timeline object used to control the rate of animation to the \a timeLine
  specified.

  \sa timeLine()
*/
void TransformMachine::setTimeLine(QTimeLine *timeLine)
{
	if (d->timeLine == timeLine)
		return;
	if (d->timeLine)
		delete d->timeLine;
	if (!timeLine)
		return;
	d->timeLine = timeLine;
	connect(timeLine, SIGNAL(valueChanged(qreal)), this, SLOT(setStep(qreal)));
}

void TransformMachine::setStartPos(const QPointF &pos)
{
    d->startPos = pos;
}

QPointF TransformMachine::posAt(qreal step) const
{
    if (step < 0.0 || step > 1.0)
        qWarning("QGraphicsItemAnimation::posAt: invalid step = %f", step);

    return QPointF(d->linearValueForStep(step, &d->xPosition, d->startPos.x()),
                   d->linearValueForStep(step, &d->yPosition, d->startPos.y()));
}

/*!
  \fn void QGraphicsItemAnimation::setPosAt(qreal step, const QPointF &point)

  Sets the position of the item at the given \a step value to the \a point specified.

  \sa posAt()
*/
void TransformMachine::setPosAt(qreal step, const QPointF &pos)
{
    d->insertUniquePair(step, pos.x(), &d->xPosition, "setPosAt");
    d->insertUniquePair(step, pos.y(), &d->yPosition, "setPosAt");
}

/*!
  Returns all explicitly inserted positions.

  \sa posAt(), setPosAt()
*/
QList<QPair<qreal, QPointF> > TransformMachine::posList() const
{
    QList<QPair<qreal, QPointF> > list;
    for (int i = 0; i < d->xPosition.size(); ++i)
        list << QPair<qreal, QPointF>(d->xPosition.at(i).step, QPointF(d->xPosition.at(i).value, d->yPosition.at(i).value));

    return list;
}

/*!
  Returns the transform used to transform the item at the specified \a step value.
*/
QTransform TransformMachine::transformAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::transformAt: invalid step = %f", step);

	QTransform transform;
	if (!d->xRotation.isEmpty())
		transform.rotate(xRotationAt(step), Qt::XAxis);
	if (!d->yRotation.isEmpty())
		transform.rotate(yRotationAt(step), Qt::YAxis);
	if (!d->zRotation.isEmpty())
		transform.rotate(zRotationAt(step), Qt::ZAxis);
	if (!d->verticalScale.isEmpty())
		transform.scale(horizontalScaleAt(step), verticalScaleAt(step));
	if (!d->verticalShear.isEmpty())
		transform.shear(horizontalShearAt(step), verticalShearAt(step));
	if (!d->xTranslation.isEmpty())
		transform.translate(xTranslationAt(step), yTranslationAt(step));
	return transform;
}

void TransformMachine::setStartTransform(const QTransform &m)
{
	d->startTransform = m;
}

qreal TransformMachine::xRotationAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::xRotationAt: invalid step = %f", step);

	return d->linearValueForStep(step, &d->xRotation);
}
void TransformMachine::setXRotationAt(qreal step, qreal angle)
{
	d->insertUniquePair(step, angle, &d->xRotation, "setXRotationAt");
}
QList<QPair<qreal, qreal> > TransformMachine::xRotationList() const
{
	QList<QPair<qreal, qreal> > list;
	for (int i = 0; i < d->xRotation.size(); ++i)
		list << QPair<qreal, qreal>(d->xRotation.at(i).step, d->xRotation.at(i).value);

	return list;
}qreal TransformMachine::yRotationAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::yRotationAt: invalid step = %f", step);

	return d->linearValueForStep(step, &d->yRotation);
}
void TransformMachine::setYRotationAt(qreal step, qreal angle)
{
	d->insertUniquePair(step, angle, &d->yRotation, "setYRotationAt");
}
QList<QPair<qreal, qreal> > TransformMachine::yRotationList() const
{
	QList<QPair<qreal, qreal> > list;
	for (int i = 0; i < d->yRotation.size(); ++i)
		list << QPair<qreal, qreal>(d->yRotation.at(i).step, d->yRotation.at(i).value);

	return list;
}
qreal TransformMachine::zRotationAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::zRotationAt: invalid step = %f", step);

	return d->linearValueForStep(step, &d->zRotation);
}
void TransformMachine::setZRotationAt(qreal step, qreal angle)
{
	d->insertUniquePair(step, angle, &d->zRotation, "setZRotationAt");
}
QList<QPair<qreal, qreal> > TransformMachine::zRotationList() const
{
	QList<QPair<qreal, qreal> > list;
	for (int i = 0; i < d->zRotation.size(); ++i)
		list << QPair<qreal, qreal>(d->zRotation.at(i).step, d->zRotation.at(i).value);

	return list;
}

void TransformMachine::setRotationAt(qreal step, qreal xrot, qreal yrot, qreal zrot)
{
	d->insertUniquePair(step, xrot, &d->xRotation, "setXRotationAt");
	d->insertUniquePair(step, yrot, &d->yRotation, "setYRotationAt");
	d->insertUniquePair(step, zrot, &d->zRotation, "setZRotationAt");
}

/*!
  Returns the horizontal translation of the item at the specified \a step value.

  \sa setTranslationAt()
*/
qreal TransformMachine::xTranslationAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::xTranslationAt: invalid step = %f", step);

	return d->linearValueForStep(step, &d->xTranslation);
}
qreal TransformMachine::yTranslationAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::yTranslationAt: invalid step = %f", step);

	return d->linearValueForStep(step, &d->yTranslation);
}
void TransformMachine::setTranslationAt(qreal step, qreal dx, qreal dy)
{
	d->insertUniquePair(step, dx, &d->xTranslation, "setTranslationAt");
	d->insertUniquePair(step, dy, &d->yTranslation, "setTranslationAt");
}
QList<QPair<qreal, QPointF> > TransformMachine::translationList() const
{
	QList<QPair<qreal, QPointF> > list;
	for (int i = 0; i < d->xTranslation.size(); ++i)
		list << QPair<qreal, QPointF>(d->xTranslation.at(i).step, QPointF(d->xTranslation.at(i).value, d->yTranslation.at(i).value));

	return list;
}

/*!
  Returns the vertical scale for the item at the specified \a step value.

  \sa setScaleAt()
*/
qreal TransformMachine::verticalScaleAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::verticalScaleAt: invalid step = %f", step);

	return d->linearValueForStep(step, &d->verticalScale, 1);
}
qreal TransformMachine::horizontalScaleAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::horizontalScaleAt: invalid step = %f", step);

	return d->linearValueForStep(step, &d->horizontalScale, 1);
}
void TransformMachine::setScaleAt(qreal step, qreal sx, qreal sy)
{
	d->insertUniquePair(step, sx, &d->horizontalScale, "setScaleAt");
	d->insertUniquePair(step, sy, &d->verticalScale, "setScaleAt");
}
QList<QPair<qreal, QPointF> > TransformMachine::scaleList() const
{
	QList<QPair<qreal, QPointF> > list;
	for (int i = 0; i < d->horizontalScale.size(); ++i)
		list << QPair<qreal, QPointF>(d->horizontalScale.at(i).step, QPointF(d->horizontalScale.at(i).value, d->verticalScale.at(i).value));

	return list;
}

/*!
  Returns the vertical shear for the item at the specified \a step value.

  \sa setShearAt()
*/
qreal TransformMachine::verticalShearAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::verticalShearAt: invalid step = %f", step);

	return d->linearValueForStep(step, &d->verticalShear, 0);
}
qreal TransformMachine::horizontalShearAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::horizontalShearAt: invalid step = %f", step);

	return d->linearValueForStep(step, &d->horizontalShear, 0);
}
void TransformMachine::setShearAt(qreal step, qreal sh, qreal sv)
{
	d->insertUniquePair(step, sh, &d->horizontalShear, "setShearAt");
	d->insertUniquePair(step, sv, &d->verticalShear, "setShearAt");
}
QList<QPair<qreal, QPointF> > TransformMachine::shearList() const
{
	QList<QPair<qreal, QPointF> > list;
	for (int i = 0; i < d->horizontalShear.size(); ++i)
		list << QPair<qreal, QPointF>(d->horizontalShear.at(i).step, QPointF(d->horizontalShear.at(i).value, d->verticalShear.at(i).value));

	return list;
}

/*!
  Returns the zValue at the specified \a step value.

  \sa setZValuerAt()
*/
qreal TransformMachine::zValueAt(qreal step) const
{
	if (step < 0.0 || step > 1.0)
		qWarning("TransformMachine::zValueAt: invalid step = %f", step);

	return d->linearValueForStep(step, &d->zValue, 0);
}

void TransformMachine::setZValueAt(qreal step, qreal z)
{
	d->insertUniquePair(step, z, &d->zValue, "setZValueAt");
}
QList<QPair<qreal, qreal> > TransformMachine::zValueList() const
{
	QList<QPair<qreal, qreal> > list;
	for (int i = 0; i < d->zValue.size(); ++i)
		list << QPair<qreal, qreal>(d->zValue.at(i).step, d->zValue.at(i).value);

	return list;
}
/*!
  Clears the scheduled transformations used for the animation, but
  retains the item and timeline.
*/
void TransformMachine::clear()
{
	d->xPosition.clear();
	d->yPosition.clear();
	d->xRotation.clear();
	d->zRotation.clear();
	d->verticalScale.clear();
	d->horizontalScale.clear();
	d->verticalShear.clear();
	d->horizontalShear.clear();
	d->xTranslation.clear();
	d->yTranslation.clear();
	d->zValue.clear();
}

/*!
  \fn void TransformMachine::setStep(qreal step)

  Sets the current \a step value for the animation, causing the
  transformations scheduled at this step to be performed.
*/
void TransformMachine::setStep(qreal x)
{
	if (x < 0.0 || x > 1.0) {
		qWarning("TransformMachine::setStep: invalid step = %f", x);
		return;
	}

	beforeAnimationStep(x);

	d->step = x;
	if (!d->xRotation.isEmpty() || !d->yRotation.isEmpty() || !d->zRotation.isEmpty()
		|| !d->verticalScale.isEmpty()
		|| !d->horizontalScale.isEmpty()
		|| !d->verticalShear.isEmpty()
		|| !d->horizontalShear.isEmpty()
		|| !d->xTranslation.isEmpty()
		|| !d->yTranslation.isEmpty()) {
		emit transformChanged(d->startTransform * transformAt(x));
	}
	if (!d->zValue.isEmpty()) {
		emit zValueChanged(zValueAt(x));
	}
    if (!d->xPosition.isEmpty() || !d->yPosition.isEmpty())
        emit posChanged(posAt(x));
	afterAnimationStep(x);
}

/*!
	Resets the item to its starting position and transformation.

	\obsolete

	You can call setStep(0) instead.
*/
void TransformMachine::reset()
{
    d->startPos = QPointF();
	d->startTransform = QTransform();
}

/*!
  \fn void TransformMachine::beforeAnimationStep(qreal step)

  This method is meant to be overridden by subclassed that needs to
  execute additional code before a new step takes place. The
  animation \a step is provided for use in cases where the action
  depends on its value.
*/
void TransformMachine::beforeAnimationStep(qreal step)
{
	Q_UNUSED(step);
}

/*!
  \fn void TransformMachine::afterAnimationStep(qreal step)

  This method is meant to be overridden in subclasses that need to
  execute additional code after a new step has taken place. The
  animation \a step is provided for use in cases where the action
  depends on its value.
*/
void TransformMachine::afterAnimationStep(qreal step)
{
	Q_UNUSED(step);
}

} //namespace PhotoKit
