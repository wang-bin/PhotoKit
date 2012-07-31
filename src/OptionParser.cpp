/******************************************************************************
	OptionParser: commandline parser based on my ProgramOptions
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


#include "OptionParser.h"
#include <QtCore/QDir>
#include "ProgramOptions.h"
#include "ThumbRecorder.h"
#include "tools/Tools.h"
#include "Config.h"

//priority: cmd > config.ini > auto detect
namespace po = ProgramOptions;

namespace PhotoKit {
QStringList OptionParser::images;
void OptionParser::parseCmd(int argc, const char *const*argv)
{
    static OptionParser cmd;
	po::parse(argc, argv);
    if (po::get("h"))
        po::help();
	if (po::get("no-thumb")) {
		Config::useThumb = false;
	}
    if (po::get("opengl")) {
        Config::openGlRendering = true;
    }
    if (po::get("no-opengl")) {
        Config::openGlRendering = false;
    }
	if (po::get("logfile")) {
		Config::logToFile = true;
	}
    QString d(po::get("d").str().c_str());
    if (!d.isEmpty()) {
        getImagesFromDirs(d.split(";"));
    }
    QString f(po::get("f").str().c_str());
	if (!f.isEmpty()) {
		Config::showLastDisplayed = false;
		images << f.split(";");
	}
	if (images.isEmpty()) {
		images << *ThumbRecorder::instance()->displayedThumbs();
	}
}

OptionParser::OptionParser()
{
    po::summary("PhotoKit is a photo browser with impressive 3d effects.\n"
                            "Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>")
            ["Usage:"]
            ("-h,--help", "show this message")
			("--no-thumb", "load images without using thumbnails")
			("--opengl", "enable opengl")
			("--no-opengl", "disable opengl")
            ("-d,--dirs", "", "load images from dirs")
            ("-f,--files", "", "load image from files")
			("--logfile", "enable log to file")
    ;

	//the read after detect so that the configuration in config file will be applied
	Config::detectSystemResources();
	Config::read();
	Config::postConfigure();
}

void OptionParser::getImagesFromDirs(const QStringList &dirs)
{
    images.clear();
    foreach(const QString& dir, dirs) {
        QDir d(dir);
        if (!d.exists())
            continue;
		QStringList list = d.entryList(Tools::imageNameFilters(), QDir::Files);
        list.replaceInStrings(QRegExp("^(.*)"), dir + "/\\1");
        images << list;
    }
}

}
