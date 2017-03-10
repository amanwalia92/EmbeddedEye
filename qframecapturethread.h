#ifndef QFRAMECAPTURETHREAD_H
#define QFRAMECAPTURETHREAD_H

#include <QObject>
#include <QThread>
#include <QMessageBox>

#include <qopencvincludables.h>

class QFrameCaptureThread : public QThread
{
    Q_OBJECT
public:
    explicit QFrameCaptureThread(int camNumber, int timeToSleep, QObject *parent = 0);
    ~QFrameCaptureThread();

    void openCamera(int camNum);
    void setTimeToSleep(int ts);
    bool isCameraOpen();
    void terminateCamera();


protected:
    void run() override;

signals:
    void sendFrame(cv::Mat*);

public slots:

private:
    int mTimeToSleep;// ms
    int mCamNumber;

    cv::VideoCapture *mCam;
    cv::Mat mOriginalFrame;

};

#endif // QFRAMECAPTURETHREAD_H
