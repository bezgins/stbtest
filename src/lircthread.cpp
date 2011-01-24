#include "headers/lircthread.h"
#include <QWSServer>

LircThread::LircThread(QObject *parent):QThread(parent)
{
    if(lirc_init("stbapp", 1) == -1)
        throw QString("Can not init lirc subsystem");

    if(lirc_readconfig("/etc/lirc/lircrc.wk", &cfg, NULL))
        throw QString("Can not read lirc config");
}

LircThread::~LircThread()
{
    lirc_freeconfig(cfg);
    lirc_deinit();
}

void LircThread::run()
{
    QWSServer *srv = QWSServer::instance();

    char *code;
    char *c;
    int ret;

    while(lirc_nextcode(&code)==0)
    {
        if(code==NULL) continue;
        while((ret=lirc_code2char(cfg,code,&c))==0 &&
              c!=NULL)
        {
            QString str(c);

            bool guessed = false;

            int key;
            Qt::KeyboardModifier mod = Qt::NoModifier;

            if(str == "Key Tab Web")
            {
                guessed = true;
                key = Qt::Key_Tab;
            }

            if(guessed)
                srv->sendKeyEvent(0, key, mod, true, false);
        }

        if(ret==-1) break;
    }

}
