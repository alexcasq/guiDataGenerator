#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <qfiledialog.h>



#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int MAX_KERNEL_LENGTH = 31;

    Mat imgInput;


    QString fileName;

    // function in combobox
    QString defGaussianBlur     = "Gaussian Blur";
    QString defBlur             = "Blur";
    QString defMedianBlur       = "Median Blur";
    QString defBilateralFilter  = "Bilateral Filter";


    // VAR TO GAUSSIAN BLUR
    Mat imgGB;
    Size kernelG;
    double sigmaX;
    double sigmaY;
    void RunGB();
    //
    // VAR TO BLU
    Mat imgBlur;
    Size kernelB;
    void RunBlur();



private slots:
    void on_pb_loadImage_clicked();

    void toggleStatusbarLoad();

    void toggleStatusbarExit();


    void on_pb_viewOriginalImage_clicked();

    void on_spinBox_k1_valueChanged(int arg1);

    void on_spinBox_k2_valueChanged(int arg1);

    void on_spinBox_sigx_valueChanged(int arg1);

    void on_spinBox_sigy_valueChanged(int arg1);



    void on_spinBox_Bk1_valueChanged(int arg1);

    void on_spinBox_Bk2_valueChanged(int arg1);

    void on_comboBox_functions_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
