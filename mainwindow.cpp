#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileName.clear();

    ui->comboBox_functions->addItem(defGaussianBlur);
    ui->comboBox_functions->addItem(defBlur);
    ui->comboBox_functions->addItem(defMedianBlur);
    ui->comboBox_functions->addItem(defBilateralFilter);


    connect(ui->loadValues, &QAction::triggered, this, &MainWindow::toggleStatusbarLoad);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::toggleStatusbarExit);

}

MainWindow::~MainWindow()
{
    delete ui;
}




/**
 * @brief MainWindow::on_pb_loadImage_clicked
 */
void MainWindow::on_pb_loadImage_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));

    imgInput = imread(fileName.toStdString());

    if(!imgInput.empty())
    {
        namedWindow("Original Image", 0);
        imshow("Original Image", imgInput);
    }

} // end on_pb_loadImage_clicked

void MainWindow::on_pb_viewOriginalImage_clicked()
{
    if(!imgInput.empty())
    {
        namedWindow("Original Image", 0);
        imshow("Original Image", imgInput);
    }

} // end on_pb_viewOriginalImage_clicked


/**
 * @brief MainWindow::toggleStatusbarLoad
 * Lad file with config values
 */
void MainWindow::toggleStatusbarLoad()
{



}


/**
 * @brief MainWindow::toggleStatusbarExit
 */
void MainWindow::toggleStatusbarExit()
{
    destroyAllWindows();
    close();


} // end toggleStatusbarExit


void MainWindow::RunGB()
{
    int s1 = ui->spinBox_k1->value();
    int s2 = ui->spinBox_k2->value();

    kernelG = Size(s1, s2);

    sigmaX = ui->spinBox_sigx->value();
    sigmaY = ui->spinBox_sigy->value();

    if(!imgInput.empty()){

        GaussianBlur(imgInput, imgGB, kernelG, sigmaX, sigmaY, BORDER_DEFAULT);

        namedWindow("GaussianBlur",0);
        imshow("GaussianBlur", imgGB);
        waitKey(1);


    } // imgInput no vacia


} // end RunGB






/**
 * @brief MainWindow::on_spinBox_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k1_valueChanged(int arg1)
{
    if ( arg1 % 2 == 0)
    {
        ui->spinBox_k1->setValue(arg1+1);
    }
    RunGB();
}// end on_spinBox_valueChanged


/**
 * @brief MainWindow::on_spinBox_2_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k2_valueChanged(int arg1)
{
    if ( arg1 % 2 == 0)
    {
        ui->spinBox_k2->setValue(arg1+1);
    }
    RunGB();
} // end on_spinBox_2_valueChanged


/**
 * @brief MainWindow::on_spinBox_3_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_sigx_valueChanged(int arg1)
{
    RunGB();
} // end on_spinBox_3_valueChanged


/**
 * @brief MainWindow::on_spinBox_4_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_sigy_valueChanged(int arg1)
{
    RunGB();

} // end on_spinBox_4_valueChanged


void MainWindow::RunBlur()
{
    int s1 = ui->spinBox_Bk1->value();
    int s2 = ui->spinBox_Bk2->value();

    kernelB = Size(s1, s2);

    if(!imgInput.empty()){
        blur(imgInput, imgBlur, kernelB, Point(-1,-1) );

        namedWindow("Blur",0);
        imshow("Blur", imgBlur);
        waitKey(1);
    }

} // end RunBlur


/**
 * @brief MainWindow::on_spinBox_Bk1_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_Bk1_valueChanged(int arg1)
{
    RunBlur();


} // end on_spinBox_Bk1_valueChanged


/**
 * @brief MainWindow::on_spinBox_Bk2_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_Bk2_valueChanged(int arg1)
{
    RunBlur();

} // on_spinBox_Bk2_valueChanged








void MainWindow::on_comboBox_functions_currentIndexChanged(const QString &arg1)
{

    if(arg1 == defGaussianBlur)
    {
        ui->stackedWidget_values->setCurrentIndex(0);
        RunGB();
    }

    if(arg1 == defBlur)
    {
        ui->stackedWidget_values->setCurrentIndex(1);
        RunBlur();
    }


} // end on_comboBox_functions_currentIndexChanged
















