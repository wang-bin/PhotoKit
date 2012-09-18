#ifndef SHARE_H
#define SHARE_H

#include <QtCore/QObject>

namespace PhotoKit {

class Share : public QObject
{
    Q_OBJECT
public:
    explicit Share(QObject *parent = 0);
    
signals:
    
public slots:
    
};

} //namespace PhotoKit
#endif // SHARE_H
