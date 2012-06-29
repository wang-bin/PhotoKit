#ifndef PHOTOKIT_CMDPARSER_H
#define PHOTOKIT_CMDPARSER_H

class QStringList;
namespace PhotoKit {

class CmdParser {
public:
    static void parse(int argc, const char* const* argv);
    static QStringList images;
private:
    CmdParser();
    static void getImagesFromDirs(const QStringList& dirs);
};



}
#endif // PHOTOKIT_CMDPARSER_H
