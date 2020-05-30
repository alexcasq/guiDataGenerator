#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileName.clear();

    //------------------------------------------------------------
    ui->comboBox_functions->addItem(defGaussianBlur);
    ui->comboBox_functions->addItem(defBlur);
    ui->comboBox_functions->addItem(defMedianBlur);
    ui->comboBox_functions->addItem(defBilateralFilter);
    ui->comboBox_functions->addItem(defConvolution);
    ui->comboBox_functions->addItem(defColorConversion);
    ui->comboBox_functions->addItem(defDataAumentation);
    ui->comboBox_functions->addItem(defDenoisingColored);
    ui->comboBox_functions->addItem(defModifyChannels);
    ui->comboBox_functions->addItem(defBasicThresholding);
    //------------------------------------------------------------

    ui->comboBox_colorConv->addItem("Press to select Convertion");
    ui->comboBox_colorConv->addItem(defBGR2HSV);
    ui->comboBox_colorConv->addItem(defBGR2GRAY);
    ui->comboBox_colorConv->addItem(defBGR2Luv);
    ui->comboBox_colorConv->addItem(defBGR2Lab);
    ui->comboBox_colorConv->addItem(defBGR2HLS);
    ui->comboBox_colorConv->addItem(defBGR2XYZ);
    ui->comboBox_colorConv->addItem(defBGR2YCrCB);

    ui->comboBox_DA->addItem("Press to select data aumentation");
    ui->comboBox_DA->addItem(defFlipXaxis);
    ui->comboBox_DA->addItem(defFlipYaxis);
    ui->comboBox_DA->addItem(defFlipXYaxes);

    ui->hsb_B->setStyleSheet("background-color: #69cbee;"
                             "alternate-background-color: #09EA09;");

    ui->hsb_G->setStyleSheet("background-color: #72edb5;"
                             "alternate-background-color: #09EA09;");

    ui->hsb_R->setStyleSheet("background-color: #f88989;"
                             "alternate-background-color: #09EA09;");

    ui->comboBox_MC_typeNorm->addItem(defNormType_NORM_INF);
    ui->comboBox_MC_typeNorm->addItem(defNormType_NORM_L1);
    ui->comboBox_MC_typeNorm->addItem(defNormType_NORM_L2);
    ui->comboBox_MC_typeNorm->addItem(defNormType_NORM_MINMAX);

    // Thresholding types
    ui->comboBox_th_type->addItem(def_thbType_THRESH_OTSU);
    ui->comboBox_th_type->addItem(def_thbType_THRESH_TRUNC);
    ui->comboBox_th_type->addItem(def_thbType_THRESH_BINARY);
    ui->comboBox_th_type->addItem(def_thbType_THRESH_TOZERO);
    ui->comboBox_th_type->addItem(def_thbType_THRESH_TRIANGLE);
    ui->comboBox_th_type->addItem(def_thbType_THRESH_BINARY_INV);
    ui->comboBox_th_type->addItem(def_thbType_THRESH_TOZERO_INV);


    // Create model
    model = new QStringListModel(this);
    ui->listView_addScript->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->label_dataAumentation->setStyleSheet("QLabel { background-color : red; color : white; }");

    connect(ui->loadValues, &QAction::triggered, this, &MainWindow::toggleStatusbarLoad);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::toggleStatusbarExit);
    connect(ui->actionClose_all_windows, &QAction::triggered, this, &MainWindow::ToolsCloseAllWindows);

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
void MainWindow::saveImage(Mat imgSave, QString iden)
{
    if(!imgSave.empty())
    {
        QString filenameS;

        if(iden == "")
        {
            filenameS = QFileDialog::getSaveFileName(this, "Save file default jpg", "",
                                                    "Images (*.png *.bmp *.jpg)");
        }

        if(iden != "")
        {
            QString suges;
            suges = iden + "  Images (*.png *.bmp *.jpg)";
            filenameS = QFileDialog::getSaveFileName(this, "Save file default jpg", iden,
                                                    suges);
        }

        ////cout<<"Filename save : " << filenameS.toStdString() << endl;

        QFileInfo info(filenameS);

        ////cout<< info.completeSuffix().toStdString() << endl;

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
    //pending development

    // load in dependence to identificator
    ymlConfig = QFileDialog::getOpenFileName(this,
        tr("Open config yml "), ".", tr("yml Files (*.yml)"));

    string identificator;

    if(!ymlConfig.isEmpty())
    {

        FileStorage fileConfig(ymlConfig.toStdString(), FileStorage::READ);
        fileConfig["identificator"] >> identificator;
        if(identificator.empty())
        {
            int ret = QMessageBox::critical(this, tr("My Application"),
                                           tr("erro in config file yml .\n"
                                              "The fields in yml file not are corrects."),
                                           QMessageBox::Ok);

        } // indetificator is incorrect

        if(identificator == "blur")
        {
            Size kernelB;
            fileConfig["kernelB"] >> kernelB;
            int s1, s2;
            s1 = kernelB.width;
            s2 = kernelB.height;

            ui->spinBox_Bk1->setValue(s1);
            ui->spinBox_Bk2->setValue(s2);


        } // identificator blur

        if(identificator == "GaussianBlur")
        {
            Size kernelG;
            double sigmaX;
            double sigmaY;

            fileConfig["kernelG"]   >> kernelG;
            fileConfig["sigmaX"]    >> sigmaX;
            fileConfig["sigmaY"]    >> sigmaY;

            int s1 = kernelG.width;
            int s2 = kernelG.height;
            ui->spinBox_k1->setValue(s1);
            ui->spinBox_k2->setValue(s2);

            ui->spinBox_sigx->setValue(sigmaX);
            ui->spinBox_sigy->setValue(sigmaY);


        } // identificator GaussianBlur


        if(identificator == "medianBlur")
        {
            int ksize;

            fileConfig["ksize"]   >> ksize;
            ui->spinBox_ksizeMB->setValue(ksize);

        } // identificator medianBlur


        if(identificator == "bilateralFilter")
        {
            int dBF;
            double sigmaColorBF;
            double sigmaSpaceBF;

            fileConfig["dBF"]   >> dBF;
            fileConfig["sigmaColorBF"]   >> sigmaColorBF;
            fileConfig["sigmaSpaceBF"]   >> sigmaSpaceBF;

            ui->spinBox_BFd->setValue(dBF);
            ui->doubleSpinBox_BFsigmaColor->setValue(sigmaColorBF);
            ui->doubleSpinBox_BFsigmaSpace->setValue(sigmaSpaceBF);


        } // identificator bilateralFilter

        if(identificator == "convolution")
        {
            //Mat kernel;
            // PENDING DEVELOPMENT

            fileConfig["kernel"]   >> kernel;


        } // identificator convolution


        if(identificator == "DenoisingColored")
        {
            float h_luminance;
            float h_color;
            int search_window;
            int block_size;

            fileConfig["h_luminance"]   >> h_luminance;
            fileConfig["h_color"]       >> h_color;
            fileConfig["search_window"] >> search_window;
            fileConfig["block_size"]    >> block_size;


            ui->doubleSpinBox_hLuminanceDC->setValue(h_luminance);
            ui->doubleSpinBox_PhotoR_DC->setValue(h_color);
            ui->spinBox_SearchW_DC->setValue(search_window);
            ui->spinBox_blocSize_DC->setValue(block_size);


        } // identificator DenoisingColored


        fileConfig.release();


    } //ymlConfig not empty

} // end toggleStatusbarLoad load files config values


/**
 * @brief MainWindow::toggleStatusbarExit
 */
void MainWindow::toggleStatusbarExit()
{
    destroyAllWindows();
    close();
} // end toggleStatusbarExit


/**
 * @brief MainWindow::ToolsCloseAllWindows
 */
void MainWindow::ToolsCloseAllWindows()
{
    destroyAllWindows();

} // end ToolsCloseAllWindows
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
    }

    if(arg1 == defColorConversion)
    {
        ui->stackedWidget_values->setCurrentIndex(5);
    }

    if(arg1 == defDataAumentation)
    {
        ui->stackedWidget_values->setCurrentIndex(6);
    }

    if(arg1 == defDenoisingColored)
    {
        ui->stackedWidget_values->setCurrentIndex(7);
    }

    if(arg1 == defModifyChannels)
    {
        ui->stackedWidget_values->setCurrentIndex(8);
    }

    if(arg1 == defBasicThresholding)
    {
        ui->stackedWidget_values->setCurrentIndex(9);
    }


} // end on_comboBox_functions_currentIndexChanged
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// GAUSSINA BLUR FUNCTION
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


