#ifndef FLOORPLANREPORTDIALOG_H
#define FLOORPLANREPORTDIALOG_H

#include <QDialog>
#include <QMap>

class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class FloorPlanReportDialog;
}

class FloorPlanReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FloorPlanReportDialog(QWidget *parent, DanetDbMan *db);
    ~FloorPlanReportDialog();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_saloonCB_currentIndexChanged(int index);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::FloorPlanReportDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    QMap<int, QList<int>> odfMap;
};

#endif // FLOORPLANREPORTDIALOG_H
