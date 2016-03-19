/********************************************************************************
** Form generated from reading UI file 'wvs_main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WVS_MAIN_WINDOW_H
#define UI_WVS_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "GLContainer.h"

QT_BEGIN_NAMESPACE

class Ui_wvs_main_window
{
public:
    QAction *actionOpen;
    QAction *actionSave_as_an_svg_file;
    QAction *actionSave_as_a_bitmap;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    GLContainer *widget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QFormLayout *formLayout;
    QLabel *label_5;
    QLabel *label_2;
    QSpinBox *stipplesSB;
    QLabel *label_3;
    QSpinBox *coneSlicesSB;
    QLabel *label_4;
    QSpinBox *coneDiameterSB;
    QLabel *label;
    QSpinBox *maxIterSB;
    QPushButton *updateParametersButton;

    void setupUi(QMainWindow *wvs_main_window)
    {
        if (wvs_main_window->objectName().isEmpty())
            wvs_main_window->setObjectName(QStringLiteral("wvs_main_window"));
        wvs_main_window->resize(1200, 800);
        wvs_main_window->setTabShape(QTabWidget::Triangular);
        actionOpen = new QAction(wvs_main_window);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave_as_an_svg_file = new QAction(wvs_main_window);
        actionSave_as_an_svg_file->setObjectName(QStringLiteral("actionSave_as_an_svg_file"));
        actionSave_as_a_bitmap = new QAction(wvs_main_window);
        actionSave_as_a_bitmap->setObjectName(QStringLiteral("actionSave_as_a_bitmap"));
        centralWidget = new QWidget(wvs_main_window);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new GLContainer(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));

        gridLayout->addWidget(widget, 0, 0, 1, 1);

        wvs_main_window->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(wvs_main_window);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        wvs_main_window->setMenuBar(menuBar);
        mainToolBar = new QToolBar(wvs_main_window);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        wvs_main_window->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(wvs_main_window);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        wvs_main_window->setStatusBar(statusBar);
        dockWidget = new QDockWidget(wvs_main_window);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(250, 407));
        dockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        formLayout = new QFormLayout(dockWidgetContents);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_5 = new QLabel(dockWidgetContents);
        label_5->setObjectName(QStringLiteral("label_5"));
        QFont font;
        font.setPointSize(14);
        label_5->setFont(font);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);

        label_2 = new QLabel(dockWidgetContents);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        stipplesSB = new QSpinBox(dockWidgetContents);
        stipplesSB->setObjectName(QStringLiteral("stipplesSB"));
        stipplesSB->setMinimum(1);
        stipplesSB->setMaximum(100000);
        stipplesSB->setValue(100);

        formLayout->setWidget(2, QFormLayout::FieldRole, stipplesSB);

        label_3 = new QLabel(dockWidgetContents);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_3);

        coneSlicesSB = new QSpinBox(dockWidgetContents);
        coneSlicesSB->setObjectName(QStringLiteral("coneSlicesSB"));
        coneSlicesSB->setMinimum(4);
        coneSlicesSB->setMaximum(128);
        coneSlicesSB->setValue(32);

        formLayout->setWidget(3, QFormLayout::FieldRole, coneSlicesSB);

        label_4 = new QLabel(dockWidgetContents);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_4);

        coneDiameterSB = new QSpinBox(dockWidgetContents);
        coneDiameterSB->setObjectName(QStringLiteral("coneDiameterSB"));
        coneDiameterSB->setMinimum(10);
        coneDiameterSB->setMaximum(1000);
        coneDiameterSB->setValue(100);

        formLayout->setWidget(4, QFormLayout::FieldRole, coneDiameterSB);

        label = new QLabel(dockWidgetContents);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label);

        maxIterSB = new QSpinBox(dockWidgetContents);
        maxIterSB->setObjectName(QStringLiteral("maxIterSB"));
        maxIterSB->setMaximum(100000);
        maxIterSB->setValue(50);

        formLayout->setWidget(5, QFormLayout::FieldRole, maxIterSB);

        updateParametersButton = new QPushButton(dockWidgetContents);
        updateParametersButton->setObjectName(QStringLiteral("updateParametersButton"));

        formLayout->setWidget(10, QFormLayout::LabelRole, updateParametersButton);

        dockWidget->setWidget(dockWidgetContents);
        wvs_main_window->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave_as_an_svg_file);
        menuFile->addAction(actionSave_as_a_bitmap);

        retranslateUi(wvs_main_window);

        QMetaObject::connectSlotsByName(wvs_main_window);
    } // setupUi

    void retranslateUi(QMainWindow *wvs_main_window)
    {
        wvs_main_window->setWindowTitle(QApplication::translate("wvs_main_window", "Weighted Voronoi Stippling", 0));
        actionOpen->setText(QApplication::translate("wvs_main_window", "Open", 0));
        actionSave_as_an_svg_file->setText(QApplication::translate("wvs_main_window", "Save as SVG", 0));
        actionSave_as_a_bitmap->setText(QApplication::translate("wvs_main_window", "Save as Bitmap", 0));
        menuFile->setTitle(QApplication::translate("wvs_main_window", "File", 0));
        label_5->setText(QApplication::translate("wvs_main_window", "Parameters", 0));
        label_2->setText(QApplication::translate("wvs_main_window", "Stipples", 0));
        label_3->setText(QApplication::translate("wvs_main_window", "Cone Slices", 0));
        label_4->setText(QApplication::translate("wvs_main_window", "Cone Diameter", 0));
        label->setText(QApplication::translate("wvs_main_window", "Max Iteration", 0));
        updateParametersButton->setText(QApplication::translate("wvs_main_window", "Update", 0));
    } // retranslateUi

};

namespace Ui {
    class wvs_main_window: public Ui_wvs_main_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WVS_MAIN_WINDOW_H
