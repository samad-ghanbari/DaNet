#ifndef ACCESSDOUBLESWITCHDIALOG_H
#define ACCESSDOUBLESWITCHDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AccessDoubleSwitchDialog;
}

class AccessDoubleSwitchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccessDoubleSwitchDialog(QWidget *parent, DanetDbMan *db);
    ~AccessDoubleSwitchDialog();
    void checkEntry();

private slots:
    void on_areaCB_currentIndexChanged(int index);

    void on_LabbrCB_currentIndexChanged(int index);

    void on_RabbrCB_currentIndexChanged(int index);

    void on_LNmsUplinkCB_currentTextChanged(const QString &arg1);

    void on_LSigUplinkCB_currentTextChanged(const QString &arg1);

    void on_LMediaUplinkCB_currentTextChanged(const QString &arg1);

    void on_LVoipUplinkCB_currentTextChanged(const QString &arg1);

    void on_RNmsUplinkCB_currentTextChanged(const QString &arg1);

    void on_RSigUplinkCB_currentTextChanged(const QString &arg1);

    void on_RMediaUplinkCB_currentTextChanged(const QString &arg1);

    void on_RVoipUplinkCB_currentTextChanged(const QString &arg1);

    void on_INmsUplinkCB_currentTextChanged(const QString &arg1);

    void on_ISigUplinkCB_currentTextChanged(const QString &arg1);

    void on_IMediaUplinkCB_currentTextChanged(const QString &arg1);

    void on_IVoipUplinkCB_currentTextChanged(const QString &arg1);

    void on_LmetroCB_currentIndexChanged(int index);

    void on_LmetroIntCB_currentTextChanged(const QString &arg1);

    void on_LmetroRemoteIntLE_textChanged(const QString &arg1);

    void on_LmetroModCB_currentTextChanged(const QString &arg1);

    void on_RmetroCB_currentIndexChanged(int index);

    void on_RmetroIntCB_currentTextChanged(const QString &arg1);

    void on_RmetroRemoteIntLE_textChanged(const QString &arg1);

    void on_RmetroModCB_currentTextChanged(const QString &arg1);

    void on_agg1CB_currentIndexChanged(int index);

    void on_agg1IntCB_currentTextChanged(const QString &arg1);

    void on_agg1RemoteIntLE_textChanged(const QString &arg1);

    void on_agg1ModCB_currentTextChanged(const QString &arg1);

    void on_agg2CB_currentIndexChanged(int index);

    void on_agg2IntCB_currentTextChanged(const QString &arg1);

    void on_agg2RemoteIntLE_textChanged(const QString &arg1);

    void on_agg2ModCB_currentTextChanged(const QString &arg1);

    void on_bras1CB_currentIndexChanged(int index);

    void on_bras2CB_currentIndexChanged(int index);

    void on_bras2EnCB_toggled(bool checked);

    void on_okBtn_clicked();

    void on_ipNextBtn_clicked();

    void on_metroBackBtn_clicked();

    void on_metroNextBtn_clicked();

    void on_aggBackBtn_clicked();

    void on_aggNextBtn_clicked();

    void on_brasBackBtn_clicked();

private:
    Ui::AccessDoubleSwitchDialog *ui;
    DanetDbMan *dbMan;
    bool bras_ip_check;
};

#endif // ACCESSDOUBLESWITCHDIALOG_H
