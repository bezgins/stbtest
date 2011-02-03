#ifndef VOLUMEWIDGET_H
#define VOLUMEWIDGET_H

#include <QWidget>
#include "player.h"

namespace Ui {
    class VolumeWidget;
}

class VolumeWidget : public QWidget {
    Q_OBJECT
public:
    VolumeWidget(QWidget *parent = 0);
    ~VolumeWidget();

protected:
    void changeEvent(QEvent *e);

private:
    virtual void timerEvent(QTimerEvent *);

signals:
    void volumeChanged(int value);

public:
    void volumeShift(int amount);
    void volume(int volume);

private:
    Ui::VolumeWidget *ui;
    Player *player;
    int timer;
};

#endif // VOLUMEWIDGET_H
