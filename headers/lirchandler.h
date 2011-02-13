#ifndef LIRCHANDLER_H
#define LIRCHANDLER_H

#include <Qt>
#include <QEvent>
#include <map>
#include "stbevent.h"

#include <QDebug>

struct STBKeyEventStruct
{
    int key;
    Qt::KeyboardModifier mod;
};

struct STBEventStruct
{
    int event;
    int payload;
};

struct LircEvent
{
    bool isKeyEvent;

    union _event
    {
        STBKeyEventStruct keyEvent;
        STBEventStruct stbEvent;
    } event;

    static LircEvent create(bool isKey, int key, int mod)
    {
        LircEvent result;

        result.isKeyEvent = isKey;

        if(result.isKeyEvent)
        {
            result.event.keyEvent.key = key;
            result.event.keyEvent.mod = (Qt::KeyboardModifier)mod;
        }
        else
        {
            result.event.stbEvent.event = key;
            result.event.stbEvent.payload = mod;
        }

        return result;
    }
};

class LircHandler
{
private:
    LircHandler();
    static LircHandler* _instance;

    static std::map<QString, LircEvent> eventMap;

    static void createEventMap();

public:

    static LircHandler* instance()
    {
        if(! _instance)
        {
            _instance = new LircHandler();
            createEventMap();
        }

        return _instance;
    }

    void handle(QObject* who, QString lircMsg);
};

#endif // LIRCHANDLER_H
