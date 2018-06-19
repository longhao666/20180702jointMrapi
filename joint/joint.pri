

DEFINES += PCAN_BASIC

SOURCES += \
    $$PWD/src/joint.c \
    $$PWD/src/logger.c \
    $$PWD/src/master.c \
    $$PWD/src/module.c \
    $$PWD/src/gripper.c \
    $$PWD/src/pcan_basic.c \


HEADERS += \
    $$PWD/src/mrapi.h \
    $$PWD/src/can_driver.h \
    $$PWD/src/joint.h \
    $$PWD/src/logger.h \
    $$PWD/src/master.h \
    $$PWD/src/module.h \
    $$PWD/src/gripper.h \
    $$PWD/src/pcan_basic.h \

INCLUDEPATH += $$PWD/src


win32 {
DEFINES += _WINDOWS
SOURCES += \
    $$PWD/src/thread_win.c
INCLUDEPATH += $$PWD/winLib/Include
LIBS += -L$$PWD/winLib -lPCANBasic
}

# 使用Linux必须要保证 ls /dev里面有can设备，不然程序直接退出，不能初始化can
unix {
DEFINES += Linux
SOURCES += \
    $$PWD/src/thread_linux.c
INCLUDEPATH += $$PWD/LinuxLib/Include
LIBS += -L$$PWD/LinuxLib -lpcanbasic
}


