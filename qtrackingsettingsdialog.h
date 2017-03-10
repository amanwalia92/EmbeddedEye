#ifndef QTRACKINGSETTINGSDIALOG_H
#define QTRACKINGSETTINGSDIALOG_H

#include <QDialog>
#include <qmaindefinitions.h>

namespace Ui {
class QTrackingSettingsDialog;
}

class QTrackingSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QTrackingSettingsDialog(QWidget *parent = 0);
    ~QTrackingSettingsDialog();

private:
    Ui::QTrackingSettingsDialog *ui;
};

#endif // QTRACKINGSETTINGSDIALOG_H
