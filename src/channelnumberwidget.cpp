#include "headers/channelnumberwidget.h"
#include "ui_channelnumberwidget.h"

#include <QDebug>
#include <QFile>

ChannelNumberWidget::ChannelNumberWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelNumberWidget)
{
    Channel ch;
    int number = 0;

    QFile chnls("./chnls.txt");
    QString line;

    if(chnls.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream t( &chnls );

        while(!t.atEnd())
        {
            line = t.readLine();

            number++;

            ch.number = number;
            ch.url = line;

            channels.push_back(ch);

            qDebug() << line << " read";
        }

        chnls.close();
    }

    currentChannel = 1;

    ui->setupUi(this);

    connect(this, SIGNAL(channel(int)), this->ui->lcdNumber, SLOT(display(int)));

    edit = false;

    this->setVisible(false);
}

ChannelNumberWidget::~ChannelNumberWidget()
{
    delete ui;
}

void ChannelNumberWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ChannelNumberWidget::current()
{
    edit = false;
    showChannel();
}

void ChannelNumberWidget::channelUp()
{
    edit = false;
    currentChannel++;

    showChannel();
}

void ChannelNumberWidget::channelDown()
{
    edit = false;
    currentChannel--;

    showChannel();
}

void ChannelNumberWidget::number(int digit)
{
    this->setVisible(true);

    if(!edit)
    {
        num = 0;
        edit = true;
    }

    num = num * 10 + digit;

    if(num > 999)
        num = digit;

    emit channel(num);

    if(timer)
        killTimer(timer);

    timer = this->startTimer(2000);
}

void ChannelNumberWidget::showChannel()
{
    this->setVisible(true);

    if(currentChannel < 1)
        currentChannel = channels.size();

    if(currentChannel > channels.size())
        currentChannel = 1;

    emit channel(channels[currentChannel-1].url);
    emit channel(channels[currentChannel-1].number);

    if(timer)
        killTimer(timer);

    timer = this->startTimer(2000);
}

void ChannelNumberWidget::timerEvent(QTimerEvent *e)
{
    killTimer(timer);
    timer = 0;

    if(edit)
    {
        tryChannel(num);
        edit = false;
    }

    this->setVisible(false);
}

void ChannelNumberWidget::tryChannel(int number)
{
    for(std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if(it->number == number)
        {
            currentChannel = it->number;
            emit channel(it->url);
            emit channel(it->number);
            break;
        }
    }
}
