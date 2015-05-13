#include "wvs_main_window.h"
#include "ui_wvs_main_window.h"

wvs_main_window::wvs_main_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wvs_main_window)
{
    ui->setupUi(this);
}

wvs_main_window::~wvs_main_window()
{
    delete ui;
}


void CVUserInterface::OpenFileThread()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
    ui.statusBar->showMessage("Opening image...", 100000);

    // stop timer
    ofTimer->stop();
}

void CVUserInterface::OpenFileAction()
{
    // Get Filename
    QString qFilename = QFileDialog::getOpenFileName(this, "Open Image", "D:\\VG_Project\\_Data\\Data011");
    if(qFilename.isEmpty()) return;
    strFilename = qFilename.toStdString();
    SystemParams::str_filename = strFilename;

    ofTimer->start(1);						// Use timer to enable cursor override
}
