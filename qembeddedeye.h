#ifndef QEMBEDDEDEYE_H
#define QEMBEDDEDEYE_H

#include <QMainWindow>
#include <qcamerasettingsdialog.h>
#include <qframedisplay.h>

#include <qmaindefinitions.h>
#include <qframecapturethread.h>


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

    void on_actionPanoramic_triggered();

    void on_actionFoveal_triggered();

    void receiveAndSetMousePos(QPoint);

    void on_actionExit_triggered();

    void on_pushButton_startPanoramicCam_clicked();

    void on_pushButton_startFovealCam_clicked();

    void on_pushButton_doTracking_clicked();

private:
    Ui::QEmbeddedEye *ui;

    QFrameDisplay *mPanDisplay;
    QFrameDisplay *mFovDisplay;

    int mPanCamNumber;
    int mFovCamNumber;

    int mPanFrameTime;
    int mFovFrameTime;


    QFrameCaptureThread *mPanCameraCapture; bool mPanPlaying = false;

    QFrameCaptureThread *mFovCameraCapture; bool mFovPlaying = false;


    void terminateAllThreads();

};

#endif // QEMBEDDEDEYE_H
