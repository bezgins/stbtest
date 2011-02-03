#include "headers/channelnumberwidget.h"
#include "ui_channelnumberwidget.h"

#include <QDebug>

ChannelNumberWidget::ChannelNumberWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelNumberWidget)
{
    Channel ch;

    ch.number = 1;
    ch.url = "udp://225.50.71.3:1234";

    channels.push_back(ch);

    ch.number = 2;
    ch.url = "udp://225.50.64.1:1234";

    channels.push_back(ch);

    ch.number = 112;
    ch.url = "udp://225.50.68.3:1234";

    channels.push_back(ch);

    currentChannel = 1;

    ui->setupUi(this);

    connect(this, SIGNAL(channel(int)), this->ui->lcdNumber, SLOT(display(int)));

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
    showChannel();
}

void ChannelNumberWidget::channelUp()
{
    currentChannel++;

    showChannel();
}

void ChannelNumberWidget::channelDown()
{
    currentChannel--;

    showChannel();
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

    this->setVisible(false);
}
