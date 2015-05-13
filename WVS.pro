#-------------------------------------------------
#
# Project created by QtCreator 2015-05-13T12:22:31
#
#-------------------------------------------------


QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WVS
TEMPLATE = app


SOURCES += main.cpp\
        wvs_main_window.cpp \
    GLWidget.cpp \
    GLContainer.cpp \
    stdafx.cpp

HEADERS  += wvs_main_window.h \
    GLContainer.h \
    MyPoint.h \
    stdafx.h \
    GLWidget.h

FORMS    += wvs_main_window.ui

INCLUDEPATH += /home/azer/Qt5.3.2/5.3/gcc_64/include/QtOpenGL

LIBS += -lglut -lGL -lGLU

CONFIG += console opengl thread precompile_header

PRECOMPILED_HEADER = stdafx.h
