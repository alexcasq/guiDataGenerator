#ifndef MASSIVEVISION_H
#define MASSIVEVISION_H

#include<QString>
#include<QStringList>
#include<QDir>
#include<QFile>
#include<QFileInfoList>
#include<QFileInfo>


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
    void executeMassive(QString pathIMages);


    /**
     * @brief unitarioExec
     * @param img
     */
    void unitarioExec(Mat img);


    QString pathOut;


    QStringList listActions;


};

#endif // MASSIVEVISION_H











