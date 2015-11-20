
QT       += core

QT       -= gui

TARGET = untitled
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    gamemap.cpp \
    field.cpp \
    minimax.cpp

HEADERS += \
    gamemap.h \
    field.h \
    minimax.h
