#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T14:37:16
#
#-------------------------------------------------

QT       += core gui network

INCLUDEPATH += C:/OpenSSL-Win32-Full/include C:/Python27/include
LIBS += -L"C:/Python27/libs" -L"C:/OpenSSL-Win32-Full/lib" -llibeay32 -lssleay32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Core-client
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    aesbox.cpp \
    downloader.cpp \
    listener.cpp \
    rsabox.cpp \
    thread.cpp \
    daemon.cpp \
    global.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    aesbox.h \
    downloader.h \
    listener.h \
    rsabox.h \
    thread.h \
    daemon.h \
    global.h \
    client.h \
    main.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    icon.png
