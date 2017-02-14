#include "qframecapturethread.h"

#include <QDebug>

QFrameCaptureThread::QFrameCaptureThread(int camNumber, int timeToSleep, QObject *parent) : QThread(parent)
{
    mTimeToSleep = timeToSleep;
    mCamNumber = camNumber;
    mCam = new cv::VideoCapture();
}
QFrameCaptureThread::~QFrameCaptureThread()
{
    if(mCam->isOpened()){
        mCam->release();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QFrameCaptureThread::openCamera(int camNum){

    if(mCam->isOpened()){
        mCam->release();
    }
    mCamNumber = camNum;
    mCam->open(mCamNumber);
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool QFrameCaptureThread::isCameraOpen(){
    return mCam->isOpened();
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QFrameCaptureThread::setTimeToSleep(int ts){
    mTimeToSleep = ts;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void QFrameCaptureThread::run(){

    if(!mCam->isOpened()){
        //QMessageBox::warning(NULL, "Camera Not Opened","Failed to Open Camera:" + QString::number(mCamNumber));
        // qDebug() << "failed to open camera: " + QString::number(mCamNumber);
        return;
    }

    while(true){

        mCam->read(mOriginalFrame);
        emit sendFrame(&mOriginalFrame);
        msleep(mTimeToSleep); // sleep for some time
    }
}
