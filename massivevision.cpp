#include "massivevision.h"

massiveVision::massiveVision(QString pathSalida, QStringList listFunctions)
{
    if(!pathSalida.isEmpty() && !listFunctions.empty())
    {
        pathOut         = pathSalida;
        listActions     = listFunctions;
    }


} // end massiveVision



/**
 * @brief massiveVision::processVision
 * @param functionYML
 * @param imgInput
 * @return
 */
massiveVision::imgIdent massiveVision::processVision(QString functionYML, Mat imgInput)
{
    struct imgIdent out;

    Mat outProcess;

    // verify is yml exist
    if(QFile::exists(functionYML))
    {
        //read yml
        FileStorage fsRead(functionYML.toStdString(), FileStorage::READ);
        string identificator;

        fsRead["identificator"] >> identificator;

        //cout<<"Indetificador:    " << identificator << endl;

        if(identificator == "blur")
        {
            Size kernelB;

            fsRead["kernelB"] >> kernelB;

            blur(imgInput, outProcess, kernelB, Point(-1,-1));

            outProcess.copyTo(out.imgS);

            out.indent = "blur";

        } // identificator blur

        if(identificator == "GaussianBlur")
        {
            Size kernelG;
            double sigmaX;
            double sigmaY;

            fsRead["kernelG"]   >> kernelG;
            fsRead["sigmaX"]    >> sigmaX;
            fsRead["sigmaY"]    >> sigmaY;

            GaussianBlur(imgInput, outProcess,
                         kernelG, sigmaX, sigmaY, BORDER_DEFAULT);

            outProcess.copyTo(out.imgS);

            out.indent = "GaussianBlur";

        } // identificator GaussianBlur


        if(identificator == "medianBlur")
        {
            int ksize;

            fsRead["ksize"]   >> ksize;

            medianBlur(imgInput, outProcess, ksize);

            outProcess.copyTo(out.imgS);

            out.indent = "medianBlur";

        } // identificator medianBlur


        if(identificator == "bilateralFilter")
        {
            int dBF;
            double sigmaColorBF;
            double sigmaSpaceBF;

            fsRead["dBF"]   >> dBF;
            fsRead["sigmaColorBF"]   >> sigmaColorBF;
            fsRead["sigmaSpaceBF"]   >> sigmaSpaceBF;

            bilateralFilter(imgInput, outProcess, dBF, sigmaColorBF, sigmaSpaceBF);

            outProcess.copyTo(out.imgS);

            out.indent = "bilateralFilter";

        } // identificator bilateralFilter

        if(identificator == "convolution")
        {
            Mat kernel;

            fsRead["kernel"]   >> kernel;

            filter2D(imgInput, outProcess, -1, kernel, Point(-1,-1), 0, BORDER_DEFAULT);

            outProcess.copyTo(out.imgS);

            out.indent = "convolution";

        } // identificator convolution


        if(identificator == "DenoisingColored")
        {
            float h_luminance;
            float h_color;
            int search_window;
            int block_size;

            fsRead["h_luminance"]   >> h_luminance;
            fsRead["h_color"]       >> h_color;
            fsRead["search_window"] >> search_window;
            fsRead["block_size"]    >> block_size;

            fastNlMeansDenoisingColored(imgInput, outProcess,
                                        h_luminance, h_color,
                                        block_size, search_window);


            outProcess.copyTo(out.imgS);

            out.indent = "DenoisingColored";

        } // identificator DenoisingColored

        if(identificator == "normalize")
        {
            string type;
            int _alfa, _beta;
            fsRead["_alfa"]     >> _alfa;
            fsRead["_beta"]     >> _beta;
            fsRead["typeNorm"]  >> type;

            cv::NormTypes TYPE;
            if(type == "NORM_INF")
            {
                TYPE = cv::NORM_INF;
            }
            if(type == "NORM_L1")
            {
                TYPE = cv::NORM_L1;
            }
            if(type == "NORM_L2")
            {
                TYPE = cv::NORM_L2;
            }

            if(type == "NORM_MINMAX")
            {
                TYPE = cv::NORM_MINMAX;
            }

            normalize(imgInput, outProcess, _alfa, _beta,
                      TYPE, -1, noArray());

            outProcess.copyTo(out.imgS);

            out.indent = "normalize";

        } // normalize

        if(identificator == "threshold")
        {
            string type;
            int thValue;
            fsRead["thValue"]   >> thValue;
            fsRead["typeTH"]    >> type;

            cv::ThresholdTypes typeTH;

            if(type == "THRESH_OTSU")
            {
                typeTH = cv::THRESH_OTSU;
            }

            if(type == "THRESH_BINARY")
            {
                typeTH = cv::THRESH_BINARY;
            }

            if(type == "THRESH_BINARY_INV")
            {
                typeTH = cv::THRESH_BINARY_INV;
            }

            if(type == "THRESH_TRUNC")
            {
                typeTH = cv::THRESH_TRUNC;
            }

            if(type == "THRESH_TOZERO")
            {
                typeTH = cv::THRESH_TOZERO;
            }

            if(type == "THRESH_TOZERO_INV")
            {
                typeTH = cv::THRESH_TOZERO_INV;
            }

            if(type == "THRESH_OTSU")
            {
                typeTH = cv::THRESH_OTSU;
            }

            if(type == "THRESH_TRIANGLE")
            {
                typeTH = cv::THRESH_TRIANGLE;
            }

            Mat grayImg;
            cvtColor(imgInput, grayImg, COLOR_BGR2GRAY);
            threshold(grayImg, outProcess, thValue, 255, typeTH);

            outProcess.copyTo(out.imgS);

            out.indent = "threshold";

        } // threshold

        fsRead.release();

    } // file exist

    return out;

} // end processVision


