#include "qtrackingsettingsdialog.h"
#include "ui_qtrackingsettingsdialog.h"

QTrackingSettingsDialog::QTrackingSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTrackingSettingsDialog)
{
    ui->setupUi(this);

    ui->comboBox_trackingMethod->addItem(QString(TRACKING_METHOD_VIOLAJONES));
    ui->comboBox_trackingMethod->addItem(QString(TRACKING_METHOD_VIOLAJONES_TEMPLATEMATCHING));
    ui->comboBox_trackingMethod->addItem(QString(TRACKING_METHOD_MOTION));

    //ui->comboBox_fovCamNum->addItem(QString::number(i));
    //ui->comboBox_panCamNum->addItem(QString::number(i));
}

QTrackingSettingsDialog::~QTrackingSettingsDialog()
{
    delete ui;
}
