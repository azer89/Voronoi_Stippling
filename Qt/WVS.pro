#-------------------------------------------------
#
# Project created by QtCreator 2015-05-14T11:08:33
#
#-------------------------------------------------

QT       += core gui opengl svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WVS
TEMPLATE = app


SOURCES += main.cpp                 \
           wvs_main_window.cpp      \
           GLWidget.cpp             \
           GLContainer.cpp          \
           stdafx.cpp               \
           RejectionSampling.cpp    \
           SystemParams.cpp

HEADERS  += wvs_main_window.h       \
            GLContainer.h           \
            MyPoint.h               \
            stdafx.h                \
            GLWidget.h              \
            RejectionSampling.h     \
            SystemParams.h \
    MyLine.h \
    MyIndexedLine.h

FORMS    += wvs_main_window.ui

LIBS += -lglut -lGL -lGLU

INCLUDEPATH +=   /usr/include/
LIBS        += -L/usr/include/
LIBS        += -lCGAL
LIBS        += -lgmp
LIBS        += -lmpfr
QMAKE_CXXFLAGS += -frounding-math -O3

QMAKE_CXXFLAGS += -std=gnu++1y
