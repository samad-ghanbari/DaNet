#ifndef ACCESSSINGLESWITCHDIALOG_H
#define ACCESSSINGLESWITCHDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AccessSingleSwitchDialog;
}

class AccessSingleSwitchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccessSingleSwitchDialog(QWidget *parent, DanetDbMan *db);
    ~AccessSingleSwitchDialog();
    void checkEntry();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_agg1CB_currentIndexChanged(int index);
    void on_metroCB_currentIndexChanged(int index);
    void on_nmsUplinkCB_currentTextChanged(const QString &arg1);
    void on_sigUplinkCB_currentTextChanged(const QString &arg1);
    void on_mediaUplinkCB_currentTextChanged(const QString &arg1);
    void on_voipUplinkCB_currentTextChanged(const QString &arg1);
    void on_agg1IntCB_currentTextChanged(const QString &arg1);
    void on_agg1ModCB_currentTextChanged(const QString &arg1);
    void on_metroIntCB_currentTextChanged(const QString &arg1);
    void on_metroModCB_currentTextChanged(const QString &arg1);
    void on_okBtn_clicked();
    void on_agg1RemoteIntLE_textChanged(const QString &arg1);
    void on_metroRemoteIntLE_textChanged(const QString &arg1);
    void on_bras2EnCB_toggled(bool checked);
    void on_bras1CB_currentIndexChanged(int index);
    void on_bras2CB_currentIndexChanged(int index);
    void on_ipNextBtn_clicked();
    void on_metroBackBtn_clicked();
    void on_metroNextBtn_clicked();
    void on_aggBackBtn_clicked();
    void on_aggNextBtn_clicked();
    void on_brasBackBtn_clicked();

private:
    Ui::AccessSingleSwitchDialog *ui;
    DanetDbMan *dbMan;
    bool bras_ip_check;
};

#endif // ACCESSSINGLESWITCHDIALOG_H
