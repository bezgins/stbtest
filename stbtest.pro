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
LIBS += -L/home/kirdyk/X-sh4/lib
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
    src/dialog.cpp \
    src/lircthread.cpp
HEADERS += headers/mainwindow.h \
    headers/dialog.h \
    headers/lircthread.h
FORMS += ui/mainwindow.ui \
    ui/dialog.ui
