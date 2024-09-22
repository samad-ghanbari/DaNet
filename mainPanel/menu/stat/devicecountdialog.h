#ifndef DEVICESCOUNTDIALOG_H
#define DEVICESCOUNTDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class DeviceCountDialog;
}

class DeviceCountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceCountDialog(QWidget *parent , DanetDbMan *db);
    ~DeviceCountDialog();

private slots:
    void on_CancelBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);
    void on_devCB_currentIndexChanged(int index);

    void on_abbrCB_currentIndexChanged(int index);

private:
    Ui::DeviceCountDialog *ui;
    DanetDbMan *dbMan;
};

#endif // DEVICESCOUNTDIALOG_H
