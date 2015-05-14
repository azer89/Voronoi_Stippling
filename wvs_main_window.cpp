#include "wvs_main_window.h"
#include "ui_wvs_main_window.h"

wvs_main_window::wvs_main_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wvs_main_window)
{
    ui->setupUi(this);

    connect(ui->actionOpen,				SIGNAL(triggered()), this, SLOT(OpenFileAction()));

    ofTimer = new QTimer(this); connect(ofTimer, SIGNAL(timeout()), this, SLOT(OpenFileThread()));
}

wvs_main_window::~wvs_main_window()
{
    delete ui;
}


void wvs_main_window::OpenFileThread()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
    //ui.statusBar->showMessage("Opening image...", 100000);

    QString qFilename = QString::fromStdString(strFilename);

    // Display image
    //ui.frame->SetImage(qFilename);
    ui->widget->GetGLWidget()->SetImage(qFilename);

    std::cout << "Image loaded\n";

    QApplication::restoreOverrideCursor();

void CVUserInterface::OpenFileThread()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
    ui.statusBar->showMessage("Opening image...", 100000);

    // stop timer
    ofTimer->stop();
}