/**
 * @brief MainWindow::on_pb_saveImageGB_clicked
 */
void MainWindow::on_pb_saveImageGB_clicked()
{
    if(!imgGB.empty())
    {
        saveImage(imgGB, "GaussianBlur");
    } // imgGB no empty

} // end on_pb_saveImageGB_clicked

/**
 * @brief MainWindow::on_pb_saveConfigGB_clicked
 */
void MainWindow::on_pb_saveConfigGB_clicked()
{
    QString fileYml;
    fileYml = QFileDialog::getSaveFileName(this,
                                           "Save file default yml",
                                           "GaussianBlurValuesConfig",
                                           "config (*.yml)");
    string nameYML;
    nameYML= fileYml.toStdString();

    QFileInfo info(fileYml);

    ////cout<< info.completeSuffix().toStdString() << endl;

    QString extension = info.completeSuffix();
    if(extension.isEmpty() || extension != "yml")
    {
         nameYML = fileYml.toStdString() + ".yml";
    }

    //--
    int s1 = ui->spinBox_k1->value();
    int s2 = ui->spinBox_k2->value();
    kernelG = Size(s1, s2);
    sigmaX = ui->spinBox_sigx->value();
    sigmaY = ui->spinBox_sigy->value();
    //--

    FileStorage fsave(nameYML, FileStorage::WRITE);
    fsave << "identificator" << "GaussianBlur";
    fsave << "kernelG" << kernelG;
    fsave << "sigmaX" << sigmaX;
    fsave << "sigmaY" << sigmaY;
    fsave.release();


} // end on_pb_saveConfigGB_clicked


