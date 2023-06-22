#ifndef REMOVEAGGCXDIALOG_H
#define REMOVEAGGCXDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveAggCxDialog;
}

class RemoveAggCxDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveAggCxDialog(QWidget *parent,DanetDbMan *Db,const int Id, const QString Abbr,const int Agg1Id, const QString Agg,const int AggEth,const int CxId, const QString Cx, const int CxEth);
    ~RemoveAggCxDialog();

private slots:
    void on_checkBox_toggled(bool checked);

    void on_okBtn_clicked();

private:
    Ui::RemoveAggCxDialog *ui;
    DanetDbMan *dbMan;
    const int id;
    const int agg1Id, cxId, agg1Eth, cxEth;
};

#endif // REMOVEAGGCXDIALOG_H
