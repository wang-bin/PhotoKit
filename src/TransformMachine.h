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

#ifndef PHOTOKIT_TRANSFORMMACHINE_H
#define PHOTOKIT_TRANSFORMMACHINE_H
/*!
    TODO: custome variables. valueAt(qreal step, const QString& key);
*/

#include <QtCore/qobject.h>

class QTransform;
class QPointF;
class QTimeLine;

template <class T1, class T2> struct QPair;

namespace PhotoKit {

class TransformMachinePrivate;
class TransformMachine : public QObject
{
	Q_OBJECT
public:
	TransformMachine(QObject *parent = 0);
	virtual ~TransformMachine();

	QTimeLine *timeLine() const;
	void setTimeLine(QTimeLine *timeLine);
	//TODO: value animation
/*
    qreal valueAt(qreal step, const QString& key) const;
    QList<QPair<qreal, qreal> > valueList(const QString& key) const;
    void setValueAt(qreal step, qreal value, const QString& key);
*/
    void setStartPos(const QPointF& pos);
	QPointF posAt(qreal step) const;
	QList<QPair<qreal, QPointF> > posList() const;
	void setPosAt(qreal step, const QPointF &pos);

	QTransform transformAt(qreal step) const;
	void setStartTransform(const QTransform& m);

	qreal xRotationAt(qreal step) const;
	QList<QPair<qreal, qreal> > xRotationList() const;
	void setXRotationAt(qreal step, qreal angle);

	qreal yRotationAt(qreal step) const;
	QList<QPair<qreal, qreal> > yRotationList() const;
	void setYRotationAt(qreal step, qreal angle);

	qreal zRotationAt(qreal step) const;
	QList<QPair<qreal, qreal> > zRotationList() const;
	void setZRotationAt(qreal step, qreal angle);

	void setRotationAt(qreal step, qreal xrot, qreal yrot, qreal zrot);

	qreal xTranslationAt(qreal step) const;
	qreal yTranslationAt(qreal step) const;
	QList<QPair<qreal, QPointF> > translationList() const;
	void setTranslationAt(qreal step, qreal dx, qreal dy);

	qreal verticalScaleAt(qreal step) const;
	qreal horizontalScaleAt(qreal step) const;
	QList<QPair<qreal, QPointF> > scaleList() const;
	void setScaleAt(qreal step, qreal sx, qreal sy);

	qreal verticalShearAt(qreal step) const;
	qreal horizontalShearAt(qreal step) const;
	QList<QPair<qreal, QPointF> > shearList() const;
	void setShearAt(qreal step, qreal sh, qreal sv);

	qreal zValueAt(qreal step) const;
	QList<QPair<qreal, qreal> > zValueList() const;
	void setZValueAt(qreal step, qreal value);

	void clear();

Q_SIGNALS:
	void transformChanged(const QTransform& transform);
	void zValueChanged(qreal value);
	void posChanged(const QPointF& pos);
	//void valueChanged(const QString& key, qreal value);

public Q_SLOTS:
	void setStep(qreal x);
	void reset();

protected:
	virtual void beforeAnimationStep(qreal step);
	virtual void afterAnimationStep(qreal step);

private:
	Q_DISABLE_COPY(TransformMachine)
	TransformMachinePrivate *d;
};

} //namespace PhotoKit

#endif // PHOTOKIT_TRANSFORMMACHINE_H
