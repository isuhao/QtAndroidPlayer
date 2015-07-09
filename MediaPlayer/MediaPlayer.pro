#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T16:35:21
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += androidextras
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD/ffmpeg/include
DEPENDPATH += $$PWD/ffmpeg/include

LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec-55
LIBS += -L$$PWD/ffmpeg/lib/ -lavutil-52
LIBS += -L$$PWD/ffmpeg/lib/ -lswscale-2
LIBS += -L$$PWD/ffmpeg/lib/ -lavformat-55
LIBS += -L$$PWD/ffmpeg/lib/ -lswresample-0
LIBS +=-Lpthread

TARGET = MediaPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        Task_Base/EventImpl.cpp \
        Task_Base/Task_Base.cpp \
        StreamBuf/StreamBuf.cpp \
        qdebug2logcat.cpp \
    mediaimpl.cpp \
    readstreamtask.cpp \
    videodecodetask.cpp \
    audiodecodeimpl.cpp \
    synchronousav.cpp \
    generator.cpp \
    audioplay.cpp \
    showdrawimage.cpp

HEADERS  += mainwindow.h\
        Task_Base/CriticalSection.h \
        Task_Base/EventImpl.h \
        Task_Base/Task_Base.h \
        StreamBuf/StreamBuf.h \
        libavinclude.h\
    mediaimpl.h \
    readstreamtask.h \
    videodecodetask.h \
    audiodecodeimpl.h \
    synchronousav.h \
    mediaimagehandler.h \
    generator.h \
    audioplay.h \
    audioplayhandler.h \
    synimagehandler.h \
    showdrawimage.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/ffmpeg/lib/libavcodec-55.so \
        $$PWD/ffmpeg/lib/libavformat-55.so \
        $$PWD/ffmpeg/lib/libavutil-52.so \
        $$PWD/ffmpeg/lib/librtmp.so \
        $$PWD/ffmpeg/lib/libswresample-0.so \
        $$PWD/ffmpeg/lib/libswscale-2.so
}

RESOURCES += \
    skin.qrc

