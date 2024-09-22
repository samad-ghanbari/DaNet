#ifndef MA5616AGGMETROPLANDIALOG_H
#define MA5616AGGMETROPLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MA5616AggMetroPlanDialog;
}

class MA5616AggMetroPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5616AggMetroPlanDialog(QWidget *parent, DanetDbMan *Db, const int ExchId, const int DslamId);
    ~MA5616AggMetroPlanDialog();
    void checkEntry();

private slots:
    void on_aggCB_currentIndexChanged(int index);
    void on_aggEthSB_valueChanged(int arg1);
    void on_amCB_currentIndexChanged(int index);
    void on_aggInt1CB_currentIndexChanged(int index);
    void on_ai1diCB_currentIndexChanged(int index);
    void on_okBtn_clicked();

    void on_twoIntChB_toggled(bool checked);

    void on_aggInt2CB_currentIndexChanged(int index);

    void on_ai2diCB_currentIndexChanged(int index);

private:
    Ui::MA5616AggMetroPlanDialog *ui;
    DanetDbMan *dbMan;
    const int exchId;
    const int dslamId;
    int motherId;
    QString mother;
    QList<int> totalVlanList;
    QStringList  totalAllowPassSL;
    bool AggChanged;
};

#endif // MA5616AGGMETROPLANDIALOG_H