//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// BLUR FUNCTION
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

/**
 * @brief MainWindow::on_pb_saveImageBLUR_clicked
 */
void MainWindow::on_pb_saveImageBLUR_clicked()
{
    if(!imgBlur.empty())
    {
        saveImage(imgBlur, "Blur");
    } // imgBlur no empty

} // endl on_pb_saveImageBLUR_clicked

/**
 * @brief MainWindow::on_pb_saveConfigBLUR_clicked
 */
void MainWindow::on_pb_saveConfigBLUR_clicked()
{
    QString fileYml;
    fileYml = QFileDialog::getSaveFileName(this,
                                           "Save file default yml",
                                           "BlurValuesConfig",
                                           "config (*.yml)");
    string nameYML;
    nameYML= fileYml.toStdString();

    QFileInfo info(fileYml);

    ////cout<< info.completeSuffix().toStdString() << endl;

    QString extension = info.completeSuffix();
    if(extension.isEmpty() || extension != "yml")
    {
         nameYML = fileYml.toStdString() + ".yml";
    }

    //--
    int s1 = ui->spinBox_Bk1->value();
    int s2 = ui->spinBox_Bk2->value();
    kernelB = Size(s1, s2);
    //--

    FileStorage fsave(nameYML, FileStorage::WRITE);
    fsave << "identificator" << "blur";
    fsave << "kernelB" << kernelB;
    fsave.release();
} // end on_pb_saveConfigBLUR_clicked

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// MEDIAN BLUR FUNCTION
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

/**
 * @brief MainWindow::on_pb_saveImageMEDIANBLUR_clicked
 */
void MainWindow::on_pb_saveImageMEDIANBLUR_clicked()
{

    if(!imgMB.empty())
    {
        saveImage(imgMB, "MedianBlur");
    } // imagen median blur not empty

} // end on_pb_saveImageMEDIANBLUR_clicked

/**
 * @brief MainWindow::on_pb_saveConfigMEDIANBLUR_clicked
 */
void MainWindow::on_pb_saveConfigMEDIANBLUR_clicked()
{
    QString fileYml;
    fileYml = QFileDialog::getSaveFileName(this,
                                           "Save file default yml",
                                           "MedianBlurValuesConfig",
                                           "config (*.yml)");
    string nameYML;
    nameYML= fileYml.toStdString();

    QFileInfo info(fileYml);

    ////cout<< info.completeSuffix().toStdString() << endl;

    QString extension = info.completeSuffix();
    if(extension.isEmpty() || extension != "yml")
    {
         nameYML = fileYml.toStdString() + ".yml";
    }

    //--
    ksize = ui->spinBox_ksizeMB->value();
    //--

    FileStorage fsave(nameYML, FileStorage::WRITE);
    fsave << "identificator" << "medianBlur";
    fsave << "ksize" << ksize;
    fsave.release();

} // end on_pb_saveConfigMEDIANBLUR_clicked

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// BILATERAL FILTER FUNCTION
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

/**
 * @brief MainWindow::on_pb_saveImageBILF_clicked
 */
void MainWindow::on_pb_saveImageBILF_clicked()
{
    if(!imgBilFil.empty())
    saveImage(imgBilFil, "BilateralFilter");
} // end on_pb_saveImageBILF_clicked


/**
 * @brief MainWindow::on_pb_saveConfigBILF_clicked
 */
void MainWindow::on_pb_saveConfigBILF_clicked()
{

    QString fileYml;
    fileYml = QFileDialog::getSaveFileName(this,
                                           "Save file default yml",
                                           "BilateralFilterValuesConfig",
                                           "config (*.yml)");
    string nameYML;
    nameYML= fileYml.toStdString();

    QFileInfo info(fileYml);

    ////cout<< info.completeSuffix().toStdString() << endl;

    QString extension = info.completeSuffix();
    if(extension.isEmpty() || extension != "yml")
    {
         nameYML = fileYml.toStdString() + ".yml";
    }

    //--
    dBF = ui->spinBox_BFd->value();
    sigmaColorBF = ui->doubleSpinBox_BFsigmaColor->value();
    sigmaSpaceBF = ui->doubleSpinBox_BFsigmaSpace->value();
    //--

    FileStorage fsave(nameYML, FileStorage::WRITE);
    fsave << "identificator" << "bilateralFilter";
    fsave << "dBF" << dBF;
    fsave << "sigmaColorBF" << sigmaColorBF;
    fsave << "sigmaSpaceBF" << sigmaSpaceBF;
    fsave.release();

} //end on_pb_saveConfigBILF_clicked
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// FILTER2D CONVOLUTIONS FUNCTION
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

        kernel = (Mat_<char>(5,5) << p00, p01,  p02,  p03,  p04,
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
        saveImage(imgConv, "Convolution");

   } // imgConv no vacia

} // end on_pb_saveImageConv_clicked

/**
 * @brief MainWindow::on_pb_saveConfigConv_clicked
 */
