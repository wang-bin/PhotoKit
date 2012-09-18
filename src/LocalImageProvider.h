#ifndef LOCALIMAGEPROVIDER_H
#define LOCALIMAGEPROVIDER_H

#include "ImageProvider.h"

namespace PhotoKit {

class LocalImageProviderPrivate;
class LocalImageProvider : public ImageProvider
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LocalImageProvider)
public:
    explicit LocalImageProvider(QObject *parent = 0);
    
    virtual bool canFetchMore() const;
    virtual void fetchMore() = 0;
};

} //namespace PhotoKit {
#endif // LOCALIMAGEPROVIDER_H
