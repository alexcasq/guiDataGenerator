#include "massivevision.h"

massiveVision::massiveVision(QString pathSalida, QStringList listFunctions)
{
    if(!pathSalida.isEmpty() && !listFunctions.empty())
    {
        pathOut         = pathSalida;
        listActions     = listFunctions;
    }


} // end massiveVision




void massiveVision::executeMassive(QString pathIMages)
{
    if(!pathIMages.isEmpty()){
        QFileInfoList listFiles;
        QDir dir(pathIMages);
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.bmp" <<"*.jpeg";
        listFiles = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);

        cout<<"Numer the files to process : " << listFiles.size() << endl;

        cout<<"Numer of actiosn : " << listActions.size() << endl;

        int contActions;
        QString action;
        Mat imgInput;
        QString nameOut;
        for(contActions = 0; contActions < listActions.size(); ++contActions)
        {
            action.clear();
            action = listActions[contActions];
            cout<<"-------------------------------------" << endl;
            cout<<"Actions -------- :  " << action.toStdString() << endl;

            foreach (const QFileInfo &fileinfo, listFiles)
            {
                QString imageFile = fileinfo.absoluteFilePath();
                cout<<"imageFile:  " << imageFile.toStdString() << endl;
                imgInput = imread(imageFile.toStdString());
                QString rootName = fileinfo.completeBaseName() + "." +
                        fileinfo.completeSuffix();

                if(action == "Flipping around X axis")
                {
                    Mat imgFlipX;
                    flip(imgInput, imgFlipX, 0);
                    nameOut.clear();
                    nameOut = pathOut + "/FlipX_" + rootName;
                    imwrite(nameOut.toStdString(), imgFlipX);
                } // flip x

                if(action == "Flipping around Y axis")
                {
                    Mat imgFlipY;
                    flip(imgInput, imgFlipY, 1);
                    nameOut.clear();
                    nameOut = pathOut + "/FlipY_" + rootName;
                    imwrite(nameOut.toStdString(), imgFlipY);
                } // flip y

                if(action == "Flipping around both axes")
                {
                    Mat imgFlipXY;
                    flip(imgInput, imgFlipXY, -1);
                    nameOut.clear();
                    nameOut = pathOut + "/FlipXY_" + rootName;
                    imwrite(nameOut.toStdString(), imgFlipXY);
                } // flip xy

                if(action.contains(".yml"))
                {
                    //read yml

                    // Verify is yml exist

                        // select function in yml

                        // execute function

                } // ymnl process

            } // list images

        } // for contActions

    } // pathIMages not empty



} // end executeMassive



void massiveVision::unitarioExec(Mat img)
{

} // end unitarioExec
