void MainWindow::on_pb_saveConfigConv_clicked()
{
    QString fileYml;
    fileYml = QFileDialog::getSaveFileName(this,
                                           "Save file default yml",
                                           "ConvolutionValuesConfig",
                                           "config (*.yml)");
    string nameYML;
    nameYML= fileYml.toStdString();

    QFileInfo info(fileYml);

    ////cout<< info.completeSuffix().toStdString() << endl;

    QString extension = info.completeSuffix();
    if(extension.isEmpty() || extension != "yml")
    {
         nameYML = fileYml.toStdString() + ".yml";
    }

    //--
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

    kernel = (Mat_<char>(5,5) << p00, p01,  p02,  p03,  p04,
                                     p10, p11,  p12,  p13,  p14,
                                     p20, p21,  p22,  p23,  p24,
                                     p30, p31,  p32,  p33,  p34,
                                     p40, p41,  p42,  p43,  p44);
    //--

    FileStorage fsave(nameYML, FileStorage::WRITE);
    fsave << "identificator" << "convolution";
    fsave << "kernel" << kernel;
    fsave.release();
} // end on_pb_saveConfigConv_clicked
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// COLOR CONVERTIONS FUNCTION
//------------------------------------------------------------------------------
/**
 * @brief MainWindow::on_comboBox_colorConv_currentIndexChanged
 * @param arg1
 */
void MainWindow::on_comboBox_colorConv_currentIndexChanged(const QString &arg1)
{
    if(!imgInput.empty() && imgInput.channels() == 3)
    {
        if(arg1 == defBGR2GRAY)
        {
            ui->pb_saveImageCC->setText("Save Image BGR2GRAY");
            cvtColor(imgInput, imgGray, COLOR_BGR2GRAY);
            namedWindow("Gray", 0);
            imshow("Gray", imgGray);
            waitKey(1);
        } // convert BGR TO GRAY
        if(arg1 == defBGR2HSV)
        {
            ui->pb_saveImageCC->setText("Save Image BGR2HSV");
            cvtColor(imgInput, imgHSV, COLOR_BGR2HSV);
            namedWindow("HSV", 0);
            imshow("HSV", imgHSV);
            waitKey(1);
        } // convert BGR TO HSV
        if(arg1 == defBGR2Luv)
        {
            ui->pb_saveImageCC->setText("Save Image BGR2Luv");
            cvtColor(imgInput, imgLuv, COLOR_BGR2Luv);
            namedWindow("Luv", 0);
            imshow("Luv", imgLuv);
            waitKey(1);
        } // convert BGR TO LUV
        if(arg1 == defBGR2Lab)
        {
            ui->pb_saveImageCC->setText("Save Image BGR2Lab");
            cvtColor(imgInput, imgLab, COLOR_BGR2Lab);
            namedWindow("Lab", 0);
            imshow("Lab", imgLab);
            waitKey(1);
        } // convert BGR TO LAB
        if(arg1 == defBGR2HLS)
        {
            ui->pb_saveImageCC->setText("Save Image BGR2HLS");
            cvtColor(imgInput, imgHLS, COLOR_BGR2HLS);
            namedWindow("HLS", 0);
            imshow("HLS", imgHLS);
            waitKey(1);
        } // convert BGR TO HLS
        if(arg1 == defBGR2XYZ)
        {
            ui->pb_saveImageCC->setText("Save Image BGR2XYZ");
            cvtColor(imgInput, imgXYZ, COLOR_BGR2XYZ);
            namedWindow("XYZ", 0);
            imshow("XYZ", imgXYZ);
            waitKey(1);
        } // convert BGR TO XYZ
        if(arg1 == defBGR2YCrCB)
        {
            ui->pb_saveImageCC->setText("Save Image BGR2YCrCb");
            cvtColor(imgInput, imgYCrCB, COLOR_BGR2YCrCb);
            namedWindow("YCrCB", 0);
            imshow("YCrCB", imgYCrCB);
            waitKey(1);
        } // convert BGR TO XYZ

    } // imgInput not empty and is image with 3 channels

} // end on_comboBox_colorConv_currentIndexChanged


/**
 * @brief MainWindow::on_pb_saveImageCC_clicked
 */
void MainWindow::on_pb_saveImageCC_clicked()
{
    if(!imgXYZ.empty())
    {
        saveImage(imgXYZ, "XYC");
        imgXYZ.release();
    }
    if(!imgGray.empty())
    {
        saveImage(imgGray, "Gray");
        imgGray.release();
    }
    if(!imgHSV.empty())
    {
        saveImage(imgHSV, "HSV");
        imgHSV.release();
    }
    if(!imgLuv.empty())
    {
        saveImage(imgLuv, "Luv");
        imgLuv.release();
    }
    if(!imgLab.empty())
    {
        saveImage(imgLab, "Lab");
        imgLab.release();
    }
    if(!imgHLS.empty())
    {
        saveImage(imgHLS, "HLS");
        imgHLS.release();
    }
    if(!imgYCrCB.empty())
    {
        saveImage(imgYCrCB, "YCrCB");
        imgYCrCB.release();
    }

} // end on_pb_saveImageCC_clicked

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// DATA AUMENTATION  FUNCTION
//------------------------------------------------------------------------------

/**
 * @brief MainWindow::on_comboBox_DA_currentIndexChanged
 * @param arg1
 */
