#ifndef QEMBEDDEDEYE_H
#define QEMBEDDEDEYE_H

#include <QMainWindow>
#include <qcamerasettingsdialog.h>
#include <qtrackingsettingsdialog.h>
#include <qstreamingsettings.h>
#include <qframedisplay.h>
#include <qstreamer.h>

#include <qmaindefinitions.h>
#include <qframecapturethread.h>
#include <qimagesaver.h>


namespace Ui {
class QEmbeddedEye;
}

class QEmbeddedEye : public QMainWindow
{
    Q_OBJECT

public:
    explicit QEmbeddedEye(QWidget *parent = 0);
    ~QEmbeddedEye();

 protected:
    void closeEvent(QCloseEvent*) override;

private slots:
    void on_actionCamera_Settings_triggered();

    void on_actionTracking_Settings_triggered();

    void on_actionPanoramic_triggered();

    void on_actionFoveal_triggered();

    void receiveAndSetMousePos(QPoint);

    void on_actionExit_triggered();

    void on_pushButton_startPanoramicCam_clicked();

    void on_pushButton_startFovealCam_clicked();

    void on_pushButton_doTracking_clicked();

    void on_pushButton_captureImages_clicked();

    // slots for objects sending signals NOT GUI related

    void receiveImageSaverStatus(QString);

    void receivePanFrame(cv::Mat*);

    void receiveFinishedSavingFrames();

    void receivePanFrameDisplayed(void);

    void receiveFovFrameDisplayed(void);


    void on_actionStreaming_Settings_triggered();

    void on_pushButton_StartStopStreaming_clicked();

private:
    Ui::QEmbeddedEye *ui;

    QFrameDisplay *mPanDisplay;
    QFrameDisplay *mFovDisplay;

    int mPanCamNumber;
    int mFovCamNumber;

    int mPanFrameTime;
    int mFovFrameTime;

    double mPanDisplayTime; // used for FPS
    double mFovDisplayTime; // used for FPS

    QFrameCaptureThread *mPanCameraCapture; bool mPanPlaying = false;

    QFrameCaptureThread *mFovCameraCapture; bool mFovPlaying = false;

    // just used for testing moving an object to another thread
    QThread *thread = NULL;
    QImageSaver *task = NULL;

    QStreamer * stream = NULL;

    void terminateAllThreads();

};

#endif // QEMBEDDEDEYE_H
