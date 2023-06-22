#ifndef REPORTDEVICEDIALOG_H
#define REPORTDEVICEDIALOG_H

#include <QDialog>
#include <QMap>

class DanetDbMan;
class QSqlQueryModel;
class QStandardItemModel;

namespace Ui {
class ReportDeviceDialog;
}

class ReportDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDeviceDialog(QWidget *parent, DanetDbMan *db);
    ~ReportDeviceDialog();
    void fillReportTV(const int devexId);
    void fillStatTV(const int devexId);

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_deviceCB_currentIndexChanged(int index);

private:
    Ui::ReportDeviceDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    QStandardItemModel *statModel;
    QMap<int,int> devexIdDeviceId, devexIdSaloonNo;
};

#endif // REPORTDEVICEDIALOG_H
