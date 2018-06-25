
include(./interface.pri)

TEMPLATE = app
DESTDIR = ../Bin

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD/../joint/src/

win32 {
    TARGET = MRJDebugger
    LIBS += -L$$OUT_PWD/../Bin -lmrapi
    LIBS += -L$$PWD/../joint/winLib -lPCANBasic
}


DEFINES += LHDEBUG
