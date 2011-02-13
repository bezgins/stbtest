# -------------------------------------------------
# Project created by QtCreator 2011-01-21T23:10:54
# -------------------------------------------------
TARGET = stbtest
TEMPLATE = app
DESTDIR = bin
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = uic
INCLUDEPATH += headers
INCLUDEPATH += /home/kirdyk/X-sh4/include
LIBS += -L/home/kirdyk/X-sh4/lib \
    -Wl,-rpath,/home/default
LIBS += -lstbplayer
LIBS += -lvmclient \
    -lSecureMedia \
    -lfilter-cas \
    -lavformat \
    -lavcodec \
    -llirc_client
LIBS += -lavutil \
    -lavcore \
    -lfreetype \
    -lz
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/lircthread.cpp \
    src/stbevent.cpp \
    src/player.cpp \
    src/volumewidget.cpp \
    src/channelnumberwidget.cpp \
    src/lirchandler.cpp
HEADERS += headers/mainwindow.h \
    headers/lircthread.h \
    headers/stbevent.h \
    headers/player.h \
    headers/volumewidget.h \
    headers/channelnumberwidget.h \
    headers/lirchandler.h
FORMS += ui/mainwindow.ui \
    ui/volumewidget.ui \
    ui/channelnumberwidget.ui
OTHER_FILES += chnls.txt
RESOURCES += res.qrc
