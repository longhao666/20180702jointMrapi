include(./mr-api.pri)

TEMPLATE = lib
DESTDIR = ../Bin
TARGET = mrapi

win32 {

    DEF_FILE += $$PWD/mrapi_vs/mrapi.def
}

QT -= gui

DEFINES += QT_DEPRECATED_WARNINGS
