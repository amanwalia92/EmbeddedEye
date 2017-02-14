#ifndef QCAMERASETTINGSDIALOG_H
#define QCAMERASETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class QCameraSettingsDialog;
}

class QCameraSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QCameraSettingsDialog(QWidget *parent = 0);
    ~QCameraSettingsDialog();

    int getPanCameraNumber();
    int getFovCameraNumber();
    int getPanFrameTime();
    int getFovFrameTime();

    void setCameraNumbers(int pNum, int fNum);
    void setFrameTimes(int pTime, int fTime);

private:
    Ui::QCameraSettingsDialog *ui;
};

#endif // QCAMERASETTINGSDIALOG_H
