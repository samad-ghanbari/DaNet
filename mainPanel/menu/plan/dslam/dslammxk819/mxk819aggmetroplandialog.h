#ifndef MXK819AggMetroPlanDialog
#define MXK819AggMetroPlanDialog

#include <QDialog>
class DanetDbMan;

namespace Ui {
class Mxk819AggMetroPlanDialog;
}

class Mxk819AggMetroPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Mxk819AggMetroPlanDialog(QWidget *parent, DanetDbMan *Db, const int ExchId, const int DslamId);
    ~Mxk819AggMetroPlanDialog();
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

    void on_next1Btn_clicked();

    void on_back2Btn_clicked();

    void on_Next2Btn_clicked();

    void on_metroBackBtn_clicked();

private:
    Ui::Mxk819AggMetroPlanDialog *ui;
    DanetDbMan *dbMan;
    const int exchId;
    const int dslamId;
    QString aggAllowPass, totalAllowPass; //, cxAllowPass
    QList<int> aggVlanList, cxVlanList, totalVlanList;
    bool AggChanged, cxChanged;
};

#endif // MXK819AggMetroPlanDialog
