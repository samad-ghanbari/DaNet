#ifndef ADDBRASAGGDIALOG_H
#define ADDBRASAGGDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AddBrasAggDialog;
}

class AddBrasAggDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBrasAggDialog(QWidget *parent, DanetDbMan *db,const int area, const int BrasShelfId, const QString Bras);
    ~AddBrasAggDialog();
    void checkFormEntry();

private slots:
    void on_exchCB_currentIndexChanged(int index);
    void on_agg1CB_currentIndexChanged(int index);
    void on_agg2CB_currentIndexChanged(int index);
    void on_agg1SB_valueChanged(int arg1);
    void on_Agg2SB_valueChanged(int arg1);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::AddBrasAggDialog *ui;
    DanetDbMan *dbMan;
    const int brasDevexId;
};

#endif // ADDBRASAGGDIALOG_H