void MainWindow::on_comboBox_DA_currentIndexChanged(const QString &arg1)
{
    if(!imgInput.empty())
    {
        Mat flipImage;
        string nameWindow;
        nameWindow ="";
        if(arg1 == defFlipXaxis)
        {
            flip(imgInput, flipImage, 0);
            nameWindow = "Flip around X";
        } // flip x

        if(arg1 == defFlipYaxis)
        {
            flip(imgInput, flipImage, 1);
            nameWindow = "Flip around Y";
        } // flip x

        if(arg1 == defFlipXYaxes)
        {
            flip(imgInput, flipImage, -1);
            nameWindow = "Flip around XY";

        } // flip x

        if(!flipImage.empty())
        {
            namedWindow(nameWindow, 0);
            imshow(nameWindow,flipImage);
            waitKey(1);
        } // flipImage not empty

    } // imgInput not empty



} // end on_comboBox_DA_currentIndexChanged

/**
 * @brief MainWindow::on_pb_addMassive_clicked
 */
void MainWindow::on_pb_addMassive_clicked()
{
    QString FunctionName;
    FunctionName = ui->comboBox_DA->currentText();

    if(FunctionName != "Press to select data aumentation")
    {
        if(!functionList.contains(FunctionName) )
        {

            functionList.append(FunctionName);

        }

        model->setStringList(functionList);
        ui->listView_addScript->setModel(model);

    } // diferent to slect value


} // end on_pb_addMassive_clicked


/**
 * @brief MainWindow::on_listView_addScript_doubleClicked
 * @param index
 */
void MainWindow::on_listView_addScript_doubleClicked(const QModelIndex &index)
{
    ////cout<<index.row()<<endl;

    QString itemText = index.data(Qt::DisplayRole).toString();

    ////cout<<itemText.toStdString()<<endl;

    functionList.removeAt(index.row());

    model->setStringList(functionList);

    ui->listView_addScript->setModel(model);

} // end on_listView_addScript_doubleClicked


/**
 * @brief MainWindow::on_pb_addFromFile_clicked
 */
void MainWindow::on_pb_addFromFile_clicked()
{

    ymlConfig = QFileDialog::getOpenFileName(this,
        tr("Open config yml "), ".", tr("yml Files (*.yml)"));

    if(!ymlConfig.isEmpty())
    {

        FileStorage fileConfig(ymlConfig.toStdString(), FileStorage::READ);
        string indetificator = fileConfig["identificator"];
        if(indetificator.empty())
        {
            int ret = QMessageBox::critical(this, tr("My Application"),
                                           tr("erro in config file yml .\n"
                                              "The fields in yml file not are corrects."),
                                           QMessageBox::Ok);

        } // indetificator is incorrect

        fileConfig.release();

        functionList.append(ymlConfig);

        model->setStringList(functionList);

        ui->listView_addScript->setModel(model);

        //on_pb_addMassive_clicked();

    } // ymlConfig exist

} // endl


/**
 * @brief MainWindow::on_pb_MassiveExe_clicked
 */
void MainWindow::on_pb_MassiveExe_clicked()
{
    int numerFunc;
    numerFunc = functionList.size();
    //cout<<"numerFunc:: "<< numerFunc << endl;

    QString dirImages = QFileDialog::getExistingDirectory(this, tr("Open Directory with images"),
                                                ".",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);


    QString outProcess = QFileDialog::getExistingDirectory(this, tr("Select Out Directory "),
                                                ".",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);



    //cout<<"dirImages:  " << dirImages.toStdString() << endl;
    //cout<<"OUTdIR   :  " << outProcess.toStdString() << endl;


    //-------------------------------------------------------------------------
    // class massiveVission process
    //-------------------------------------------------------------------------
    massiveVision msv(outProcess, functionList);
    msv.executeMassive(dirImages, *ui->progressBar_DA, *ui->lcdNumber_imagesP);
    waitKey(1);
    //-------------------------------------------------------------------------


} // end on_pb_MassiveExe_clicked


/**
 * @brief MainWindow::on_pb_oneFileExec_clicked
 */
