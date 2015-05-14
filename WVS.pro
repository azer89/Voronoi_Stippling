#-------------------------------------------------
#
# Project created by QtCreator 2015-05-13T15:33:48
#
#-------------------------------------------------

QT += core gui opengl

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



