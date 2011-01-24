#include <QtGui/QApplication>
#include <stbtypes.h>
#include <stbplayer.h>
#include <signal.h>
#include <iostream>
#include <sys/socket.h>
#include "mainwindow.h"
#include "lircthread.h"

#include <QtDebug>

int setup_unix_signal_handlers()
{
    struct sigaction sigint, term;

    sigint.sa_handler = MainWindow::intSignalHandler;
    sigemptyset(&sigint.sa_mask);
    sigint.sa_flags = 0;
    sigint.sa_flags |= SA_RESTART;

    if (sigaction(SIGINT, &sigint, 0) > 0)
        return 1;

    term.sa_handler = MainWindow::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &term, 0) > 0)
        return 2;

    return 0;
}

int main(int argc, char *argv[])
{
    HPlayer player ;
    player =  STB_CreatePlayer();

    STB_SetVideoControl(player, 0);

    STB_SetTopWin(player, 0);
    STB_SetAspect(player, 2);

    int lircFd[2];

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, lircFd))
       qFatal("Couldn't create HUP socketpair");

    QApplication a(argc, argv);
    MainWindow w;

    w.setLircFd(lircFd[0]);

    LircThread *thread = new LircThread();

    thread->lircFd = lircFd[1];

    thread->start();

    w.SetPlayer(player);

    w.show();

    w.showMaximized();

    int s;
    if(s = setup_unix_signal_handlers())
    {
        std::cout << s << std::endl;
        qFatal("Can not set up signal handlers!\n");
    }

    return a.exec();
}
