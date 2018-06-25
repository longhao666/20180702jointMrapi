include(./mr-api.pri)

TEMPLATE = lib
DESTDIR = ../Bin

win32 {
    TARGET = mrapi
    DEF_FILE += $$PWD/mrapi_vs/mrapi.def
}

QT -= gui

DEFINES += QT_DEPRECATED_WARNINGS
