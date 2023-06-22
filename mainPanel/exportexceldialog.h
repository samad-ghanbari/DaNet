#ifndef EXPORTEXCELDIALOG_H
#define EXPORTEXCELDIALOG_H

#include <QDialog>
class QSqlQueryModel;
class DanetDbMan;

namespace Ui {
class ExportExcelDialog;
}

class ExportExcelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportExcelDialog(QWidget *parent,DanetDbMan *db,const QList<int> IntIds);
    ~ExportExcelDialog();

private slots:
    void on_confirmChB_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::ExportExcelDialog *ui;
    DanetDbMan *dbMan;
    const QList<int> intIdList;
    QSqlQueryModel *model;
};

#endif // EXPORTEXCELDIALOG_H
