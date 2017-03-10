#ifndef QSTREAMINGSETTINGS_H
#define QSTREAMINGSETTINGS_H

#include <QDialog>

namespace Ui {
class QStreamingSettings;
}

class QStreamingSettings : public QDialog
{
    Q_OBJECT

public:
    explicit QStreamingSettings(QWidget *parent = 0);
    ~QStreamingSettings();
    QString getAddress();
    QString getPortNumber();

private:
    Ui::QStreamingSettings *ui;
};

#endif // QSTREAMINGSETTINGS_H