/**
 * @brief massiveVision::executeMassive
 * @param pathIMages
 * @param pbar
 * @param imgNum
 */
void massiveVision::executeMassive(QString pathIMages, QProgressBar &pbar,
                                   QLCDNumber &imgNum)
{
    if(!pathIMages.isEmpty())
    {
        QFileInfoList listFiles;
        QDir dir(pathIMages);
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.bmp" <<"*.jpeg";
        listFiles = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);

        //cout<<"Numer the files to process : " << listFiles.size() << endl;

        //cout<<"Numer of actiosn : " << listActions.size() << endl;
        Mat imageStatus(80, 250, CV_8UC3, Scalar(0, 0, 0));
        string prStatus;
        prStatus = "Status_process";
        putText(imageStatus, prStatus, Point2f(10,17), FONT_HERSHEY_PLAIN, 0.77,  Scalar(0,0,255,255));
        namedWindow("Status_process",1);

        int contActions;
        QString action;
        Mat imgInput;
        QString nameOut;
        pbar.setRange(0, listActions.size());
        imgNum.display("0");
        int numImgProcessed;
        numImgProcessed = 0;

        nameOut.clear();
        QString format = "dd_MM_yyyy_hh_mm_ss_zz";
        QDateTime date = QDateTime::currentDateTime();
        QString dateString = date.toString(format);
        //cout<<dateString.toStdString()<<endl;
        QString folderOut;
        folderOut.clear();
        folderOut = pathOut + "/" + dateString;
        if(!QDir(folderOut).exists())
        {
            QDir().mkdir(folderOut);

        } // pathOut not exist created


        int actionNum;
        actionNum = 0;
        for(contActions = 0; contActions < listActions.size(); ++contActions)
        {
            actionNum = actionNum + 1;
            action.clear();
            action = listActions[contActions];
            //cout<<"-------------------------------------" << endl;
            //cout<<"Actions -------- :  " << action.toStdString() << endl;

            pbar.setValue(contActions);

            prStatus = "Run_actions";
            putText(imageStatus, prStatus, Point2f(10,29),
                    FONT_HERSHEY_PLAIN, 0.75,  Scalar(0,0,255,255));
            imshow("Status_process", imageStatus);
            char key = waitKey(1);
            if(key == 'q' || key == 'Q')
            {
                destroyAllWindows();
                break;
            }


            foreach (const QFileInfo &fileinfo, listFiles)
            {
                numImgProcessed = numImgProcessed +1;
                QString lcdVal = QString::number(numImgProcessed);
                imgNum.display(lcdVal);

                QString imageFile = fileinfo.absoluteFilePath();
                //cout<<"imageFile:  " << imageFile.toStdString() << endl;

                imgInput = imread(imageFile.toStdString());

                if(!imgInput.empty())
                {
                    QString rootName = fileinfo.completeBaseName() + "." +
                            fileinfo.completeSuffix();

                    if(action == "Flipping around X axis")
                    {
                        Mat imgFlipX;
                        flip(imgInput, imgFlipX, 0);
                        nameOut.clear();
                        nameOut = folderOut + "/FlipX_" + rootName;
                        imwrite(nameOut.toStdString(), imgFlipX);
                    } // flip x

                    if(action == "Flipping around Y axis")
                    {
                        Mat imgFlipY;
                        flip(imgInput, imgFlipY, 1);
                        nameOut.clear();
                        nameOut = folderOut + "/FlipY_" + rootName;
                        imwrite(nameOut.toStdString(), imgFlipY);
                    } // flip y

                    if(action == "Flipping around both axes")
                    {
                        Mat imgFlipXY;
                        flip(imgInput, imgFlipXY, -1);
                        nameOut.clear();
                        nameOut = folderOut + "/FlipXY_" + rootName;
                        imwrite(nameOut.toStdString(), imgFlipXY);
                    } // flip xy

                    if(action.contains(".yml"))
                    {
                        struct imgIdent outPV;

                        outPV = processVision(action, imgInput);

                        if(!outPV.imgS.empty())
                        {
                            QString valAction = QString::number(actionNum);
                            nameOut = folderOut + "/" + valAction + "_"
                                    + outPV.indent + "_" + rootName;
                            //cout<<"Name out:  " << nameOut.toStdString() << endl;
                            imwrite(nameOut.toStdString(), outPV.imgS);

                            if(flagDuplicate == true)
                            {
                                QString rename;
                                rename = pathIMages + "/" + valAction + "_"
                                                + outPV.indent + "_" + rootName;
                                imwrite(rename.toStdString(), imgInput);

                            }// flag duplicate true


                            prStatus = "Process image";
                            putText(imageStatus, prStatus, Point2f(10,41),
                                    FONT_HERSHEY_PLAIN, 0.75,  Scalar(0,0,255,255));
                            imshow("Status_process", imageStatus);
                            char key = waitKey(1);
                            if(key == 'q' || key == 'Q')
                            {
                                destroyAllWindows();
                                break;
                            }

                        } // imgYML not empty

                    } // ymnl process


                } // imgInput not empty

            } // list images

        } // for contActions

        pbar.setValue(contActions);


    } // pathIMages not empty

    destroyAllWindows();


} // end executeMassive


