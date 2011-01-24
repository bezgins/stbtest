#ifndef LIRCTHREAD_H
#define LIRCTHREAD_H

#include <QThread>
#include <lirc/lirc_client.h>

class LircThread : public QThread
{
    Q_OBJECT

    lirc_config *cfg;

public:
    LircThread(QObject *parent = 0);

    ~LircThread();

protected:
    virtual void run();
};

#endif // LIRCTHREAD_H
