#include <QtGui/QApplication>
#include <stbtypes.h>
#include <stbplayer.h>
#include <signal.h>
#include <iostream>
#include "mainwindow.h"


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

//    PIG_Info pig;
//
//    pig.state = 0;
//
//    STB_SetPIG(player, &pig);

//    Viewport_Info vp;
//    vp.height=376;
//    vp.width=520;
//    vp.xpos=200;
//    vp.ypos=200;
//
//    STB_SetViewport(player, &vp);

    QApplication a(argc, argv);
    MainWindow w;

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
