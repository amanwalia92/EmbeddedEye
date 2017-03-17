#ifndef QFRAMEDISPLAY_H
#define QFRAMEDISPLAY_H

#include <QObject>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>

#include <qmaindefinitions.h>
#include <qopencvincludables.h>

class QFrameDisplay : public QLabel
{
    Q_OBJECT

public:
    QFrameDisplay(int type);

protected:
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;

public slots:
    void receiveFrameCaptured(cv::Mat*);

signals:
    void sendMousePosition(QPoint);
    void sendClickedMousePosition(QPoint);
    void sendFrameDisplayed(void);

private:
    int mType = -1;
    QPixmap mPixMap;
    int mNum = 0;

};

#endif // QFRAMEDISPLAY_H
