#include "headers/player.h"

Player::Player(QObject *parent)
    :QObject(parent)
{
    volume_timer = new QTimer();

    player =  STB_CreatePlayer();

    STB_SetVideoControl(player, 0);

    STB_SetTopWin(player, 0);

    STB_SetAspect(player, 0x10);

    STB_SetChromaKey(player, 0xffffff, 0xffffff);
}

Player::~Player()
{
    STB_Stop(player);
    STB_ReleasePlayer(player);

    delete volume_timer;
}

void Player::play(QString url)
{
    STB_Stop(player);

    STB_PlaySolution(player, "auto", url.toAscii());
}

void Player::switchAspect()
{
    unsigned char aspect = STB_GetAspect(player);

    aspect /= 0x10;

    ++aspect;

    aspect = (aspect >5) ? 0 : aspect;

    STB_SetAspect(player, aspect * 0x10);
}

unsigned char Player::volume()
{
    volume_timer->start(1000);
    return STB_GetVolume(player);
}

void Player::volume(int volume)
{
    int value = (volume > 100) ? 100 : volume;

    value = (value < 0 )? 0 : value;

    STB_SetVolume(player, value);
}