void MainWindow::on_pb_oneFileExec_clicked()
{
    on_pb_loadImage_clicked();

    if(!imgInput.empty() && !fileName.isEmpty())
    {
        QString outProcess = QFileDialog::getExistingDirectory(this, tr("Select Out Directory "),
                                                    ".",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

        //---------------------------------------------------------
        // class massiveVission process unit image
        //---------------------------------------------------------
        massiveVision msv(outProcess, functionList);
        msv.unitarioExec(imgInput, fileName, *ui->progressBar_DA);
        //---------------------------------------------------------

    } // imgInput not empty

} // end on_pb_oneFileExec_clicked



//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// DENOISING COLORED FUNCTION
//------------------------------------------------------------------------------
/**
 * @brief MainWindow::denoisingColored
 */
void MainWindow::denoisingColored()
{
    if(!imgInput.empty())
    {
        ui->doubleSpinBox_PhotoR_DC->setDisabled(true);
        ui->doubleSpinBox_hLuminanceDC->setDisabled(true);
        ui->spinBox_SearchW_DC->setDisabled(true);
        ui->spinBox_blocSize_DC->setDisabled(true);

        float h_luminance    = 0.0;
        float h_color       = 0.0;
        int search_window   = 0;
        int block_size      = 0;

        h_luminance     = (float)ui->doubleSpinBox_hLuminanceDC->value();
        h_color         = (float)ui->doubleSpinBox_PhotoR_DC->value();
        search_window   = ui->spinBox_SearchW_DC->value();
        block_size      = ui->spinBox_blocSize_DC->value();

//        src Input 8-bit 3-channel image.
//        dst Output image with the same size and type as src .
//        templateWindowSize Size in pixels of the template patch that is used to compute weights.
//         Should be odd. Recommended value 7 pixels
//        searchWindowSize Size in pixels of the window that is used to compute weighted average for
//         given pixel. Should be odd. Affect performance linearly: greater searchWindowsSize - greater
//         denoising time. Recommended value 21 pixels
//        h Parameter regulating filter strength for luminance component. Bigger h value perfectly
//         removes noise but also removes image details, smaller h value preserves details but also preserves
//         some noise
//        hColor The same as h but for color components. For most images value equals 10
//         will be enough to remove colored noise and do not distort colors

        fastNlMeansDenoisingColored(imgInput, imgDenoisedColored,
                                    h_luminance, h_color,
                                    block_size, search_window);

        namedWindow("DenoisingColored", 0);
        imshow("DenoisingColored", imgDenoisedColored);
        waitKey(1);

        ui->doubleSpinBox_PhotoR_DC->setDisabled(false);
        ui->doubleSpinBox_hLuminanceDC->setDisabled(false);
        ui->spinBox_SearchW_DC->setDisabled(false);
        ui->spinBox_blocSize_DC->setDisabled(false);

    } // imgINput not empty

} // end denoisingColored


/**
 * @brief MainWindow::on_doubleSpinBox_hLuminanceDC_valueChanged
 * @param arg1
 */
void MainWindow::on_doubleSpinBox_hLuminanceDC_valueChanged(double arg1)
{
    denoisingColored();

} // end on_doubleSpinBox_hLuminanceDC_valueChanged

/**
 * @brief MainWindow::on_doubleSpinBox_PhotoR_DC_valueChanged
 * @param arg1
 */
void MainWindow::on_doubleSpinBox_PhotoR_DC_valueChanged(double arg1)
{
    denoisingColored();
} // end on_doubleSpinBox_PhotoR_DC_valueChanged

/**
 * @brief MainWindow::on_spinBox_SearchW_DC_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_SearchW_DC_valueChanged(int arg1)
{
    denoisingColored();
} // end on_spinBox_SearchW_DC_valueChanged

/**
 * @brief MainWindow::on_spinBox_blocSize_DC_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_blocSize_DC_valueChanged(int arg1)
{
    denoisingColored();
} // end on_spinBox_blocSize_DC_valueChanged



void MainWindow::on_pb_saveImageDENCOL_clicked()
{
    if(!imgDenoisedColored.empty())
    {
        saveImage(imgDenoisedColored, "DenoisingColored");

    } // imgDenoisedColored not empty

}// end on_pb_saveImageDENCOL_clicked

/**
 * @brief MainWindow::on_pb_saveConfigDENCOL_clicked
 */
void MainWindow::on_pb_saveConfigDENCOL_clicked()
{

    QString fileYml;
    fileYml = QFileDialog::getSaveFileName(this,
                                           "Save file default yml",
                                           "denoisingColoredValuesConfig",
                                           "config (*.yml)");
    string nameYML;
    nameYML= fileYml.toStdString();

    QFileInfo info(fileYml);

    ////cout<< info.completeSuffix().toStdString() << endl;

    QString extension = info.completeSuffix();
    if(extension.isEmpty() || extension != "yml")
    {
         nameYML = fileYml.toStdString() + ".yml";
    }

    //--

    float h_luminance    = 0.0;
    float h_color       = 0.0;
    int search_window   = 0;
    int block_size      = 0;

    h_luminance     = (float)ui->doubleSpinBox_hLuminanceDC->value();
    h_color         = (float)ui->doubleSpinBox_PhotoR_DC->value();
    search_window   = ui->spinBox_SearchW_DC->value();
    block_size      = ui->spinBox_blocSize_DC->value();
    //--

    FileStorage fsave(nameYML, FileStorage::WRITE);
    fsave << "identificator" << "DenoisingColored";
    fsave << "h_luminance" << h_luminance;
    fsave << "h_color" << h_color;
    fsave << "search_window" << search_window;
    fsave << "block_size" << block_size;
    fsave.release();

} // end on_pb_saveConfigDENCOL_clicked

//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
// MODIFY VALUE COLOR
//------------------------------------------------------------------------------
/**
 * @brief MainWindow::on_hsb_R_valueChanged
 * @param value
 */
void MainWindow::on_hsb_R_valueChanged(int value)
{
    if(!imgInput.empty())
    {
        Mat rgbchannel[3];
        // The actual splitting.
        split(imgInput, rgbchannel);
        //Mat red;
        //red = rgbchannel[0];
        Mat result;
        Mat R = Mat::ones(imgInput.rows, imgInput.cols, CV_8UC1);
        //result = (red/value);
        result = R * value;
        vector<Mat> channels; //BGR
        channels.push_back(rgbchannel[0]);
        channels.push_back(rgbchannel[1]);
        channels.push_back(result);


        /// Merge the three channels
        Mat imgMergeR;
        merge(channels, imgMergeR);

        namedWindow("Change Channels R",0);
        imshow("Change Channels R", imgMergeR);

    } // imgInput not empty


} // end on_hsb_R_valueChanged


/**
 * @brief MainWindow::on_hsb_G_valueChanged
 * @param value
 */
void MainWindow::on_hsb_G_valueChanged(int value)
{
    if(!imgInput.empty())
    {
        Mat rgbchannel[3];
        // The actual splitting.
        split(imgInput, rgbchannel);
        //Mat red;
        //red = rgbchannel[0];
        Mat result;
        Mat G = Mat::ones(imgInput.rows, imgInput.cols, CV_8UC1);
        //result = (red/value);
        result = G * value;
        vector<Mat> channels;  //BGR
        channels.push_back(rgbchannel[0]);
        channels.push_back(result);
        channels.push_back(rgbchannel[2]);

        /// Merge the three channels
        Mat imgMergeG;
        merge(channels, imgMergeG);

        namedWindow("Change Channels G",0);
        imshow("Change Channels G", imgMergeG);

    } // imgInput not empty


} // end on_hsb_G_valueChanged

/**
 * @brief MainWindow::on_hsb_B_valueChanged
 * @param value
 */
void MainWindow::on_hsb_B_valueChanged(int value)
{
    if(!imgInput.empty())
    {
        Mat rgbchannel[3];
        // The actual splitting.
        split(imgInput, rgbchannel);
        //Mat red;
        //red = rgbchannel[0];
        Mat result;
        Mat B = Mat::ones(imgInput.rows, imgInput.cols, CV_8UC1);
        //result = (red/value);
        result = B * value;
        vector<Mat> channels;  //BGR
        channels.push_back(result);
        channels.push_back(rgbchannel[1]);
        channels.push_back(rgbchannel[2]);

        Mat imgMergeB;
        /// Merge the three channels
        merge(channels, imgMergeB);

        namedWindow("Change Channels B",0);
        imshow("Change Channels B", imgMergeB);

    } // imgInput not empty


} // end on_hsb_B_valueChanged


/**
 * @brief MainWindow::on_pb_MC_merge_clicked
 */
void MainWindow::on_pb_MC_merge_clicked()
{
    if(!imgInput.empty())
    {
        int Bval, Rval, Gval;

        Bval = ui->hsb_B->value();
        Rval = ui->hsb_R->value();
        Gval = ui->hsb_G->value();

//        cout<<"------------------------------" << endl;
//        cout<<"Bval:   " << Bval << endl;
//        cout<<"Rval:   " << Rval << endl;
//        cout<<"Gval:   " << Gval << endl;
//        cout<<"------------------------------" << endl;

        Mat rgbchannel[3];
        split(imgInput, rgbchannel);

        Mat resultB;
        Mat B = Mat::ones(imgInput.rows, imgInput.cols, CV_8UC1);
        resultB = rgbchannel[0] / Bval;

        Mat resultG;
        Mat G = Mat::ones(imgInput.rows, imgInput.cols, CV_8UC1);
        resultG = rgbchannel[1] / Gval;

        Mat resultR;
        Mat R = Mat::ones(imgInput.rows, imgInput.cols, CV_8UC1);
        resultR = rgbchannel[2] / Rval;

        vector<Mat> channels;  //BGR
        channels.push_back(resultB);
        channels.push_back(resultG);
        channels.push_back(resultR);

        /// Merge the three channels
        merge(channels, imgMerge);

        namedWindow("Change Channels BGR",0);
        imshow("Change Channels BGR", imgMerge);


        QString typeNorm;
        int _alfa, _beta;
        _alfa = ui->doubleSpinBox_MD_alfa->value();
        _beta = ui->doubleSpinBox_MC_beta->value();
        typeNorm = ui->comboBox_MC_typeNorm->currentText();
        cv::NormTypes TYPE;
        if(typeNorm == defNormType_NORM_INF)
        {
            TYPE = cv::NORM_INF;
        }
        if(typeNorm == defNormType_NORM_L1)
        {
            TYPE = cv::NORM_L1;
        }
        if(typeNorm == defNormType_NORM_L2)
        {
            TYPE = cv::NORM_L2;
        }

        if(typeNorm == defNormType_NORM_MINMAX)
        {
            TYPE = cv::NORM_MINMAX;
        }

        Mat normalizeImMerge;
        normalize(imgMerge, normalizeImMerge, _alfa, _beta, TYPE, -1, noArray());
        namedWindow("normalizeImMerge",0);
        imshow("normalizeImMerge", normalizeImMerge);

        Mat normalizeIm;
        normalize(imgInput, normalizeIm, _alfa, _beta, TYPE, -1, noArray());
        namedWindow("normalizeIm",0);
        imshow("normalizeIm", normalizeIm);




    } // imgInput not empty

} //  end on_pb_MC_merge_clicked


/**
 * @brief MainWindow::on_pb_saveConfigMC_clicked
 */
void MainWindow::on_pb_saveConfigMC_clicked()
{
    QString fileYml;
    fileYml = QFileDialog::getSaveFileName(this,
                                           "Save file default yml",
                                           "colorModifyValuesConfig",
                                           "config (*.yml)");
    string nameYML;
    nameYML= fileYml.toStdString();

    QFileInfo info(fileYml);

    ////cout<< info.completeSuffix().toStdString() << endl;

    QString extension = info.completeSuffix();
    if(extension.isEmpty() || extension != "yml")
    {
         nameYML = fileYml.toStdString() + ".yml";
    }

    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    QString typeNorm;
    typeNorm.clear();
    int _alfa, _beta;
    _alfa = ui->doubleSpinBox_MD_alfa->value();
    _beta = ui->doubleSpinBox_MC_beta->value();
    typeNorm = ui->comboBox_MC_typeNorm->currentText();
    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    FileStorage fsave(nameYML, FileStorage::WRITE);
    fsave << "identificator" << "normalize";
    fsave << "_alfa" << _alfa;
    fsave << "_beta" << _beta;
    fsave << "typeNorm" << typeNorm.toStdString();
    fsave.release();

} // end on_pb_saveConfigMC_clicked

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// THRESHOLD BASIC
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 * @brief MainWindow::RunTH_b
 */
void MainWindow::RunTH_b()
{
    if(!imgInput.empty())
    {

        int thValue;
        thValue = ui->spinBox_th_value->value();

        QString QtypeTH;
        QtypeTH.clear();
        ThresholdTypes typeTH;
        typeTH = THRESH_BINARY;
        QtypeTH = ui->comboBox_th_type->currentText();

        if(QtypeTH == "THRESH_BINARY")
        {
            typeTH = THRESH_BINARY;
        }

        if(QtypeTH == "THRESH_BINARY_INV")
        {
            typeTH = THRESH_BINARY_INV;
        }

        if(QtypeTH == "THRESH_TRUNC")
        {
            typeTH = THRESH_TRUNC;
        }

        if(QtypeTH == "THRESH_TOZERO")
        {
            typeTH = THRESH_TOZERO;
        }

        if(QtypeTH == "THRESH_TOZERO_INV")
        {
            typeTH = THRESH_TOZERO_INV;
        }


        if(QtypeTH == "THRESH_OTSU")
        {
            typeTH = THRESH_OTSU;
        }

        if(QtypeTH == "THRESH_TRIANGLE")
        {
            typeTH = THRESH_TRIANGLE;
        }

        Mat grayImg;
        cvtColor(imgInput, grayImg, COLOR_BGR2GRAY);
        threshold(grayImg, imgTHb, thValue, 255, typeTH);

        namedWindow("thresHoldBasic", 0);
        imshow("thresHoldBasic", imgTHb);

    } // imgInput not empty


} // end RunTH_b


/**
 * @brief MainWindow::on_comboBox_th_type_currentIndexChanged
 * @param arg1
 */
void MainWindow::on_comboBox_th_type_currentIndexChanged(const QString &arg1)
{
    RunTH_b();

} // on_comboBox_th_type_currentIndexChanged

/**
 * @brief MainWindow::on_spinBox_th_value_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_th_value_valueChanged(int arg1)
{
    RunTH_b();

} // end on_spinBox_th_value_valueChanged


/**
 * @brief MainWindow::on_pb_saveImage_thb_clicked
 */
void MainWindow::on_pb_saveImage_thb_clicked()
{
    if(!imgTHb.empty())
    {
        saveImage(imgTHb, "thresHoldBasic");

    } // imgDenoisedColored not empty

}


/**
 * @brief MainWindow::on_pb_saveConfig_th_b_clicked
 */
void MainWindow::on_pb_saveConfig_th_b_clicked()
{
    QString fileYml;
    fileYml = QFileDialog::getSaveFileName(this,
                                           "Save file default yml",
                                           "basic_thresholding",
                                           "config (*.yml)");
    string nameYML;
    nameYML= fileYml.toStdString();

    QFileInfo info(fileYml);

    ////cout<< info.completeSuffix().toStdString() << endl;

    QString extension = info.completeSuffix();
    if(extension.isEmpty() || extension != "yml")
    {
         nameYML = fileYml.toStdString() + ".yml";
    }

    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    int thValue;
    thValue = ui->spinBox_th_value->value();
    QString QtypeTH;
    QtypeTH.clear();
    ThresholdTypes typeTH;
    typeTH = THRESH_BINARY;
    QtypeTH = ui->comboBox_th_type->currentText();


    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    FileStorage fsave(nameYML, FileStorage::WRITE);
    fsave << "identificator" << "threshold";
    fsave << "thValue" << thValue;
    fsave << "typeTH" << QtypeTH.toStdString();
    fsave.release();

} // on_pb_saveConfig_th_b_clicked
//------------------------------------------------------------------------------



































