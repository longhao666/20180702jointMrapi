
path1 = src
path2 = include
path3 = drivers/pcan_basic
path4 = lib/PCANBasic

#message($$path1);
#message($$(path1) + 0);

DEFINES += PCAN_BASIC

SOURCES += \
    $$PWD/$$path1/joint.c \
    $$PWD/$$path1/logger.c \
    $$PWD/$$path1/master.c \
    $$PWD/$$path1/module.c \
    $$PWD/$$path1/gripper.c \

HEADERS += \
    $$PWD/$$path2/mrapi.h \
    $$PWD/$$path2/can_driver.h \
    $$PWD/$$path2/joint.h \
    $$PWD/$$path2/logger.h \
    $$PWD/$$path2/master.h \
    $$PWD/$$path2/module.h \
    $$PWD/$$path2/gripper.h \

SOURCES += \
    $$PWD/$$path3/pcan_basic.c \

HEADERS += \
    $$PWD/$$path3/pcan_basic.h \


INCLUDEPATH += $$PWD/$$path2
INCLUDEPATH += $$PWD/$$path3

win32 {
    DEFINES += _WINDOWS
    SOURCES += \
        $$PWD/$$path3/thread_win.c
    INCLUDEPATH += $$PWD/$$path4/Include
    LIBS += -L$$PWD/$$path4/x64 -lPCANBasic
    LIBS += -L$$PWD/$$path4/x64/VC_LIB -lPCANBasic
    DEPENDPATH += $$PWD/$$path4/x64
}

# 使用Linux必须要保证 ls /dev里面有can设备，不然程序直接退出，不能初始化can
unix {
    DEFINES += Linux
    SOURCES += \
        $$PWD/drivers/pcan_basic/thread_linux.c
    INCLUDEPATH += /usr/include/PCANBasic.h
    LIBS += -L/usr/lib -lpcanbasic
}
