#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSocketNotifier>
#include <QKeyEvent>

#include "volumewidget.h"
#include "channelnumberwidget.h"
#include "stbevent.h"
#include "player.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SetPlayer(Player *player)
    {
        this->player = player;
    }

    // Unix signal handlers.
    static void termSignalHandler(int unused);
    static void intSignalHandler(int unused);

    void setLircFd(int fd);


protected:
    void changeEvent(QEvent *e);
    bool event(QEvent *event);

    bool processKey(QKeyEvent *event);

    bool processSTBEvent(STBEvent *event);

    void switchAspect();

public slots:
    void channel1();
    void channel2();
    void handleSigInt();
    void handleSigTerm();
    void handleLirc();

private:
    Ui::MainWindow *ui;
    ChannelNumberWidget *chNum;
    Player *player;
    VolumeWidget *volume;

    int lircFd;

    static int sigintFd[2];
    static int sigtermFd[2];

    QSocketNotifier *snInt;
    QSocketNotifier *snTerm;
    QSocketNotifier *snLirc;
};

#endif // MAINWINDOW_H
