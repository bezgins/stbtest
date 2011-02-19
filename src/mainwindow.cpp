#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <sys/socket.h>

#include <QMessageBox>
#include <QDebug>
#include <QEvent>

#include "lirchandler.h"

int MainWindow::sigintFd[2];
int MainWindow::sigtermFd[2];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    player(new Player(this))
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

    this->setStyleSheet("background: rgb(255,255,255)");
    this->setWindowFlags(Qt::FramelessWindowHint);

}

void MainWindow::init()
{
    volume->volume(player->volume());

    connect(volume, SIGNAL(volumeChanged(int)), player, SLOT(volume(int)));
    connect(chNum, SIGNAL(channel(QString)), player, SLOT(play(QString)));

    this->ui->gridLayout_2->addWidget(chNum, 0, 0, 1, 1);
    this->ui->gridLayout_2->addWidget(volume, 2, 0, 1, 3);

    this->chNum->current();
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

    LircHandler::instance()->handle(this, QString(str));

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
    delete player;
    delete ui;
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

void MainWindow::switchAspect()
{
    player->switchAspect();
}

bool MainWindow::processKey(QKeyEvent *event)
{
    if(event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
    {
        chNum->number(event->key() - 0x30);
        event->setAccepted(true);
        return true;
    }

    return false;
}

bool MainWindow::processSTBEvent(STBEvent *event)
{
    switch(event->type())
    {
    case STBEvent::Volume:
            volume->volumeShift(event->getPayload());;
            return true;
            break;
    case STBEvent::Page:
            if(event->getPayload() == 1)
                chNum->channelUp();
            else
                chNum->channelDown();
            return true;
            break;
    case STBEvent::Aspect:
            switchAspect();
            return true;
            break;
    case STBEvent::Power:
            player->power();
            if(!player->isStandBy())
            {
                chNum->current();
            }
            return true;
            break;
    default:
            break;
    }

    return false;
}

bool MainWindow::event(QEvent *event)
{
    QEvent::Type type = event->type();

    if(type == QEvent::KeyPress)
    {
        if(processKey(static_cast<QKeyEvent*>(event)))
        {
            return true;
        }
    }
    else if(type >= QEvent::User)
    {
        if(processSTBEvent(dynamic_cast<STBEvent*>(event)))
        {
            return true;
        }
    }

    return QMainWindow::event(event);
}
