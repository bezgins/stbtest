#include "headers/lircthread.h"
#include <QtDebug>
#include <sys/socket.h>

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
    char *code;
    char *c;
    int ret;

    while(lirc_nextcode(&code)==0)
    {
        if(code==NULL) continue;
        while((ret=lirc_code2char(cfg,code,&c))==0 &&
              c!=NULL)
        {

            int sz = strlen(code);
            ::write(lircFd, &sz, sizeof(sz));
            ::write(lircFd, c, sz);
        }

        if(ret==-1) break;
    }

}
