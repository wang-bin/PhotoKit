#ifndef PHOTOKIT_CMDPARSER_H
#define PHOTOKIT_CMDPARSER_H

class QStringList;
namespace PhotoKit {

class OptionParser {
public:
    static void parseCmd(int argc, const char* const* argv);
    static QStringList images;
private:
    OptionParser();
    static void getImagesFromDirs(const QStringList& dirs);
};



}
#endif // PHOTOKIT_CMDPARSER_H
