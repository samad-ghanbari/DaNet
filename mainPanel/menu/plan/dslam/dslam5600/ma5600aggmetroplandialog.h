#ifndef MA5600AGGMETROPLANDIALOG_H
#define MA5600AGGMETROPLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MA5600AggMetroPlanDialog;
}

class MA5600AggMetroPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5600AggMetroPlanDialog(QWidget *parent, DanetDbMan *Db, const int ExchId, const int DslamId);
    ~MA5600AggMetroPlanDialog();
    void checkEntry();

private slots:
    void on_agg1CB_currentIndexChanged(int index);
    void on_agg2CB_currentIndexChanged(int index);
    void on_okBtn_clicked();
    void on_agg1EthSB_valueChanged(int arg1);
    void on_agg2EthSB_valueChanged(int arg1);
    void on_agg1Int1CB_currentIndexChanged(int index);
    void on_agg1Int2CB_currentIndexChanged(int index);
    void on_agg2Int1CB_currentIndexChanged(int index);
    void on_agg2Int2CB_currentIndexChanged(int index);
    void on_agg1dIntCB_toggled(bool checked);
    void on_agg2ConnCB_toggled(bool checked);
    void on_agg2dIntCB_toggled(bool checked);
    void on_metroConnCB_toggled(bool checked);
    void on_cxdIntCB_toggled(bool checked);
    void on_cxCB_currentIndexChanged(int index);
    void on_a1i1diCB_currentIndexChanged(int index);
    void on_a1i2diCB_currentIndexChanged(int index);
    void on_a2i1diCB_currentIndexChanged(int index);
    void on_a2i2diCB_currentIndexChanged(int index);
    void on_cxi1diCB_currentIndexChanged(int index);
    void on_cxi2diCB_currentIndexChanged(int index);
    void on_cxInt1CB_currentIndexChanged(int index);
    void on_cxInt2CB_currentIndexChanged(int index);
    void on_a1mCB_currentTextChanged(const QString &arg1);
    void on_a2mCB_currentIndexChanged(int index);
    void on_cxmCB_currentIndexChanged(int index);

    void on_cxEthSB_valueChanged(int arg1);

    void on_nextBtn_clicked();

    void on_metroBackBtn_clicked();

    void on_agg2NextBtn_clicked();

    void on_agg2BackBtn_clicked();

    void on_agg1ConnCB_toggled(bool checked);

private:
    Ui::MA5600AggMetroPlanDialog *ui;
    DanetDbMan *dbMan;
    const int exchId;
    const int dslamId;
    QString aggAllowPass,  totalAllowPass; //cxAllowPass,
    QList<int> aggVlanList,  totalVlanList; //cxVlanList,
    bool AggChanged, cxChanged;
};

#endif // MA5600AGGMETROPLANDIALOG_H
