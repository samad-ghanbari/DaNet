#ifndef EDITBRASAGGDIALOG_H
#define EDITBRASAGGDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditBrasAggDialog;
}

class EditBrasAggDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditBrasAggDialog(QWidget *parent, DanetDbMan *db, const int Id, const int BrasId, const int Area, const QString BrasName, const int ExchId, const int Agg1Id, const int Eth1, const int Agg2Id, const int Eth2);
    ~EditBrasAggDialog();
    void checkFormEntry();

private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
    void on_exchCB_currentIndexChanged(int index);
    void on_agg1CB_currentIndexChanged(int index);
    void on_eth1SB_valueChanged(int arg1);
    void on_agg2CB_currentIndexChanged(int index);
    void on_eth2SB_valueChanged(int arg1);

private:
    Ui::EditBrasAggDialog *ui;
    DanetDbMan *dbMan;
    const int id, brasId, currentAgg1Eth, currentAgg2Eth;
};

#endif // EDITBRASAGGDIALOG_H
