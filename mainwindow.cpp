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
    ui->comboBox_functions->addItem(defConvolution);



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



//------------------------------------------------------------------------------
/**
 * @brief MainWindow::saveImage
 * @param imgSave
 */
void MainWindow::saveImage(Mat imgSave)
{
    if(!imgSave.empty())
    {

        QString filenameS;
        filenameS = QFileDialog::getSaveFileName(this, "Save file default jpg", "",
                                                "Images (*.png *.bmp *.jpg)");

        //cout<<"Filename save : " << filenameS.toStdString() << endl;

        QFileInfo info(filenameS);

        //cout<< info.completeSuffix().toStdString() << endl;

        QString extension = info.completeSuffix();
        if(extension.isEmpty())
        {
         string saveImage;
         saveImage = filenameS.toStdString() + ".jpg";
         imwrite(saveImage, imgSave);
        }

        if(extension == "jpg" || extension == "png" || extension == "bmp")
        {
         string saveImage;
         saveImage = filenameS.toStdString();
         imwrite(saveImage, imgSave);
        }
        else
        {
         string saveImage;
         saveImage = filenameS.toStdString() + ".jpg";
         imwrite(saveImage, imgSave);
        }

    } // input image not empty


} // end saveImage
//------------------------------------------------------------------------------



/**
 * @brief MainWindow::on_pb_viewOriginalImage_clicked
 */
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


//------------------------------------------------------------------------------
/**
 * @brief MainWindow::RunGB
 */
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
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * @brief MainWindow::RunBlur
 */
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
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/**
 * @brief MainWindow::on_comboBox_functions_currentIndexChanged
 * @param arg1
 */
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

    if(arg1 == defMedianBlur)
    {
        ui->stackedWidget_values->setCurrentIndex(2);
        RunMedianBlur();
    }

    if(arg1 == defBilateralFilter)
    {
        ui->stackedWidget_values->setCurrentIndex(3);
        RunBilateralFilter();
    }

    if(arg1 == defConvolution)
    {
        ui->stackedWidget_values->setCurrentIndex(4);
        //RunBilateralFilter();
    }



} // end on_comboBox_functions_currentIndexChanged
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
/**
 * @brief MainWindow::RunMedianBlur
 */
void MainWindow::RunMedianBlur()
{
    if(!imgInput.empty())
    {
        ksize = ui->spinBox_ksizeMB->value();
        medianBlur(imgInput, imgMB, ksize);
        namedWindow("MedianBlur",0);
        imshow("MedianBlur", imgMB);
        waitKey(1);
    }


} // end RunMedianBlur


/**
 * @brief MainWindow::on_spinBox_ksizeMB_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_ksizeMB_valueChanged(int arg1)
{
    if ( arg1 % 2 == 0)
    {
        ui->spinBox_ksizeMB->setValue(arg1+1);
    }

    RunMedianBlur();

} // end on_spinBox_ksizeMB_valueChanged
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/**
 * @brief MainWindow::RunBilateralFilter
 */
void MainWindow::RunBilateralFilter()
{

    if(!imgInput.empty())
    {
        dBF = ui->spinBox_BFd->value();
        sigmaColorBF = ui->doubleSpinBox_BFsigmaColor->value();
        sigmaSpaceBF = ui->doubleSpinBox_BFsigmaSpace->value();

        bilateralFilter(imgInput, imgBilFil, dBF, sigmaColorBF, sigmaSpaceBF);
        namedWindow("bilateralFilter",0);
        imshow("bilateralFilter", imgBilFil);
        waitKey(1);
    } // imgInput no vacia



} // end RunBilateralFilter

/**
 * @brief MainWindow::on_spinBox_BFd_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_BFd_valueChanged(int arg1)
{
    ui->spinBox_BFd->setDisabled(true);
    RunBilateralFilter();
    ui->spinBox_BFd->setDisabled(false);


} // end on_spinBox_BFd_valueChanged

/**
 * @brief MainWindow::on_doubleSpinBox_BFsigmaColor_valueChanged
 * @param arg1
 */
