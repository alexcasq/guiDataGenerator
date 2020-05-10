#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <qfiledialog.h>
#include <QInputDialog>



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

    /**
     * @brief saveImage
     * @param imgSave
     */
    void saveImage(Mat imgSave);



    QString fileName;

    // function in combobox
    QString defGaussianBlur     = "Gaussian Blur";
    QString defBlur             = "Blur";
    QString defMedianBlur       = "Median Blur";
    QString defBilateralFilter  = "Bilateral Filter";
    QString defConvolution      = "Apply Convolution";


    // VAR TO GAUSSIAN BLUR
    Mat imgGB;
    Size kernelG;
    double sigmaX;
    double sigmaY;
    void RunGB();
    //
    // VAR TO BLUR
    Mat imgBlur;
    Size kernelB;
    void RunBlur();
    //
    // VAR TO MEDIAN BLUR
    Mat imgMB;
    int ksize;
    void RunMedianBlur();
    //
    //VAR TO BILATERAL FILTER
    // d – Diameter of each pixel neighborhood that is used during filtering.
    //     If it is non-positive, it is computed from sigmaSpace .
    // sigmaColor – Filter sigma in the color space. A larger value of the
    //    parameter means that farther colors within the pixel neighborhood
    //    (see sigmaSpace ) will be mixed together, resulting in larger areas
    //    of semi-equal color.
    // sigmaSpace – Filter sigma in the coordinate space. A larger value of the
    //    parameter means that farther pixels will influence each other as long
    //    as their colors are close enough (see sigmaColor ). When d>0 , it
    //    specifies the neighborhood size regardless of sigmaSpace. Otherwise,
    //    d is proportional to sigmaSpace.
    // Reduce ruido manteniendo bordes nitidos
    Mat imgBilFil;
    int dBF;
    double sigmaColorBF;
    double sigmaSpaceBF;
    void RunBilateralFilter();
    //
    // VAR FILTER2D to convolutions
    Mat imgConv;
    int p00, p01, p02, p03, p04;
    int p10, p11, p12, p13, p14;
    int p20, p21, p22, p23, p24;
    int p30, p31, p32, p33, p34;
    int p40, p41, p42, p43, p44;
    void RunFilter2D();






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

    void on_spinBox_ksizeMB_valueChanged(int arg1);

    void on_spinBox_BFd_valueChanged(int arg1);

    void on_doubleSpinBox_BFsigmaColor_valueChanged(double arg1);

    void on_doubleSpinBox_BFsigmaSpace_valueChanged(double arg1);


    void on_spinBox_k00_valueChanged(int arg1);

    void on_spinBox_k01_valueChanged(int arg1);

    void on_spinBox_k02_valueChanged(int arg1);

    void on_spinBox_k03_valueChanged(int arg1);

    void on_spinBox_k04_valueChanged(int arg1);

    void on_spinBox_k10_valueChanged(int arg1);

    void on_spinBox_k11_valueChanged(int arg1);

    void on_spinBox_k12_valueChanged(int arg1);

    void on_spinBox_k13_valueChanged(int arg1);

    void on_spinBox_k14_valueChanged(int arg1);

    void on_spinBox_k20_valueChanged(int arg1);

    void on_spinBox_k21_valueChanged(int arg1);

    void on_spinBox_k22_valueChanged(int arg1);

    void on_spinBox_k23_valueChanged(int arg1);

    void on_spinBox_k24_valueChanged(int arg1);

    void on_spinBox_k30_valueChanged(int arg1);

    void on_spinBox_k31_valueChanged(int arg1);

    void on_spinBox_k32_valueChanged(int arg1);

    void on_spinBox_k33_valueChanged(int arg1);

    void on_spinBox_k34_valueChanged(int arg1);

    void on_spinBox_k40_valueChanged(int arg1);

    void on_spinBox_k41_valueChanged(int arg1);

    void on_spinBox_k42_valueChanged(int arg1);

    void on_spinBox_k43_valueChanged(int arg1);

    void on_spinBox_k44_valueChanged(int arg1);



    void on_pb_saveImageConv_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H












