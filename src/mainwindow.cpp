#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <sys/socket.h>

#include <QWSServer>
#include <QMessageBox>
#include <QDebug>
#include <QEvent>

int MainWindow::sigintFd[2];
int MainWindow::sigtermFd[2];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    chNum(new ChannelNumberWidget(this)),
    player(new Player(this)),
    volume(new VolumeWidget(this))
{
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigintFd))
       qFatal("Couldn't create HUP socketpair");

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
       qFatal("Couldn't create TERM socketpair");

    volume->volume(player->volume());

    connect(volume, SIGNAL(volumeChanged(int)), player, SLOT(volume(int)));
    connect(chNum, SIGNAL(channel(QString)), player, SLOT(play(QString)));

    snInt = new QSocketNotifier(sigintFd[1], QSocketNotifier::Read, this);
    connect(snInt, SIGNAL(activated(int)), this, SLOT(handleSigInt()));
    snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
    connect(snTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));

    ui->setupUi(this);

    std::cout << "Started" << std::endl;

    this->setStyleSheet("background: rgb(255,255,255)");
    this->setWindowFlags(Qt::FramelessWindowHint);

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

    QString cmd(str);

    qDebug() << QString(str);


    bool guessed = false;
    bool custom = false;

    int key;
    Qt::KeyboardModifier mod = Qt::NoModifier;

    QStringList parts = cmd.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    STBEvent *event;

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
        else if(parts.at(1) == "alt-KeySym:0x6c")
        {
            guessed = true;
            key = Qt::Key_A;
        }
        else if(parts.at(1) == "KP_Add")
        {
            guessed = true;
            custom = true;
            event = new STBVolumeEvent();
            event->setPayload(1);
        }
        else if(parts.at(1) == "minus")
        {
            guessed = true;
            custom = true;
            event = new STBVolumeEvent();
            event->setPayload(-1);
        }
        else if(parts.at(1) == "Page_Up")
        {
            guessed = true;
            custom = true;
            event = new STBPageEvent();
            event->setPayload(1);
        }
        else if(parts.at(1) == "Page_Down")
        {
            guessed = true;
            custom = true;
            event = new STBPageEvent();
            event->setPayload(-1);
        }
    }

    if(guessed)
    {
        if(!custom)
        {
            QWSServer *srv = QWSServer::instance();

            srv->sendKeyEvent(-1, key, mod, true, false);
            srv->sendKeyEvent(-1, key, mod, false, false);
        }
        else
        {
            QCoreApplication *app = QApplication::instance();
            app->sendEvent(this, event);
        }
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
    delete player;
    delete ui;
}

void MainWindow::channel1()
{
    player->play("udp://225.50.71.3:1234");
}

void MainWindow::channel2()
{
    player->play("udp://225.50.64.1:1234");
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
    switch(event->key())
    {
    case Qt::Key_A:
        switchAspect();
        return true;
        break;
    default:
        break;
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
            delete event;
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
