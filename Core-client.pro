#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T14:37:16
#
#-------------------------------------------------

QT       += core gui network sql

INCLUDEPATH += C:/OpenSSL-Win32-Full/include
LIBS += -L"C:/OpenSSL-Win32-Full/lib" -llibeay32 -lssleay32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Core-client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aesbox.cpp \
    listener.cpp \
    rsabox.cpp \
    global.cpp \
    client.cpp \
    http.cpp \
    database.cpp \
    uploader.cpp \
    window.cpp \
    daemon.cpp \
    handler.cpp \
    job.cpp \
    jobsmanager.cpp \
    systray.cpp \
    dialog.cpp \
    jobinfo.cpp

HEADERS  += mainwindow.h \
    aesbox.h \
    listener.h \
    rsabox.h \
    global.h \
    client.h \
    http.h \
    database.h \
    uploader.h \
    window.h \
    daemon.h \
    handler.h \
    job.h \
    jobsmanager.h \
    systray.h \
    dialog.h \
    jobinfo.h

FORMS    += mainwindow.ui \
    window.ui \
    dialog.ui \
    jobinfo.ui

OTHER_FILES += \
    icon.png \
    https.txt

RESOURCES += \
    resources.qrc
