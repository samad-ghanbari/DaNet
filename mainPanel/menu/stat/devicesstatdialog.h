#ifndef DEVICESSTATDIALOG_H
#define DEVICESSTATDIALOG_H

#include <QDialog>
class QStandardItemModel;
class DanetDbMan;

namespace Ui {
class DevicesStatDialog;
}

class DevicesStatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DevicesStatDialog(QWidget *parent , DanetDbMan *db);
    ~DevicesStatDialog();

private slots:
    void on_CancelBtn_clicked();
    void on_exportBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);

private:
    Ui::DevicesStatDialog *ui;
    DanetDbMan *dbMan;
    QStandardItemModel *model;
};

#endif // DEVICESSTATDIALOG_H
