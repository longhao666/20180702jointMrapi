
SOURCES += \
    $$PWD/src/bottom.cpp \
    $$PWD/src/move.cpp \
    $$PWD/src/oscilloscope.cpp \
    $$PWD/src/oscilloscopethread.cpp \
    $$PWD/src/paintarea.cpp \
    $$PWD/src/showitem.cpp \
    $$PWD/src/showqueue.cpp \
    $$PWD/src/pid.cpp \
    $$PWD/src/setAndHealth.cpp \

HEADERS+= \
    $$PWD/include/bottom.h \
    $$PWD/include/move.h \
    $$PWD/include/oscilloscope.h \
    $$PWD/include/oscilloscopethread.h \
    $$PWD/include/paintarea.h \
    $$PWD/include/showitem.h \
    $$PWD/include/showqueue.h \
    $$PWD/include/pid.h \
    $$PWD/include/setAndHealth.h \

FORMS += \
    $$PWD/ui/bottom.ui \
    $$PWD/ui/move.ui \
    $$PWD/ui/oscilloscope.ui \
    $$PWD/ui/pid.ui \
    $$PWD/ui/setAndHealth.ui \


INCLUDEPATH += $$PWD/include/

#INCLUDEPATH += $$PWD/qwt/include/
#LIBS += -L$$PWD/qwt/lib -lqwt
INCLUDEPATH += $$PWD/qwt/include
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qwt/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/qwt/lib/ -lqwtd
DEPENDPATH += $$PWD/qwt/lib


##Bottom
#SOURCES += \
#    $$PWD/Bottom/bottom.cpp

#HEADERS += \
#    $$PWD/Bottom/bottom.h

#FORMS += \
#    $$PWD/Bottom/bottom.ui

##Move
#SOURCES += \
#    $$PWD/Move/move.cpp

#HEADERS += \
#    $$PWD/Move/move.h

#FORMS += \
#    $$PWD/Move/move.ui

##SetAndHealth
#SOURCES += \
#    $$PWD/SetAndHealth/setAndHealth.cpp

#HEADERS += \
#    $$PWD/SetAndHealth/setAndHealth.h

#FORMS += \
#    $$PWD/SetAndHealth/setAndHealth.ui

##Pid
#SOURCES += \
#    $$PWD/Pid/pid.cpp

#HEADERS += \
#    $$PWD/Pid/pid.h

#FORMS += \
#    $$PWD/Pid/pid.ui

##Oscilloscope
#SOURCES += \
#    $$PWD/Oscilloscope/oscilloscope.cpp \
#    $$PWD/Oscilloscope/oscilloscopethread.cpp \
#    $$PWD/OscilloScope/Component/paintarea.cpp \
#    $$PWD/OscilloScope/Component/showitem.cpp \
#    $$PWD/OscilloScope/Component/showqueue.cpp \

#HEADERS += \
#    $$PWD/Oscilloscope/oscilloscope.h \
#    $$PWD/Oscilloscope/oscilloscopethread.h \
#    $$PWD/OscilloScope/Component/paintarea.h \
#    $$PWD/OscilloScope/Component/showitem.h \
#    $$PWD/OscilloScope/Component/showqueue.h \

#FORMS += \
#    $$PWD/Oscilloscope/oscilloscope.ui

##INCLUDEPATH += $$PWD/OscilloScope/qwt/include
##LIBS += -L$$PWD/OscilloScope/qwt/lib -lqwt
#INCLUDEPATH += $$PWD/qwt/include
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qwt/lib/ -lqwt
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/qwt/lib/ -lqwtd
#DEPENDPATH += $$PWD/qwt/lib

#INCLUDEPATH += $$PWD/OscilloScope/Component

#SOURCES += \
#    $$PWD/src/joint.c \
#    $$PWD/src/logger.c \
#    $$PWD/src/master.c \
#    $$PWD/src/module.c \


#HEADERS += \
#    $$PWD/include/can_driver.h \
#    $$PWD/include/joint.h \
#    $$PWD/include/logger.h \
#    $$PWD/include/master.h \
#    $$PWD/include/module.h \
#    $$PWD/include/mrapi.h \

#SOURCES += \
#    $$PWD/pcan_basic/pcan_basic.c \
#    $$PWD/pcan_basic/thread_linux.c \
#    $$PWD/pcan_basic/thread_win.c \

#HEADERS += \
#    $$PWD/pcan_basic/pcan_basic.h \

#INCLUDEPATH += $$PWD/include
#INCLUDEPATH += $$PWD/pcan_basic

#INCLUDEPATH += $$PWD/PCANBasic/Include
#LIBS += -L$$PWD/PCANBasic/x64 -lPCANBasic
#DEPENDPATH += -L$$PWD/PCANBasic/x64 -lPCANBasic
#LIBS += -L$$PWD/PCANBasic/x64/VC_LIB -lPCANBasic

