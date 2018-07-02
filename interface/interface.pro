
include(./interface.pri)

TEMPLATE = app
DESTDIR = ../Bin
TARGET = MRJDebugger

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD/../mr-api/include

LIBS += -L$$OUT_PWD/../Bin -lmrapi

win32 {
    LIBS += -L$$PWD/../joint/winLib -lPCANBasic
}

unix {
    LIBS += -L/usr/lib -lpcanbasic
}

DEFINES += LHDEBUG