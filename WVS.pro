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
            SystemParams.h

FORMS    += wvs_main_window.ui

LIBS += -lglut -lGL -lGLU

QMAKE_CXXFLAGS += -std=gnu++1y
