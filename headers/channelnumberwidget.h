#ifndef CHANNELNUMBERWIDGET_H
#define CHANNELNUMBERWIDGET_H

#include <QWidget>
#include <QString>
#include <vector>

struct Channel{
    int number;
    QString url;
};

namespace Ui {
    class ChannelNumberWidget;
}

class ChannelNumberWidget : public QWidget {
    Q_OBJECT
public:
    ChannelNumberWidget(QWidget *parent = 0);
    ~ChannelNumberWidget();

protected:
    void changeEvent(QEvent *e);

    virtual void timerEvent(QTimerEvent *e);

    void showChannel();

signals:
    void channel(int value);
    void channel(QString value);
    void channelNum(int value);

public slots:
    void channelUp();
    void channelDown();
    void current();
    void number(int digit);
    void tryChannel(int number);

private:
    Ui::ChannelNumberWidget *ui;
    int currentChannel;
    std::vector<Channel> channels;
    int timer;
    bool edit;
    int num;
};

#endif // CHANNELNUMBERWIDGET_H
