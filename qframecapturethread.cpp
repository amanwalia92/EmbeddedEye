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

    mCam->set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
    mCam->set(CV_CAP_PROP_FRAME_WIDTH, 640);
    mCam->set(CV_CAP_PROP_FRAME_HEIGHT, 480);

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


    while(true){


        if(mCam && mCam->isOpened()){

            mCam->read(mOriginalFrame);

            emit sendFrame(&mOriginalFrame);

            if(mTimeToSleep>0)
            {
                 msleep(mTimeToSleep); // sleep for some time
            }
        }

    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QFrameCaptureThread::terminateCamera(){

    if(mCam->isOpened()){
        mCam->release();
    }
}
