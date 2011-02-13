#ifndef STBEVENT_H
#define STBEVENT_H

#include <QEvent>

class STBEvent : public QEvent
{
    int payload;
public:
    enum Type
    {
        Generic = QEvent::User + 1,
        Volume,
        Page,
        Aspect
    };


    STBEvent();
    STBEvent(Type type)
        : QEvent((QEvent::Type) type)
    {

    }

    int getPayload()
    {
        return payload;
    }

    void setPayload(int payload)
    {
        this->payload = payload;
    }
};

class STBPageEvent : public STBEvent
{
public:
    STBPageEvent() : STBEvent(Page)
    {

    }
};

class STBVolumeEvent : public STBEvent
{
public:
    STBVolumeEvent() : STBEvent(Volume)
    {

    }
};


#endif // STBEVENT_H
