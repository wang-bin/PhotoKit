#ifndef NFCSHARE_H
#define NFCSHARE_H

#include "Share.h"

namespace PhotoKit {

class NFCShare : public Share
{
    Q_OBJECT
public:
    explicit NFCShare(QObject *parent = 0);
    
signals:
    
public slots:
    
};

} //namespace PhotoKit
#endif // NFCSHARE_H
