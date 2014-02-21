#-------------------------------------------------
#
# Project created by QtCreator 2014-02-13T01:25:51
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = Core
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    global.cpp \
    thread.cpp \
    listener.cpp \
    downloader.cpp

HEADERS += \
    global.h \
    thread.h \
    listener.h \
    downloader.h
