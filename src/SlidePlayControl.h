/******************************************************************************
	SlidePlayControl: slide effect controller
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


#ifndef PHOTOKIT_SLIDEPLAYCONTROL_H
#define PHOTOKIT_SLIDEPLAYCONTROL_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include "nexteffect.h"

namespace PhotoKit {
class SlideDisplay;
class SlidePlayControl : public QObject
{
	Q_OBJECT
public:
	enum Direction { Forward, Backward};
	explicit SlidePlayControl(QObject *parent = 0);
	~SlidePlayControl();

	bool isRunning() const {return running;}
	void setDisplay(SlideDisplay* display); //not own the display
	void setDirectory(const QString& dir); //add remove
	void setImages(const QStringList& images);
	void addImage(const QString& path);
	void setPlayOne(bool yes); //TODO: remove
	void setDirection(Direction d);
	//void setImages(const QStringList& paths); //add remove
	QString currentImage() const {return current_path;}
	void setCurrentImage(const QString& path) {current_path = path;}
	void setNextImage(const QString& path) {next_path = path;}

	void setRandomEffect(bool r) {random=r;}
	void setEffectType(EffectId t);

public slots:
	void start();
	void stop();
	void startOne();

protected:
	virtual void timerEvent(QTimerEvent *);


private:
	bool random;
	bool one;
	bool running;
	Direction direction;
	SlideDisplay *view;
	NextEffect *effect;
	int tid_effect, tid_slide;

	QString current_path, next_path;
	QStringList paths;
};

} //namespace PhotoKit

#endif // PHOTOKIT_SLIDEPLAYCONTROL_H
