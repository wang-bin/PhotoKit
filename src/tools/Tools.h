#ifndef PHTOKIT_TOOLS_H
#define PHTOKIT_TOOLS_H

class QString;
class QStringList;
namespace PhotoKit {

namespace Tools {

void showTip(const QString& text, bool force = false, int msshow = 8000);

QStringList imageNameFilters();

} //namespace Tools

} //namespace PhotoKit


#endif // PHTOKIT_TOOLS_H
