/********************************************************************************
** Form generated from reading UI file 'wvs_main_window.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WVS_MAIN_WINDOW_H
#define UI_WVS_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "GLContainer.h"

QT_BEGIN_NAMESPACE

class Ui_wvs_main_window
{
public:
    QAction *actionOpenFile;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    GLContainer *widget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *wvs_main_window)
    {
        if (wvs_main_window->objectName().isEmpty())
            wvs_main_window->setObjectName(QString::fromUtf8("wvs_main_window"));
        wvs_main_window->resize(1024, 768);
        actionOpenFile = new QAction(wvs_main_window);
        actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
        centralWidget = new QWidget(wvs_main_window);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget = new GLContainer(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));

        gridLayout->addWidget(widget, 0, 0, 1, 1);

        wvs_main_window->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(wvs_main_window);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 25));
        wvs_main_window->setMenuBar(menuBar);
        mainToolBar = new QToolBar(wvs_main_window);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        wvs_main_window->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(wvs_main_window);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        wvs_main_window->setStatusBar(statusBar);

        mainToolBar->addAction(actionOpenFile);

        retranslateUi(wvs_main_window);

        QMetaObject::connectSlotsByName(wvs_main_window);
    } // setupUi

    void retranslateUi(QMainWindow *wvs_main_window)
    {
        wvs_main_window->setWindowTitle(QApplication::translate("wvs_main_window", "wvs_main_window", 0, QApplication::UnicodeUTF8));
        actionOpenFile->setText(QApplication::translate("wvs_main_window", "Open File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class wvs_main_window: public Ui_wvs_main_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WVS_MAIN_WINDOW_H
