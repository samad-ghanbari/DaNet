#ifndef EDITAGGCXDIALOG_H
#define EDITAGGCXDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditAggCxDialog;
}

class EditAggCxDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditAggCxDialog(QWidget *parent, DanetDbMan *Db, const int Id, const int area, const int exchId, const int aggId, const int aggEth,const int cxId, const int cxEth);
    ~EditAggCxDialog();

private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();
    void on_abbrCB_currentIndexChanged(int index);
    void on_agg1CB_currentIndexChanged(int index);
    void on_cxCB_currentIndexChanged(int index);
    void on_aggEthSB_valueChanged(int arg1);
    void on_cxEthSB_valueChanged(int arg1);

private:
    Ui::EditAggCxDialog *ui;
    DanetDbMan *dbMan;
    const int id;
    const int currentAggEth, currentCxEth;
};

#endif // EDITAGGCXDIALOG_H
