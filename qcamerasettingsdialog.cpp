#include "qcamerasettingsdialog.h"
#include "ui_qcamerasettingsdialog.h"

QCameraSettingsDialog::QCameraSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCameraSettingsDialog)
{
    ui->setupUi(this);

    for(int i = 0; i < 5; i++)
    {
        ui->comboBox_fovCamNum->addItem(QString::number(i));
        ui->comboBox_panCamNum->addItem(QString::number(i));
    }


}
////////////////////////////////////////////////////////////////////////////////////////////////
QCameraSettingsDialog::~QCameraSettingsDialog()
{
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////
int QCameraSettingsDialog::getPanCameraNumber(){
    return ui->comboBox_panCamNum->currentText().toInt();
}
////////////////////////////////////////////////////////////////////////////////////////////////
int QCameraSettingsDialog::getFovCameraNumber(){
    return ui->comboBox_fovCamNum->currentText().toInt();
}
////////////////////////////////////////////////////////////////////////////////////////////////
int QCameraSettingsDialog::getPanFrameTime(){
    return ui->spinBox_panFrameTime->value();
}
////////////////////////////////////////////////////////////////////////////////////////////////
int QCameraSettingsDialog::getFovFrameTime(){
    return ui->spinBox_fovFrameTime->value();
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QCameraSettingsDialog::setCameraNumbers(int pNum, int fNum){
    ui->comboBox_panCamNum->setCurrentIndex(pNum);
    ui->comboBox_fovCamNum->setCurrentIndex(fNum);
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QCameraSettingsDialog::setFrameTimes(int pTime, int fTime){
    ui->spinBox_panFrameTime->setValue(pTime);
    ui->spinBox_fovFrameTime->setValue(fTime);
}
