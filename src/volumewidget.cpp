#include "headers/volumewidget.h"
#include "ui_volumewidget.h"

VolumeWidget::VolumeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeWidget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    this->setStyleSheet("background:transparent;");

    this->setVisible(false);
}

VolumeWidget::~VolumeWidget()
{
    delete ui;
}

void VolumeWidget::changeEvent(QEvent *e)
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

void VolumeWidget::timerEvent(QTimerEvent *)
{
    killTimer(timer);
    timer = 0;

    this->setVisible(false);
}

void VolumeWidget::volumeShift(int amount)
{
    this->setVisible(true);

    if(timer)
        killTimer(timer);

    this->ui->progressBar->setValue(this->ui->progressBar->value() + amount);

    emit volumeChanged(this->ui->progressBar->value());

    timer = this->startTimer(1500);
}

void VolumeWidget::volume(int volume)
{
    this->ui->progressBar->setValue(volume);
    this->ui->lcdNumber->display(volume);
}
