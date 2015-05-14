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

    // stop timer
    ofTimer->stop();
}

void wvs_main_window::OpenFileAction()
{
    // Get Filename
    QString qFilename = QFileDialog::getOpenFileName(this, "");
    if(qFilename.isEmpty()) return;
    strFilename = qFilename.toStdString();

    //std::cout << strFilename << "\n";

    ofTimer->start(1);						// Use timer to enable cursor override
}
