#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

#include <QString>

#include <QTimer>

#include <stbtypes.h>
#include <stbplayer.h>

class Player : public QObject
{
    Q_OBJECT
    HPlayer player;
    QTimer *volume_timer;
public:
    Player(QObject *parent = 0);
    ~Player();

    QTimer *volumeTimer()
    {
        return volume_timer;
    }

    void switchAspect();

    unsigned char volume();

public slots:
    void volume(int volume);
    void play(QString url);

};

#endif // PLAYER_H
