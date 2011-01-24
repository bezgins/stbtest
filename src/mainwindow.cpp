#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <sys/socket.h>

#include <QWSServer>

#include <QMessageBox>

#include <QtDebug>

int MainWindow::sigintFd[2];
int MainWindow::sigtermFd[2];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigintFd))
       qFatal("Couldn't create HUP socketpair");

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
       qFatal("Couldn't create TERM socketpair");

    snInt = new QSocketNotifier(sigintFd[1], QSocketNotifier::Read, this);
    connect(snInt, SIGNAL(activated(int)), this, SLOT(handleSigInt()));
    snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
    connect(snTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));

    ui->setupUi(this);

    std::cout << "Started" << std::endl;

    connect(ui->channel1Button, SIGNAL(clicked()), this,
                        SLOT(channel1()));

    connect(ui->channel2Button, SIGNAL(clicked()), this,
                        SLOT(channel2()));

    //this->setWindowOpacity(0.1);
    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    ui->channel1Button->setWindowOpacity(1.0);
    ui->channel2Button->setWindowOpacity(1.0);
}

void MainWindow::setLircFd(int fd)
{
    lircFd = fd;

    snLirc = new QSocketNotifier(lircFd, QSocketNotifier::Read, this);
    connect(snLirc, SIGNAL(activated(int)), this, SLOT(handleLirc()));

}

void MainWindow::handleLirc()
{
    snLirc->setEnabled(false);

    char str[255];
    int sz;

    ::read(lircFd, &sz, sizeof(sz));
    ::read(lircFd, str, sz);

    QString cmd(str);

    qDebug() << QString(str);


    bool guessed = false;

    int key;
    Qt::KeyboardModifier mod = Qt::NoModifier;

    QStringList parts = cmd.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    if(parts.at(0) == "Key")
    {
        qDebug() << "Key caught";

        if(parts.at(1) == "Tab")
        {
            qDebug() << "Tab";

            guessed = true;
            key = Qt::Key_Tab;
        }
        else if(parts.at(1) == "shift-Tab")
        {
            qDebug() << "Shift-Tab";

            guessed = true;
            key = Qt::Key_Tab;
            mod = Qt::ShiftModifier;
        }
        else if(parts.at(1) == "Return")
        {
            qDebug() << "Return";

            guessed = true;
            key = Qt::Key_Space;
        }
    }

    if(guessed)
    {
        QWSServer *srv = QWSServer::instance();

        srv->sendKeyEvent(-1, key, mod, true, false);
        srv->sendKeyEvent(-1, key, mod, false, false);
    }

    snLirc->setEnabled(true);
}

void MainWindow::intSignalHandler(int)
{
    char a = 1;
    ::write(sigintFd[0], &a, sizeof(a));
}

void MainWindow::termSignalHandler(int)
{
    char a = 1;
    ::write(sigtermFd[0], &a, sizeof(a));
}

void MainWindow::handleSigTerm()
{
    snTerm->setEnabled(false);
    char tmp;
    ::read(sigtermFd[1], &tmp, sizeof(tmp));

    this->close();

    snTerm->setEnabled(true);
}

void MainWindow::handleSigInt()
{
    snInt->setEnabled(false);
    char tmp;
    ::read(sigintFd[1], &tmp, sizeof(tmp));

    this->close();

    snInt->setEnabled(true);
}

MainWindow::~MainWindow()
{
    STB_Stop(player);
    STB_ReleasePlayer(player);

    delete ui;
}

void MainWindow::channel1()
{
    STB_Stop(player);

    STB_PlaySolution(player, "auto", "udp://225.50.71.1:1234");

//    int aspect = STB_GetAspect(player);
//
//    QMessageBox msg;
//    msg.setText(QString("Aspect is %1").arg(aspect));
//    msg.exec();
}

void MainWindow::channel2()
{
    STB_Stop(player);

    STB_PlaySolution(player, "auto", "udp://225.50.64.1:1234");
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
