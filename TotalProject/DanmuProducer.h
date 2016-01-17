#ifndef DANMUPRODUCER_H
#define DANMUPRODUCER_H
#include <QSemaphore>
#include <QThread>
#include <QTime>
#include <QMutex>
#include <QWaitCondition>

class DanmuProducer : public QThread{

public:
     void run();
};

#endif // DANMUPRODUCER_H
