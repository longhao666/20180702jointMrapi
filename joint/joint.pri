
DEFINES += _WINDOWS
DEFINES += PCAN_BASIC

SOURCES += \
    $$PWD/src/joint.c \
    $$PWD/src/logger.c \
    $$PWD/src/master.c \
    $$PWD/src/module.c \


#HEADERS += \
#    $$PWD/include/can_driver.h \
#    $$PWD/include/joint.h \
#    $$PWD/include/logger.h \
#    $$PWD/include/master.h \
#    $$PWD/include/module.h \
#    $$PWD/include/mrapi.h \

SOURCES += \
    $$PWD/pcan_basic/pcan_basic.c \
#    $$PWD/pcan_basic/thread_linux.c \
    $$PWD/pcan_basic/thread_win.c \

HEADERS += \
    $$PWD/pcan_basic/pcan_basic.h \

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/pcan_basic

INCLUDEPATH += $$PWD/PCANBasic/Include
LIBS += -L$$PWD/PCANBasic/x64 -lPCANBasic
DEPENDPATH += -L$$PWD/PCANBasic/x64 -lPCANBasic
LIBS += -L$$PWD/PCANBasic/x64/VC_LIB -lPCANBasic

