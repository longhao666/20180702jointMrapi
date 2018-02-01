#-------------------------------------------------
#
# Project created by QtCreator 2017-09-18T23:01:33
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OscilloScopeLH
TEMPLATE = lib
DESTDIR = ../Bin

DEFINES += OSCILLOSCOPE_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        oscilloscope.cpp \
    Component/paintarea.cpp \
    Component/showitem.cpp \
    Component/showqueue.cpp \
    oscilloscopethread.cpp

HEADERS += \
        oscilloscope.h \
        oscilloscope_global.h \
    Component/paintarea.h \
    Component/showitem.h \
    Component/showqueue.h \
    oscilloscopethread.h

FORMS += \
  oscilloscope.ui


#INCLUDEPATH += $$PWD/qwt/include
#LIBS += -L$$PWD/qwt/lib/ -lqwt

INCLUDEPATH += $$PWD/qwt/include
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qwt/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/qwt/lib/ -lqwtd
DEPENDPATH += $$PWD/qwt/lib


include(../mr-api.pri)