void MainWindow::on_doubleSpinBox_BFsigmaColor_valueChanged(double arg1)
{
    ui->doubleSpinBox_BFsigmaColor->setDisabled(true);
    RunBilateralFilter();
    ui->doubleSpinBox_BFsigmaColor->setDisabled(false);

} // end on_doubleSpinBox_BFsigmaColor_valueChanged

/**
 * @brief MainWindow::on_doubleSpinBox_BFsigmaSpace_valueChanged
 * @param arg1
 */
void MainWindow::on_doubleSpinBox_BFsigmaSpace_valueChanged(double arg1)
{
    ui->doubleSpinBox_BFsigmaSpace->setDisabled(true);
    RunBilateralFilter();
    ui->doubleSpinBox_BFsigmaSpace->setDisabled(false);

} // end on_doubleSpinBox_BFsigmaSpace_valueChanged
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

/**
 * @brief MainWindow::RunFilter2D
 */
void MainWindow::RunFilter2D()
{
    if(!imgInput.empty())
    {
        p00 = ui->spinBox_k00->value();
        p01 = ui->spinBox_k01->value();
        p02 = ui->spinBox_k02->value();
        p03 = ui->spinBox_k03->value();
        p04 = ui->spinBox_k04->value();

        p10 = ui->spinBox_k10->value();
        p11 = ui->spinBox_k11->value();
        p12 = ui->spinBox_k12->value();
        p13 = ui->spinBox_k13->value();
        p14 = ui->spinBox_k14->value();

        p20 = ui->spinBox_k20->value();
        p21 = ui->spinBox_k21->value();
        p22 = ui->spinBox_k22->value();
        p23 = ui->spinBox_k23->value();
        p24 = ui->spinBox_k24->value();

        p30 = ui->spinBox_k30->value();
        p31 = ui->spinBox_k31->value();
        p32 = ui->spinBox_k32->value();
        p33 = ui->spinBox_k33->value();
        p34 = ui->spinBox_k34->value();

        p40 = ui->spinBox_k40->value();
        p41 = ui->spinBox_k41->value();
        p42 = ui->spinBox_k42->value();
        p43 = ui->spinBox_k43->value();
        p44 = ui->spinBox_k44->value();

        Mat kernel = (Mat_<char>(5,5) << p00, p01,  p02,  p03,  p04,
                                         p10, p11,  p12,  p13,  p14,
                                         p20, p21,  p22,  p23,  p24,
                                         p30, p31,  p32,  p33,  p34,
                                         p40, p41,  p42,  p43,  p44);

        filter2D(imgInput, imgConv, -1, kernel, Point(-1,-1), 0, BORDER_DEFAULT);

        if(!imgConv.empty())
        {
            namedWindow("Convolution",0);
            imshow("Convolution", imgConv);
            waitKey(1);
        } // imgConv no vacia

    } // imgInput no vacia

} // end RunFilter2D

/**
 * @brief MainWindow::on_spinBox_k00_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k00_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k01_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k01_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k02_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k02_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k03_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k03_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k04_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k04_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k10_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k10_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k11_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k11_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k12_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k12_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k13_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k13_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k14_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k14_valueChanged(int arg1)
{
    RunFilter2D();
}


void MainWindow::on_spinBox_k20_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k11_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k21_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k12_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k22_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k13_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k23_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k14_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k24_valueChanged(int arg1)
{
    RunFilter2D();
}

void MainWindow::on_spinBox_k30_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k11_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k31_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k12_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k32_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k13_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k33_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k14_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k34_valueChanged(int arg1)
{
    RunFilter2D();
}


void MainWindow::on_spinBox_k40_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k11_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k41_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k12_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k42_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k13_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k43_valueChanged(int arg1)
{
    RunFilter2D();
}
/**
 * @brief MainWindow::on_spinBox_k14_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_k44_valueChanged(int arg1)
{
    RunFilter2D();
}


/**
 * @brief MainWindow::on_pb_saveImageConv_clicked
 */
void MainWindow::on_pb_saveImageConv_clicked()
{
   if(!imgConv.empty())
   {
        saveImage(imgConv);

   } // imgConv no vacia

} // end on_pb_saveImageConv_clicked

//------------------------------------------------------------------------------

















