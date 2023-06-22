#ifndef ADDAGGCXDIALOG_H
#define ADDAGGCXDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AddAggCxDialog;
}

class AddAggCxDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAggCxDialog(QWidget *parent,DanetDbMan *Db, const int area);
    ~AddAggCxDialog();

private slots:
    void on_abbrCB_currentIndexChanged(int index);
    void on_agg1CB_currentIndexChanged(int index);
    void on_cxCB_currentIndexChanged(int index);
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::AddAggCxDialog *ui;
    DanetDbMan *dbMan;
};

#endif // ADDAGGCXDIALOG_H
