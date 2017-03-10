#include "qstreamingsettings.h"
#include "ui_qstreamingsettings.h"

QStreamingSettings::QStreamingSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QStreamingSettings)
{
    ui->setupUi(this);
}

QStreamingSettings::~QStreamingSettings()
{
    delete ui;
}


QString QStreamingSettings::getAddress(){
    return ui->lineEdit_IPAddress->text();
}
/*
QString QStreamingSettings::getPortNumber(){
    return ui->lineEdit_PortNumber->text();
}
*/
