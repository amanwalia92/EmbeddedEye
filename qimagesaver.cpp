#include "qimagesaver.h"

#include <iostream>
#include <string>
#include <QFile>
#include <QTextStream>

QImageSaver::QImageSaver(int numFramesToSave)
{
    mNumFramesToSave = numFramesToSave;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QImageSaver::setFrameCaptured(cv::Mat *frm)
{
    sendStatus("Got a frame!");
    mFrmPointer = frm->clone();
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QImageSaver::startSaving()
{

    //QString filename="QEM.txt";
    //QFile file( filename );
    //if ( file.open(QIODevice::ReadWrite) )
    //{
    //    QTextStream stream( &file );
    //    while(mCurrNumFramesSaved++ < mNumFramesToSave)
    //    {
    //        if(mCurrNumFramesSaved%1000==0)
    //        {
    //            sendStatus("Currently at:" + QString::number(mCurrNumFramesSaved));
    //        }
    //        stream << "something:" + QString::number(mCurrNumFramesSaved) << endl;
    //    }
    //}

    //sendStatus("STARTED TO SAVE!");

    std::string fileName = "./imgs/img";
    std::string fileType = ".jpg";
    //mFrmPointer = cv::Mat(400,400,CV_8UC3, cv::Scalar(255,0,100));
    while(mCurrNumFramesSaved < mNumFramesToSave)
    {
        //locker.lock();
        //if(mFrmPointer.data)
        //{
        //     // fileName + std::to_string(mCurrNumFramesSaved) + fileType
        //     if(!cv::imwrite(fileName + std::to_string(mCurrNumFramesSaved) + fileType, mFrmPointer)){
        //         sendStatus("FAILED TO SAVE!");
        //     }
        //     else
        //     {
        //         mCurrNumFramesSaved++;sendStatus("Saved Frame:" + QString::number(mCurrNumFramesSaved));
        //     }
        //}
        //else
        //{
        //    //sendStatus("No Data to save!");
        //}
        //locker.unlock();
    }

    emit finishedSaving();
}
////////////////////////////////////////////////////////////////////////////////////////////////
