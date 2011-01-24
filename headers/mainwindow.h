#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stbtypes.h>
#include <stbplayer.h>

#include <QMainWindow>
#include <QSocketNotifier>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SetPlayer(HPlayer player)
    {
        this->player = player;
    }

    // Unix signal handlers.
    static void termSignalHandler(int unused);
    static void intSignalHandler(int unused);

protected:
    void changeEvent(QEvent *e);

public slots:
    void channel1();
    void channel2();
    void handleSigInt();
    void handleSigTerm();

private:
    Ui::MainWindow *ui;
    HPlayer player;

    static int sigintFd[2];
    static int sigtermFd[2];

    QSocketNotifier *snInt;
    QSocketNotifier *snTerm;

};

#endif // MAINWINDOW_H
