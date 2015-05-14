<<<<<<< HEAD
#-------------------------------------------------
#
# Project created by QtCreator 2015-05-13T15:33:48
#
#-------------------------------------------------

QT       += core gui opengl
=======
QT       += core gui
>>>>>>> 47eda7be3315b81d5ab46d3c230d0870a06bc503

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



