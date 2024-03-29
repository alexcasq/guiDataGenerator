#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <qfiledialog.h>
#include <QInputDialog>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QMessageBox>


#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <massivevision.h>


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
    void saveImage(Mat imgSave, QString iden);


    QString fileName;

    // function in combobox
    QString defGaussianBlur      = "Gaussian Blur";
    QString defBlur              = "Blur";
    QString defMedianBlur        = "Median Blur";
    QString defBilateralFilter   = "Bilateral Filter";
    QString defConvolution       = "Apply Convolution";
    QString defColorConversion   = "Color Conversion";
    QString defDataAumentation   = "Data Aumentation";
    QString defDenoisingColored  = "Denoising Colored";
    QString defModifyChannels    = "Modify Channels";
    QString defBasicThresholding = "Basic Thresholding";
    QString defUtils             = "Utils";

    // Funtions in combobox color conversion
    QString defBGR2GRAY     = "BGR -> GRAY";
    QString defBGR2HSV      = "BGR -> HSV";
    QString defBGR2Luv      = "BGR -> Luv";
    QString defBGR2Lab      = "BGR -> Lab";
    QString defBGR2HLS      = "BGR -> HLS";
    QString defBGR2XYZ      = "BGR -> XYZ";
    QString defBGR2YCrCB    = "BGR -> YCrCB";

    // Funtions in combobox data aumentation
    QString defFlipXaxis     = "Flipping around X axis";
    QString defFlipYaxis     = "Flipping around Y axis";
    QString defFlipXYaxes     = "Flipping around both axes";

    Mat imgGray;
    Mat imgHSV;
    Mat imgLuv;
    Mat imgLab;
    Mat imgHLS;
    Mat imgXYZ;
    Mat imgYCrCB;


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
    Mat kernel;
    void RunFilter2D();
    //
    // VAR DENOISING COLORED
    void denoisingColored();
    Mat imgDenoisedColored;
    //
    // VAR MODIFY CHANNELS
    Mat imgMerge;
    QString defNormType_NORM_INF        = "NORM_INF";
    QString defNormType_NORM_L1         = "NORM_L1";
    QString defNormType_NORM_L2         = "NORM_L2";
    QString defNormType_NORM_MINMAX     = "NORM_MINMAX";
    //
    // VAR TH BASICS
    Mat imgTHb;
    QString def_thbType_THRESH_BINARY       = "THRESH_BINARY";
    QString def_thbType_THRESH_BINARY_INV   = "THRESH_BINARY_INV";
    QString def_thbType_THRESH_TRUNC        = "THRESH_TRUNC";
    QString def_thbType_THRESH_TOZERO       = "THRESH_TOZERO";
    QString def_thbType_THRESH_TOZERO_INV   = "THRESH_TOZERO_INV";
    QString def_thbType_THRESH_OTSU         = "THRESH_OTSU";
    QString def_thbType_THRESH_TRIANGLE     = "THRESH_TRIANGLE";
    void RunTH_b();


    QStringListModel *model;
    QStringList functionList;
    QString ymlConfig;


private slots:


    void on_pb_loadImage_clicked();

    void toggleStatusbarLoad();

    void toggleStatusbarExit();

    void ToolsCloseAllWindows();


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

    void on_comboBox_colorConv_currentIndexChanged(const QString &arg1);

    void on_comboBox_DA_currentIndexChanged(const QString &arg1);

    void on_doubleSpinBox_hLuminanceDC_valueChanged(double arg1);

    void on_doubleSpinBox_PhotoR_DC_valueChanged(double arg1);

    void on_spinBox_SearchW_DC_valueChanged(int arg1);

    void on_spinBox_blocSize_DC_valueChanged(int arg1);

    void on_pb_addMassive_clicked();

    void on_listView_addScript_doubleClicked(const QModelIndex &index);



    void on_pb_saveImageGB_clicked();

    void on_pb_saveImageBLUR_clicked();

    void on_pb_saveImageMEDIANBLUR_clicked();

    void on_pb_saveImageBILF_clicked();

    void on_pb_saveImageCC_clicked();

    void on_pb_saveImageDENCOL_clicked();

    void on_pb_saveConfigGB_clicked();

    void on_pb_saveConfigBLUR_clicked();

    void on_pb_saveConfigMEDIANBLUR_clicked();

    void on_pb_saveConfigBILF_clicked();

    void on_pb_saveConfigConv_clicked();

    void on_pb_saveConfigDENCOL_clicked();

    void on_pb_addFromFile_clicked();

    void on_pb_MassiveExe_clicked();

    void on_pb_oneFileExec_clicked();

    void on_hsb_R_valueChanged(int value);

    void on_hsb_G_valueChanged(int value);

    void on_hsb_B_valueChanged(int value);

    void on_pb_MC_merge_clicked();

    void on_pb_saveConfigMC_clicked();

    void on_comboBox_th_type_currentIndexChanged(const QString &arg1);

    void on_spinBox_th_value_valueChanged(int arg1);

    void on_pb_saveImage_thb_clicked();

    void on_pb_saveConfig_th_b_clicked();

    void on_pb_renameImages_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H












