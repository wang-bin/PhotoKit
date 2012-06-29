#include "OptionParser.h"
#include <QtCore/QDir>
#include <QtGui/QImageReader>
#include "ProgramOptions.h"
#include "Config.h"
namespace po = ProgramOptions;

namespace PhotoKit {
static QStringList image_formats;
QStringList OptionParser::images;
void OptionParser::parseCmd(int argc, const char *const*argv)
{
    static OptionParser cmd;
    po::parse(argc, argv);
    if (po::get("h"))
        po::help();
    if (po::get("opengl")) {
        Config::openGlRendering = true;
    }
    if (po::get("no-opengl")) {
        Config::openGlRendering = false;
    }
    QString d(po::get("d").str().c_str());
    if (!d.isEmpty()) {
        getImagesFromDirs(d.split(";"));
    }
    QString f(po::get("f").str().c_str());
    if (!f.isEmpty())
        images << f.split(";");
    if (images.isEmpty())
        getImagesFromDirs(QStringList() << "~");

    Config::postConfigure();
}

OptionParser::OptionParser()
{
    po::summary("PhotoKit is a photo browser with impressive 3d effects.\n"
                            "Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>")
            ["Usage:"]
            ("-h,--help", "show this message")
            ("--opengl", "enable opengl")
            ("--no-opengl", "disable opengl")
            ("-d,--dirs", "", "load images from dirs")
            ("-f,--files", "", "load image from files")
    ;
    image_formats.clear();
    foreach(QByteArray f, QImageReader::supportedImageFormats()) {
        image_formats << QString("*." + f);
    }
    Config::read();
    Config::detectSystemResources();
}

void OptionParser::getImagesFromDirs(const QStringList &dirs)
{
    images.clear();
    foreach(const QString& dir, dirs) {
        QDir d(dir);
        if (!d.exists())
            continue;
        QStringList list = d.entryList(image_formats, QDir::Files);
        list.replaceInStrings(QRegExp("^(.*)"), dir + "/\\1");
        images << list;
    }
}

}
