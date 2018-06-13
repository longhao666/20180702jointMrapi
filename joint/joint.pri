
DEFINES += _WINDOWS
DEFINES += PCAN_BASIC

SOURCES += \
    $$PWD/src/joint.c \
    $$PWD/src/logger.c \
    $$PWD/src/master.c \
    $$PWD/src/module.c \
    $$PWD/src/gripper.c \
    $$PWD/src/pcan_basic.c \
#    $$PWD/src/thread_linux.c \
    $$PWD/src/thread_win.c \


HEADERS += \
    $$PWD/src/mrapi.h \
    $$PWD/src/can_driver.h \
    $$PWD/src/joint.h \
    $$PWD/src/logger.h \
    $$PWD/src/master.h \
    $$PWD/src/module.h \
    $$PWD/src/girpper.h \
    $$PWD/src/pcan_basic.h \

INCLUDEPATH += $$PWD/src

INCLUDEPATH += $$PWD/PCANBasic/Include
LIBS += -L$$PWD/PCANBasic/x64 -lPCANBasic
DEPENDPATH += -L$$PWD/PCANBasic/x64 -lPCANBasic
LIBS += -L$$PWD/PCANBasic/x64/VC_LIB -lPCANBasic

