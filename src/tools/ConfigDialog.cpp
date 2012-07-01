/******************************************************************************
	ConfigDialog.cpp: description
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


#include "ConfigDialog.h"

#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QLayout>
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QScrollArea>
#include "Config.h"

namespace PhotoKit {

class ConfigDialogPrivate {
public:
	void setupUi(ConfigDialog* dialog) {
		dialog->setWindowTitle(QObject::tr("Setup"));
		gl = new QCheckBox("OpenGL " + Config::glVersion);
		showTips = new QCheckBox(QObject::tr("Show tips"));
		marginH = new QSpinBox;
		marginV = new QSpinBox;
		thumbWidth = new QSpinBox;
		thumbHeigh = new QSpinBox;
		thumbSpace = new QSpinBox;
		thumbRows = new QSpinBox;
		clearCache = new QPushButton(QObject::tr("Clear thumbnail cache"));
		//backgroundColor = new QLabel;
		//back = new QPushButton(QObject::tr("Background"));
		//glowColor = new QLabel;
		//glow = new QPushButton(QObject::tr("Glow"));

		ok = new QPushButton(QObject::tr("Ok"));
		cancel = new QPushButton(QObject::tr("Cancel"));

		QSize s(qApp->desktop()->size());
		marginH->setMinimum(0);
		marginH->setMaximum(s.width());
		marginV->setMinimum(0);
		marginV->setMaximum(s.height());
		thumbWidth->setMinimum(0);
		thumbWidth->setMaximum(s.width());
		thumbHeigh->setMinimum(0);
		thumbHeigh->setMaximum(s.height());
		thumbSpace->setMinimum(0);
		thumbSpace->setMaximum(222);
		thumbRows->setMinimum(1);
		thumbRows->setMaximum(6);

		int r = 0;
		QGridLayout *layout = new QGridLayout;
		layout->addWidget(gl, r, 0);
		r++;
		layout->addWidget(showTips, r, 0);
		r++;
		layout->addWidget(new QLabel(QObject::tr("Horizontal margin")), r, 0);
		layout->addWidget(marginH, r, 1);
		r++;
		layout->addWidget(new QLabel(QObject::tr("Vertical margin")), r, 0);
		layout->addWidget(marginV, r, 1);
		r++;
		layout->addWidget(new QLabel(QObject::tr("Thumb width")), r, 0);
		layout->addWidget(thumbWidth, r, 1);
		r++;
		layout->addWidget(new QLabel(QObject::tr("Thumb height")), r, 0);
		layout->addWidget(thumbHeigh, r, 1);
		r++;
		layout->addWidget(new QLabel(QObject::tr("Thumb space")), r, 0);
		layout->addWidget(thumbSpace, r, 1);
		r++;
		layout->addWidget(new QLabel(QObject::tr("Thumb rows")), r, 0);
		layout->addWidget(thumbRows, r, 1);
		r++;
		layout->addWidget(clearCache, r, 0);
		r++;
		//layout->addWidget(backgroundColor, r, 0);
		//layout->addWidget(back, r, 1);r++;
		//layout->addWidget(glowColor, r, 0);
		//layout->addWidget(glow, r, 1);r++;
		//layout->addWidget(cancel, r, 0);
		//layout->addWidget(ok, r, 1);

		QWidget *content = new QWidget;
		content->setMinimumWidth(dialog->width() - 40);
		content->setLayout(layout);
		QScrollArea *area = new QScrollArea;
		area->setWidget(content);
		QHBoxLayout *hb = new QHBoxLayout;
		hb->addWidget(cancel);
		hb->addWidget(ok);
		QVBoxLayout *vb = new QVBoxLayout;
		vb->addWidget(area);
		vb->addLayout(hb);
		dialog->setLayout(vb);

		QObject::connect(clearCache, SIGNAL(clicked()), dialog, SLOT(clearCache()));
		QObject::connect(ok, SIGNAL(clicked()), dialog, SLOT(saveConfig()));
		QObject::connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));
		//QObject::connect(back, SIGNAL(clicked()), dialog, SLOT(selectBackgroundColor()));
		//QObject::connect(glow, SIGNAL(clicked()), dialog, SLOT(selectGlowColor()));


		QSettings cfg(Config::configPath, QSettings::IniFormat);
		cfg.setIniCodec("UTF-8");
		cfg.setPath(QSettings::IniFormat, QSettings::UserScope, Config::configPath);
		//backgroundColor->setAcceptDrops(.setRgb(cfg.value("backgroundColor", QColor(66, 66, 66).rgb()).value<QRgb>());
		//glowColor.setRgb(cfg.value("glowColor", QColor(Qt::green).lighter(100).rgb()).value<QRgb>());
		gl->setChecked(cfg.value("opengl", true).toBool());
		showTips->setChecked(cfg.value("showTips", true).toBool());

		marginH->setValue(cfg.value("contentHMargin", 666).toInt());
		marginV->setValue(cfg.value("contentVMargin", 88).toInt());

		//thumbMargin = cfg.value("thumbMargin", 4).toInt();
		//thumbBorder = cfg.value("thumbBorder", 1).toInt();

		thumbWidth->setValue(cfg.value("thumbItemWidth", 160).toInt());
		thumbHeigh->setValue(cfg.value("thumbItemHeight", 120).toInt());
		thumbRows->setValue(cfg.value("thumbRows", 3).toInt());
		thumbSpace->setValue(cfg.value("thumbSpacing", 4).toInt());

	}

	QCheckBox *gl, *showTips;
	QSpinBox *marginH, *marginV, *thumbWidth, *thumbHeigh;
	QSpinBox *thumbSpace;//, thumbMargin, thumbBorder;
	QSpinBox *thumbRows;
	QLabel *backgroundColor, glowColor;
	QPushButton *ok, *cancel;
	QPushButton *back, *glow;
	QPushButton *clearCache;
};

ConfigDialog::ConfigDialog(QWidget *parent) :
	QDialog(parent),d_ptr(new ConfigDialogPrivate)
{
	d_ptr->setupUi(this);
}


void ConfigDialog::selectBackgroundColor()
{

}

void ConfigDialog::selectGlowColor()
{

}

void ConfigDialog::saveConfig()
{
	Q_D(ConfigDialog);
	QSettings cfg(Config::configPath, QSettings::IniFormat);
	cfg.setIniCodec("UTF-8");
/*	cfg.setValue("backgroundColor", backgroundColor.rgb());
	cfg.setValue("glowColor", glowColor.rgb());*/
	cfg.setValue("opengl", d->gl->isChecked());
	cfg.setValue("showTips", d->showTips->isChecked());
	cfg.setValue("contentHMargin", d->marginH->value());
	cfg.setValue("contentVMargin", d->marginV->value());
	cfg.setValue("thumbRows", d->thumbRows->value());
	cfg.setValue("thumbSpacing", d->thumbSpace->value());

	cfg.setValue("thumbItemWidth", d->thumbWidth->value());
	cfg.setValue("thumbItemHeight", d->thumbHeigh->value());
	accept();
}

void ConfigDialog::clearCache()
{
	QDir thumbDir(Config::thumbDir);
	QStringList files = thumbDir.entryList(QDir::Files | QDir::Hidden);
	foreach(QString f, files) {
		QFile::remove(thumbDir.filePath(f));
	}

	QFile::remove(Config::thumbRecordFile);
}

} //namespace PhotoKit
