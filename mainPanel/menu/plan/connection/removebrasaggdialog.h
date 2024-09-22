#ifndef REMOVEBRASAGGDIALOG_H
#define REMOVEBRASAGGDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveBrasAggDialog;
}

class RemoveBrasAggDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveBrasAggDialog(QWidget *parent, DanetDbMan *db, const int Id, const int BrasId, const QString BrasName, const QString Exch, const QString Agg1, const int Eth1, const QString Agg2, const int Eth2);
    ~RemoveBrasAggDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveBrasAggDialog *ui;
    DanetDbMan *dbMan;
    const int id, brasId, agg1Eth, agg2Eth;
};

#endif // REMOVEBRASAGGDIALOG_H