/**
 * @brief massiveVision::unitarioExec
 * @param imgInput
 * @param filename
 * @param pbar
 */
void massiveVision::unitarioExec(Mat imgInput, QString filename, QProgressBar &pbar)
{
    int contActions;
    QString action;
    QString nameOut;
    nameOut.clear();

    QString format = "dd_MM_yyyy_hh_mm_ss_zz";
    QDateTime date = QDateTime::currentDateTime();
    QString dateString = date.toString(format);
    //cout<<dateString.toStdString()<<endl;
    QString folderOut;
    folderOut.clear();
    folderOut = pathOut + "/" + dateString;
    if(!QDir(folderOut).exists())
    {
        QDir().mkdir(folderOut);

    } // pathOut not exist created

    pbar.setRange(0, listActions.size());

    Mat imageStatus(80, 250, CV_8UC3, Scalar(0, 0, 0));
    string prStatus;
    prStatus = "Status_process";
    putText(imageStatus, prStatus, Point2f(10,17), FONT_HERSHEY_PLAIN, 0.77,  Scalar(0,0,255,255));
    namedWindow("Status_process",1);

    for(contActions = 0; contActions < listActions.size(); ++contActions)
    {
        action.clear();
        action = listActions[contActions];
        //cout<<"-------------------------------------" << endl;
        //cout<<"Actions -------- :  " << action.toStdString() << endl;
        pbar.setValue(contActions);

        if(!imgInput.empty())
        {
            QFileInfo info1(filename);

            QString rootName = info1.completeBaseName() + "." +
                    info1.completeSuffix();

            prStatus = "Process unit image";
            putText(imageStatus, prStatus, Point2f(10,41), FONT_HERSHEY_PLAIN, 0.75,  Scalar(0,0,255,255));
            imshow("Status_process", imageStatus);
            char key = waitKey(1);
            if(key == 'q' || key == 'Q')
            {
                destroyAllWindows();
                break;
            }

            if(action == "Flipping around X axis")
            {
                Mat imgFlipX;
                flip(imgInput, imgFlipX, 0);
                nameOut.clear();
                nameOut = folderOut + "/FlipX_" + rootName;
                imwrite(nameOut.toStdString(), imgFlipX);
            } // flip x

            if(action == "Flipping around Y axis")
            {
                Mat imgFlipY;
                flip(imgInput, imgFlipY, 1);
                nameOut.clear();
                nameOut = folderOut + "/FlipY_" + rootName;
                imwrite(nameOut.toStdString(), imgFlipY);
            } // flip y

            if(action == "Flipping around both axes")
            {
                Mat imgFlipXY;
                flip(imgInput, imgFlipXY, -1);
                nameOut.clear();
                nameOut = folderOut + "/FlipXY_" + rootName;
                imwrite(nameOut.toStdString(), imgFlipXY);
            } // flip xy

            if(action.contains(".yml"))
            {
                struct imgIdent outPV;

                outPV = processVision(action, imgInput);

                if(!outPV.imgS.empty())
                {

                    nameOut = folderOut + "/" + outPV.indent + "_" + rootName;

                    //cout<<"Name out:  " << nameOut.toStdString() << endl;

                    imwrite(nameOut.toStdString(), outPV.imgS);

                } // imgYML not empty

            } // ymnl process


        } // imgInput not empty

    } // for contActions

    destroyAllWindows();

    pbar.setValue(contActions);

} // end unitarioExec
















