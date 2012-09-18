#include "LocalImageProvider.h"
#include "ImageProvider_p.h"
#include "ThumbTask.h"

namespace PhotoKit {

class LocalImageProviderPrivate : public ImageProviderPrivate
{
    ThumbTask *task;
};


LocalImageProvider::LocalImageProvider(QObject *parent) :
    ImageProvider(parent)
{
}

bool LocalImageProvider::canFetchMore() const
{
    return true;
}

void LocalImageProvider::fetchMore()
{

}

} //namespace PhotoKit {
