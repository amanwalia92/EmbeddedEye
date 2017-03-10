#include "qembeddedeye.h"
#include "ui_qembeddedeye.h"

#include <QDebug>
#include <QMessageBox>

QEmbeddedEye::QEmbeddedEye(QWidget *parent) :
    QMainWindow(parent),
    mPanDisplay(NULL),
    mFovDisplay(NULL),
    mPanCamNumber(1), mFovCamNumber(0),
    mPanFrameTime(0),mFovFrameTime(0),
    mPanDisplayTime(0), mFovDisplayTime(0),
    ui(new Ui::QEmbeddedEye)
{
    // create the camera capturing threads
    mPanCameraCapture = new QFrameCaptureThread(mPanCamNumber, mPanFrameTime, this);
    connect( mPanCameraCapture, SIGNAL(finished()), mPanCameraCapture, SLOT(deleteLater()));

    mFovCameraCapture = new QFrameCaptureThread(mFovCamNumber, mFovFrameTime, this);
    connect( mFovCameraCapture, SIGNAL(finished()), mFovCameraCapture, SLOT(deleteLater()));


    stream = new QStreamer();
    if(!stream->createElements())
    {
        QMessageBox::warning(this, "GStreamer failed", "GStreamer failed to create pipeline elements!");
        return;
    }

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
    mPanCameraCapture->quit();
    mFovCameraCapture->quit();

    if(stream)
    {
        stream->stopStreaming();
    }
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
        if(mCamSettings.getPanCameraNumber() == mCamSettings.getFovCameraNumber())
        {
            QMessageBox::warning(this, "Same Camera Number", "Please select different camera numbers for each camera.");
            return;
        }

        // set the camera numbers
        mPanCamNumber = mCamSettings.getPanCameraNumber();
        mFovCamNumber = mCamSettings.getFovCameraNumber();

        // set the inter frame time if necessary
        mPanFrameTime = mCamSettings.getPanFrameTime();
        mFovFrameTime = mCamSettings.getFovFrameTime();

        mPanCameraCapture->setTimeToSleep(mPanFrameTime);
        mFovCameraCapture->setTimeToSleep(mFovFrameTime);


    }

}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_actionTracking_Settings_triggered()
{
    QTrackingSettingsDialog mTrackSettings;

    mTrackSettings.setModal(true);
    mTrackSettings.exec();

    if(mTrackSettings.result() == QDialog::Accepted) // if accepted, set the new camera settings
    {

        qDebug() << "Accepted tracking settings";
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
        // connect frame displayed image
        connect(mPanDisplay, SIGNAL(sendFrameDisplayed()), this, SLOT(receivePanFrameDisplayed()));


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
        // connect frame displayed image
        connect(mFovDisplay, SIGNAL(sendFrameDisplayed()), this, SLOT(receiveFovFrameDisplayed()));

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
    if(!mPanPlaying)
    {
        mPanCameraCapture->openCamera(mPanCamNumber);

        if(mPanCameraCapture->isCameraOpen())
        {
            mPanCameraCapture->start();
            mPanPlaying = true;
            ui->pushButton_startPanoramicCam->setEnabled(false);
        }
        else
        {
            QMessageBox::warning(this, "Panoramic Camera Failed to Open", "Unable to Open Panoramic Camera: " + QString::number(mPanCamNumber) + ". Restart application and select another camera number!");
        }
    }

}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_pushButton_startFovealCam_clicked()
{
    if(!mFovPlaying)
    {
        mFovCameraCapture->openCamera(mFovCamNumber);

        if(mFovCameraCapture->isCameraOpen())
        {

            QMessageBox::StandardButton rpns;
            rpns = QMessageBox::question(this, "Display Foveal Feed", "Displaying this foveal feed will not allow for feed to be streamed. "
                                                                      "If you want to stream then select 'No'. If you want to display "
                                                                      "the feed, click 'Yes'.",
                                         QMessageBox::Yes | QMessageBox::No
                                        );

            if(rpns == QMessageBox::Yes)
            {
                mFovCameraCapture->start();
                mFovPlaying = true;
                ui->pushButton_startFovealCam->setEnabled(false);
            }
        }
        else
        {
            QMessageBox::warning(this, "Foveal Camera Failed to Open", "Unable to Open Foveal Camera: " + QString::number(mFovCamNumber) + ". Restart application and select another camera number!");
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_pushButton_doTracking_clicked()
{

}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_pushButton_captureImages_clicked()
{
    // we might not need the image saving functionality
    //qDebug() << "on_pushButton_captureImages_clicked";
    //if(!thread)
    //{
    //    thread = new QThread( );
    //    task  = new QImageSaver(100);
    //    task->moveToThread(thread);
    //
    //    connect( thread, SIGNAL(started()), task, SLOT(startSaving()));
    //    connect( task, SIGNAL(finishedSaving()), thread, SLOT(quit()) );
    //
    //    connect(task, SIGNAL(sendStatus(QString)), this, SLOT(receiveImageSaverStatus(QString)));
    //
    //    connect(task, SIGNAL(finishedSaving()), this, SLOT(receiveFinishedSavingFrames()));
    //
    //    //automatically delete thread and task object when work is done:
    //    //connect( thread, SIGNAL(finished()), task, SLOT(deleteLater()) );
    //    //connect( thread, SIGNAL(finished()), thread, SLOT(deleteLater()) );
    //    thread->start();
    //}
    //else
    //{
    //
    //}
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::receiveImageSaverStatus(QString str)
{
    ui->label_imageSaverStatus->setText(str);
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::receivePanFrame(cv::Mat*frm){
    // qDebug() << "receive Pan Frame!";
    if(task){
        task->setFrameCaptured(frm);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::receiveFinishedSavingFrames(){
    // not yet implemented
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::receivePanFrameDisplayed(void){

    double ellapsedTime = CLOCKS_PER_SEC/(clock() - mPanDisplayTime);
    ui->label_panFPSDisplay->setText(QString::number(ellapsedTime));
    mPanDisplayTime = clock();
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::receiveFovFrameDisplayed(void){


    double ellapsedTime = CLOCKS_PER_SEC/(clock() - mFovDisplayTime);
    ui->label_fovFPSDisplay->setText(QString::number(ellapsedTime));
    mFovDisplayTime = clock();
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_actionStreaming_Settings_triggered()
{
    QStreamingSettings mStreamSettings;

    mStreamSettings.setModal(true);
    mStreamSettings.exec();

    if(mStreamSettings.result() == QDialog::Accepted)
    {
        ui->label_StreamingStatus->setText(mStreamSettings.getAddress() /* + " | " + mStreamSettings.getPortNumber() */);

        stream->setCameraID(mFovCamNumber);
        stream->addClient(mStreamSettings.getAddress().toUtf8().constData()/*, mStreamSettings.getPortNumber().toUtf8().constData()*/);


    } // 10.24.191.215

}
////////////////////////////////////////////////////////////////////////////////////////////////
void QEmbeddedEye::on_pushButton_StartStopStreaming_clicked()
{

    if(stream)
    {
        stream->initialize();
        stream->startStreaming();

    }

}
