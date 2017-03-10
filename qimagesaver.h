#ifndef QIMAGESAVER_H
#define QIMAGESAVER_H

#include <QObject>
#include <qopencvincludables.h>
#include <QMutex>

class QImageSaver : public QObject
{
    Q_OBJECT
public:
    explicit QImageSaver(int numFramesToSave);
    void setFrameCaptured(cv::Mat*);

signals:
    void finishedSaving(void);
    void sendStatus(QString);


public slots:
    void startSaving(void);

private:
    cv::Mat mFrmPointer;
    int mNumFramesToSave;
    int mCurrNumFramesSaved = 0;

    QMutex locker;

};

#endif // QIMAGESAVER_H
