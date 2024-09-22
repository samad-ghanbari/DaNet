#ifndef MXK819SingleAggMetroPlanDialog
#define MXK819SingleAggMetroPlanDialog

#include <QDialog>
class DanetDbMan;

namespace Ui {
class Mxk819SingleAggMetroPlanDialog;
}

class Mxk819SingleAggMetroPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Mxk819SingleAggMetroPlanDialog(QWidget *parent, DanetDbMan *Db, const int ExchId, const int DslamId);
    ~Mxk819SingleAggMetroPlanDialog();
    void checkEntry();

private slots:
    void on_agg1CB_currentIndexChanged(int index);
    void on_okBtn_clicked();
    void on_agg1EthSB_valueChanged(int arg1);
    void on_agg1Int1CB_currentIndexChanged(int index);
    void on_agg1Int2CB_currentIndexChanged(int index);
    void on_agg1Int3CB_currentIndexChanged(int index);
    void on_agg1Int4CB_currentIndexChanged(int index);
    void on_agg14IntCB_toggled(bool checked);
    void on_metroConnCB_toggled(bool checked);
    void on_cxdIntCB_toggled(bool checked);
    void on_cxCB_currentIndexChanged(int index);
    void on_a1i1diCB_currentIndexChanged(int index);
    void on_a1i2diCB_currentIndexChanged(int index);
    void on_a1i3diCB_currentIndexChanged(int index);
    void on_a1i4diCB_currentIndexChanged(int index);
    void on_cxi1diCB_currentIndexChanged(int index);
    void on_cxi2diCB_currentIndexChanged(int index);
    void on_cxInt1CB_currentIndexChanged(int index);
    void on_cxInt2CB_currentIndexChanged(int index);
    void on_a1mCB_currentTextChanged(const QString &arg1);
    void on_cxmCB_currentIndexChanged(int index);
    void on_cxEthSB_valueChanged(int arg1);
    void on_nextBtn_clicked();
    void on_metroBackBtn_clicked();
    void on_agg1ConnCB_toggled(bool checked);

private:
    Ui::Mxk819SingleAggMetroPlanDialog *ui;
    DanetDbMan *dbMan;
    const int exchId;
    const int dslamId;
    QString aggAllowPass, totalAllowPass; //, cxAllowPass
    QList<int> aggVlanList, cxVlanList, totalVlanList;
    bool AggChanged, cxChanged;
};

#endif // MXK819AggMetroPlanDialog
