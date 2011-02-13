#include "headers/lirchandler.h"

#include <QStringList>
#include <QWSServer>

LircHandler* LircHandler::_instance = 0;

std::map<QString, LircEvent> LircHandler::eventMap;

LircHandler::LircHandler()
{
}

void LircHandler::createEventMap()
{
    eventMap["Tab"]             = LircEvent::create(true, Qt::Key_Tab,      Qt::NoModifier);
    eventMap["shift-Tab"]       = LircEvent::create(true, Qt::Key_Tab,      Qt::ShiftModifier);
    eventMap["Escape"]          = LircEvent::create(true, Qt::Key_Escape,   Qt::NoModifier);
    eventMap["Up"]              = LircEvent::create(true, Qt::Key_Up,       Qt::NoModifier);
    eventMap["Down"]            = LircEvent::create(true, Qt::Key_Down,     Qt::NoModifier);
    eventMap["Left"]            = LircEvent::create(true, Qt::Key_Left,     Qt::NoModifier);
    eventMap["Right"]           = LircEvent::create(true, Qt::Key_Right,    Qt::NoModifier);
    eventMap["Return"]          = LircEvent::create(true, Qt::Key_Space,    Qt::NoModifier);

    eventMap["KeySym:0x30"]     = LircEvent::create(true, Qt::Key_0, Qt::NoModifier);
    eventMap["KeySym:0x31"]     = LircEvent::create(true, Qt::Key_1, Qt::NoModifier);
    eventMap["KeySym:0x32"]     = LircEvent::create(true, Qt::Key_2, Qt::NoModifier);
    eventMap["KeySym:0x33"]     = LircEvent::create(true, Qt::Key_3, Qt::NoModifier);
    eventMap["KeySym:0x34"]     = LircEvent::create(true, Qt::Key_4, Qt::NoModifier);
    eventMap["KeySym:0x35"]     = LircEvent::create(true, Qt::Key_5, Qt::NoModifier);
    eventMap["KeySym:0x36"]     = LircEvent::create(true, Qt::Key_6, Qt::NoModifier);
    eventMap["KeySym:0x37"]     = LircEvent::create(true, Qt::Key_7, Qt::NoModifier);
    eventMap["KeySym:0x38"]     = LircEvent::create(true, Qt::Key_8, Qt::NoModifier);
    eventMap["KeySym:0x39"]     = LircEvent::create(true, Qt::Key_9, Qt::NoModifier);

    eventMap["alt-KeySym:0x6c"] = LircEvent::create(false, STBEvent::Aspect,    0);
    eventMap["KP_Add"]          = LircEvent::create(false, STBEvent::Volume,    1);
    eventMap["minus"]           = LircEvent::create(false, STBEvent::Volume,   -1);
    eventMap["Page_Up"]         = LircEvent::create(false, STBEvent::Page,      1);
    eventMap["Page_Down"]       = LircEvent::create(false, STBEvent::Page,     -1);
}

void LircHandler::handle(QObject* who, QString lircMsg)
{
    QStringList parts = lircMsg.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    if(parts.at(0) == "Key")
    {
        std::map<QString, LircEvent>::const_iterator res = eventMap.find(parts.at(1));

        if(res != eventMap.end())
        {
            if(res->second.isKeyEvent)
            {
                QWSServer *srv = QWSServer::instance();

                srv->sendKeyEvent(-1, res->second.event.keyEvent.key, res->second.event.keyEvent.mod, true, false);
                srv->sendKeyEvent(-1, res->second.event.keyEvent.key, res->second.event.keyEvent.mod, false, false);
            }
            else
            {
                STBEvent* evt = new STBEvent((STBEvent::Type)res->second.event.stbEvent.event);
                evt->setPayload(res->second.event.stbEvent.payload);
                QCoreApplication *app = QApplication::instance();
                app->sendEvent(who, evt);
                delete evt;
            }
        }
    }
}
