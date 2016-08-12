#include "wvs_main_window.h"
#include "ui_wvs_main_window.h"
#include "SystemParams.h"

/*
* Reza Adhitya Saputra
* reza.adhitya.saputra@gmail.com
*
*/

wvs_main_window::wvs_main_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wvs_main_window)
{
    ui->setupUi(this);



    connect(ui->actionOpen,	 SIGNAL(triggered()), this, SLOT(OpenFileAction()));
    ofTimer = new QTimer(this); connect(ofTimer, SIGNAL(timeout()), this, SLOT(OpenFileThread()));

    // actionSave_as_an_svg_file
    connect(ui->actionSave_as_an_svg_file,	 SIGNAL(triggered()), this, SLOT(SaveSVG()));

    // actionSave_as_a_bitmap
    connect(ui->actionSave_as_a_bitmap,	 SIGNAL(triggered()), this, SLOT(SaveBitmap()));

    /*
    stipplesSB
    coneSlicesSB
    coneDiameterSB
    maxIterSB
    */
    /*
    connect(ui->stipplesSB,     SIGNAL(valueChanged(int)),    this, SLOT(Recalculate()));
    connect(ui->coneSlicesSB,   SIGNAL(valueChanged(int)),    this, SLOT(Recalculate()));
    connect(ui->coneDiameterSB, SIGNAL(valueChanged(int)),    this, SLOT(Recalculate()));
    connect(ui->maxIterSB,      SIGNAL(valueChanged(int)),    this, SLOT(Recalculate()));
    */

    //updateParametersButton
    connect(ui->updateParametersButton,      SIGNAL(pressed()),    this, SLOT(Recalculate()));

	//LoadBitmapImage("C:/Users/azer/workspace/WVS/Input_Image/sanjuro.png");
}

wvs_main_window::~wvs_main_window()
{
    delete ui;
}

void wvs_main_window::SaveSVG()
{
    ui->widget->GetGLWidget()->SaveToSvg();
}

void wvs_main_window::SaveBitmap()
{
    ui->widget->GetGLWidget()->SaveToBitmap();
}

void wvs_main_window::OpenFileThread()
{

    if(!imageLoaded)
    {
        //QApplication::setOverrideCursor(Qt::WaitCursor);
        //QApplication::processEvents();
        //ui.statusBar->showMessage("Opening image...", 100000);

        QString qFilename = QString::fromStdString(strFilename);

        // set parameters
        SystemParams::num_stipples = ui->stipplesSB->value();
        SystemParams::cone_slices = ui->coneSlicesSB->value();
        SystemParams::cone_diameter = ui->coneDiameterSB->value();
        SystemParams::max_iter = ui->maxIterSB->value();

        // Display image
        ui->widget->GetGLWidget()->SetImage(qFilename);
        
		ui->widget->GetGLWidget()->OneStepLloydIteration();
		ui->widget->GetGLWidget()->repaint();

        ui->widget->SetScrolls();

        //std::cout << "Image processed\n";
        imageLoaded = true;

        ui->stipplesSB->setDisabled(true);
        ui->coneSlicesSB->setDisabled(true);
        ui->coneDiameterSB->setDisabled(true);
        ui->maxIterSB->setDisabled(true);
        ui->updateParametersButton->setDisabled(true);
        ui->actionOpen->setDisabled(true);
        ui->actionSave_as_an_svg_file->setDisabled(true);
        ui->actionSave_as_a_bitmap->setDisabled(true);

        //ofTimer->stop();

    }
    else
    {
        if(ui->widget->GetGLWidget()->IsCalculationDone())
        {
            //std::cout << "stop\n";

            ui->stipplesSB->setDisabled(false);
            ui->coneSlicesSB->setDisabled(false);
            ui->coneDiameterSB->setDisabled(false);
            ui->maxIterSB->setDisabled(false);
            ui->updateParametersButton->setDisabled(false);
            ui->actionOpen->setDisabled(false);
            ui->actionSave_as_an_svg_file->setDisabled(false);
            ui->actionSave_as_a_bitmap->setDisabled(false);

            ofTimer->stop();
            //QApplication::restoreOverrideCursor();
        }
        else
        {
            //std::cout << "OpenFileThread() repaint\n";
			ui->widget->GetGLWidget()->OneStepLloydIteration();
            ui->widget->GetGLWidget()->repaint();
        }
    }

    /*
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
    //ui.statusBar->showMessage("Opening image...", 100000);

    QString qFilename = QString::fromStdString(strFilename);

    // set parameters
    SystemParams::num_stipples = ui->stipplesSB->value();
    SystemParams::cone_slices = ui->coneSlicesSB->value();
    SystemParams::cone_diameter = ui->coneDiameterSB->value();
    SystemParams::max_iter = ui->maxIterSB->value();

    // Display image
    ui->widget->GetGLWidget()->SetImage(qFilename);
    ui->widget->GetGLWidget()->repaint();
    ui->widget->SetScrolls();

    std::cout << "Image processed\n";

    //ofTimer->stop();
    QApplication::restoreOverrideCursor();
    */
}

//void wvs_main_window::ProcessImage()
//{
//}

void wvs_main_window::OpenFileAction()
{
    // Get Filename
    QString qFilename = QFileDialog::getOpenFileName(this, "/home/azer/Desktop/");
    if(qFilename.isEmpty()) return;
    
	LoadBitmapImage(qFilename.toStdString());
	/*strFilename = qFilename.toStdString();

    imageLoaded = false;
    ofTimer->start(0); // Use timer to enable cursor override*/
}

void wvs_main_window::LoadBitmapImage(std::string strFileName)
{
	strFilename = strFileName;

	imageLoaded = false;
	ofTimer->start(0); // Use timer to enable cursor override
}

void wvs_main_window::Recalculate()
{
    std::cout << "recalculate\n";
    imageLoaded = false;
    ofTimer->start(0);
}
