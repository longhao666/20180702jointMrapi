include(./joint.pri)

TEMPLATE = lib
DESTDIR = ../Bin

win32 {
    TARGET = mrapi
    DEF_FILE += $$PWD/mrapi.def
}

QT -= gui

DEFINES += QT_DEPRECATED_WARNINGS


