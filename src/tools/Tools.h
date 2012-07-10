#ifndef PHTOKIT_TOOLS_H
#define PHTOKIT_TOOLS_H

class QString;
class QStringList;
class QImage;
namespace PhotoKit {

namespace Tools {

//TODO: remove force param
void showTip(const QString& text, bool force = false, int msshow = 8000);
void showTip(const QImage &image, bool force = false, int msshow = 8000);
void showOk(int msshow = 8000);
void showError(int msshow = 8000);

QStringList imageNameFilters();

} //namespace Tools

} //namespace PhotoKit


#endif // PHTOKIT_TOOLS_H
