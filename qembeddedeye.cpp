#include "qembeddedeye.h"
#include "ui_qembeddedeye.h"

#include <QDebug>
#include <QMessageBox>

QEmbeddedEye::QEmbeddedEye(QWidget *parent) :
    QMainWindow(parent),
    mPanDisplay(NULL),
    mFovDisplay(NULL),
    mPanCamNumber(0), mFovCamNumber(1),
    mPanFrameTime(20),mFovFrameTime(20),
    ui(new Ui::QEmbeddedEye)
{

    // create the camera capturing threads
    mPanCameraCapture = new QFrameCaptureThread(mPanCamNumber, mPanFrameTime, this);
    mPanCameraCapture->openCamera(mPanCamNumber); // try opening the camera

    mFovCameraCapture = new QFrameCaptureThread(mFovCamNumber, mFovFrameTime, this);
    mFovCameraCapture->openCamera(mFovCamNumber); // try opening the camera


    ui->setupUi(this);
}
////////////////////////////////////////////////////////////////////////////////////////////////
QEmbeddedEye::~QEmbeddedEye()
{
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::closeEvent(QCloseEvent*){
    terminateAllThreads();
    QApplication::quit();
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::terminateAllThreads(){
    mPanCameraCapture->terminate();
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_actionCamera_Settings_triggered()
{
    QCameraSettingsDialog mCamSettings;

    mCamSettings.setCameraNumbers(mPanCamNumber,mFovCamNumber);
    mCamSettings.setFrameTimes(mPanFrameTime,mFovFrameTime);

    mCamSettings.setModal(true);
    mCamSettings.exec();

    if(mCamSettings.result() == QDialog::Accepted) // if accepted, set the new camera settings
    {
        //qDebug() << "---------------\nmCamSettins.getFovCameraNumber():" << mCamSettings.getFovCameraNumber() <<
        //            " mCamSettins.getFovFrameTime():" << mCamSettings.getFovFrameTime();
        //
        //qDebug() << "mCamSettins.getPanCameraNumber():" << mCamSettings.getPanCameraNumber() <<
        //            " mCamSettins.getPanFrameTime():" << mCamSettings.getPanFrameTime();


        if(mCamSettings.getPanCameraNumber() == mCamSettings.getFovCameraNumber())
        {
            QMessageBox::warning(this, "Same Camera Number", "Please select different camera numbers for each camera.");
            return;
        }

        mPanCamNumber = mCamSettings.getPanCameraNumber();
        mFovCamNumber = mCamSettings.getFovCameraNumber();

        mPanFrameTime = mCamSettings.getPanFrameTime();
        mFovFrameTime = mCamSettings.getFovFrameTime();

        // try reopening the pan camera
        mPanCameraCapture->terminate();
        ui->pushButton_startPanoramicCam->setText("Start Pan"); mPanPlaying = false;
        mPanCameraCapture->openCamera(mPanCamNumber); mPanCameraCapture->setTimeToSleep(mPanFrameTime);

        // try reopening the foveal camera
        mFovCameraCapture->terminate();
        ui->pushButton_startFovealCam->setText("Start Foveal"); mFovPlaying = false;
        mFovCameraCapture->openCamera(mFovCamNumber); mFovCameraCapture->setTimeToSleep(mFovFrameTime);

    }

}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_actionPanoramic_triggered()
{
    if(mPanDisplay){
        mPanDisplay->show();
    }
    else{

        mPanDisplay = new QFrameDisplay(PAN_DISPLAY);
        // connect the mouse movement events and mouse click events
        connect(mPanDisplay, SIGNAL(sendMousePosition(QPoint)), this, SLOT(receiveAndSetMousePos(QPoint)));

        // connect the camera
        connect(mPanCameraCapture, SIGNAL(sendFrame(cv::Mat*)),mPanDisplay, SLOT(receiveFrameCaptured(cv::Mat*)));

        mPanDisplay->show();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_actionFoveal_triggered()
{
    if(mFovDisplay){
        mFovDisplay->show();
    }
    else{
        mFovDisplay = new QFrameDisplay(FOVEAL_DISPLAY);

        // connect the camera
        connect(mFovCameraCapture, SIGNAL(sendFrame(cv::Mat*)), mFovDisplay, SLOT(receiveFrameCaptured(cv::Mat*)));

        mFovDisplay->show();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::receiveAndSetMousePos(QPoint pos){

    ui->label_XYPos->setText(QString::number(pos.x()) + " , " + QString::number(pos.y()));
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_actionExit_triggered()
{
    terminateAllThreads();
    QApplication::quit();
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_pushButton_startPanoramicCam_clicked()
{
    if(mPanCameraCapture->isCameraOpen())
    {
        if(mPanPlaying)
        {
            mPanCameraCapture->terminate();
            ui->pushButton_startPanoramicCam->setText("Start Pan");
        }
        else
        {
            mPanCameraCapture->start();
            ui->pushButton_startPanoramicCam->setText("Stop Pan");
        }
        mPanPlaying = !mPanPlaying;

    }
    else
    {
        QMessageBox::warning(this, "Panoramic Camera Failed to Open", "Unable to Open Panoramic Camera: " + QString::number(mPanCamNumber));
    }

}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_pushButton_startFovealCam_clicked()
{
    if(mFovCameraCapture->isCameraOpen())
    {
        if(mFovPlaying)
        {
            mFovCameraCapture->terminate();
            ui->pushButton_startFovealCam->setText("Start Foveal");
        }
        else
        {
            mFovCameraCapture->start();
            ui->pushButton_startFovealCam->setText("Stop Foveal");
        }
        mFovPlaying = !mFovPlaying;

    }
    else
    {
        QMessageBox::warning(this, "Foveal Camera Failed to Open", "Unable to Open Foveal Camera: " + QString::number(mFovCamNumber));
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_pushButton_doTracking_clicked()
{

}
