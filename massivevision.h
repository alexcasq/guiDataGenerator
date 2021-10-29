#ifndef MASSIVEVISION_H
#define MASSIVEVISION_H

#include<QString>
#include<QStringList>
#include<QDir>
#include<QFile>
#include<QFileInfoList>
#include<QFileInfo>
#include<QDate>
#include<QDateTime>
#include<QProgressBar>
#include<QLCDNumber>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


class massiveVision
{
public:
    massiveVision(QString pathSalida, QStringList listFunctions);

    /**
     * @brief executeMassive
     * @param pathIMages
     */
    void executeMassive(QString pathIMages, QProgressBar &pbar,
                        QLCDNumber &imgNum);


    /**
     * @brief unitarioExec
     * @param img
     */
    void unitarioExec(Mat img, QString filename, QProgressBar &pbar);


    QString pathOut;

    bool flagDuplicate;

    QStringList listActions;

    struct imgIdent {
       Mat imgS;
       QString indent;
    };


private:



    imgIdent processVision(QString functionYML, Mat imgInput);


};

#endif // MASSIVEVISION_H











