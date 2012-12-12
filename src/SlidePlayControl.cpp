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


#include "SlidePlayControl.h"

#include <qdatetime.h>
#include <qdir.h>
#include <qapplication.h>
#include <qpainter.h>
#if QT_VERSION>0x040000
//#include <QtGui/QStyle>
#include <QtCore/QTimerEvent>
#include <QDesktopWidget>
#endif
#include "ezlog.h"
#include "nexteffect.h"
#include "SlideDisplay.h"
#include "nexteffectfactory.h"
#include "tools/Tools.h"
#include "Config.h"

namespace PhotoKit {

SlidePlayControl::SlidePlayControl(QObject *parent) :
	QObject(parent),direction(Forward)
{
	random = true;
	one = false;
	running = false;
	effect = 0;
	view = 0;
	//NextEffectFactory::init();
}

SlidePlayControl::~SlidePlayControl()
{
	if (effect) {
		delete effect;
		effect = 0;
	}
	/*if (view) {
		delete view;
		view = 0;
	}*/
}

void SlidePlayControl::start()
{
	if (!view) {
		return;
		/*
		view = new SlideDisplay;
		//view->setAttribute(Qt::WA_PaintOutsidePaintEvent);
		view->setAttribute(Qt::WA_NoSystemBackground);
		//view->resize(qApp->desktop()->size());
		//view->resize(effect->size());
		view->show();
		view->move(qApp->desktop()->availableGeometry(view).center()-view->rect().center());
		//view->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, view->size(),qApp->desktop()->availableGeometry()));
		*/
	}
	ezlog_debug();
	if (random)
		effect = NextEffectFactory::getRandom();
	ezlog_debug();

	startOne();
	tid_slide = startTimer(6666);
	running = true;
}

void SlidePlayControl::stop()
{
	killTimer(tid_slide);
	killTimer(tid_effect);
	running = false;
	view->setImagePath(current_path);
	view->update();
}

void SlidePlayControl::setDisplay(SlideDisplay *display)
{
	view = display;
}

void SlidePlayControl::setDirectory(const QString &dir)
{
	paths = QDir(dir).entryList(Tools::imageNameFilters()).replaceInStrings(QRegExp("^"), dir+"/");
}

void SlidePlayControl::setImages(const QStringList &images)
{
	paths = images;
}

void SlidePlayControl::addImage(const QString &path)
{
	paths.append(path);
}

void SlidePlayControl::setPlayOne(bool yes)
{
	one = yes;
}

void SlidePlayControl::setDirection(Direction d)
{
	direction = d;
}

void SlidePlayControl::setEffectType(EffectId t)
{
	if (effect!=0 && effect->hasType(t)) {
		effect->setType(t);
		return;
	}
	if (effect!=0)
		delete effect;

	effect = NextEffectFactory::create(t);//effectFromType(t);
	//if (view)
		//effect->setSize(view->size());
	//effect->setFrames(10);
}


void SlidePlayControl::timerEvent(QTimerEvent *e)
{
	if (e->timerId()==tid_slide) {
		killTimer(tid_effect);
		if (random)
			effect = NextEffectFactory::getRandom();
		startOne();
	} else if (e->timerId()==tid_effect) {
		if (!effect->prepareNextFrame()) {
			killTimer(tid_effect);
			ezlog_debug("%s: Effect [%d %s] end!", qPrintable(QTime::currentTime().toString("hh:mm:ss.zzz")), effect->type(), NextEffectFactory::name(effect->type()).c_str());
			if (one) {
				killTimer(tid_slide);
				running = false;
				view->setImagePath(current_path);
				view->update();
			}
		}
#if QT_VERSION<0x040000
		//bitBlt()
#else
		//QPainter p(view);
		//p.drawPixmap(view->rect(), *effect->currentFrame());
		view->update();
#endif
	}
}

void SlidePlayControl::startOne()
{/*
	int idx = paths.indexOf(next_path);
	bool first = (idx==-1);
	if (idx==paths.size()-1 || first)
		idx = 0;

	current_path = paths.at(idx);
	if (idx==paths.size()-1)
		next_path = paths.at(0);
	else
		next_path = paths.at(idx+1);
*/
	ezlog_debug("total slide images: %d", paths.size());
	int idx = paths.indexOf(current_path);
	if (direction == Forward) {
		bool last = (idx == (paths.size() -1));
		if (last) {
			next_path = paths.first();
		} else {
			next_path = paths.at(idx + 1);
		}
	} else {
		bool first = (idx == 0);
		if (first) {
			next_path = paths.last();
		} else {
			next_path = paths.at(idx - 1);
		}
	}
#if 0
	QImage p0(view->size(), QImage::Format_RGB32), p1(view->size(), QImage::Format_RGB32);
	/*if (first) {
		next_path = current_path = paths.at(0);
		p0.fill(Qt::black);
	} else {
		p0.load(current_path);
	}*/
	p0.load(current_path);
	p1.load(next_path);

	//TODO: view's size
	static QSize s = qApp->desktop()->size();
	if (p0.size() != s) {
		if (Config::keepAspectRatio) {
			p0 = p0.scaled(s, Qt::KeepAspectRatio);
			if (p0.size() != s) {
				QImage backGoundImage0(s, QImage::Format_ARGB32_Premultiplied);
				QPainter painter(&backGoundImage0);
				painter.fillRect(backGoundImage0.rect(), Qt::black);
				painter.drawImage((s.width()-p0.width())/2, (s.height()-p0.height())/2, p0);
				p0 = backGoundImage0;
			}
		} else {
			p0 = p0.scaled(s, Qt::IgnoreAspectRatio);
		}
	}
	if (p1.size() != s) {
		if (Config::keepAspectRatio) {
			p1 = p1.scaled(s, Qt::KeepAspectRatio);
			if (p1.size() != s) {
				QImage backGoundImage1(s, QImage::Format_ARGB32_Premultiplied);
				QPainter painter(&backGoundImage1);
				painter.fillRect(backGoundImage1.rect(), Qt::black);
				painter.drawImage((s.width()-p1.width())/2, (s.height()-p1.height())/2, p1);
				p1 = backGoundImage1;
			}
		} else {
			p1 = p1.scaled(s, Qt::IgnoreAspectRatio);
		}
	}
	ezlog_debug("playing '%s' format=%d %dx%d", qPrintable(next_path), p1.format(), p1.width(), p1.height());
	effect->setInitialPixmaps(p0, p1);
#endif
	effect->setImages(current_path, next_path);
	//effect->setSpeed(0.618);
	effect->setSize(view->size());
	effect->setBackgroundColor(Config::backgroundColor);
	effect->prepare();
	view->setEffect(effect);
	//view->resize(effect->size());
	//ezlog_debug("size=%dx%d", effect->size().width(), effect->size().height());

	tid_effect = startTimer(33);
	ezlog_debug("%s: Effect [%d %s] start!", qPrintable(QTime::currentTime().toString("hh:mm:ss.zzz")), effect->type(), NextEffectFactory::name(effect->type()).c_str());
	setCurrentImage(next_path);
}

} //namespace PhotoKit
