#ifndef MA5616SINGLEAGGBRASPLANDIALOG_H
#define MA5616SINGLEAGGBRASPLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MA5616SingleAggBrasPlanDialog;
}

class MA5616SingleAggBrasPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5616SingleAggBrasPlanDialog(QWidget *parent, DanetDbMan *Db, const int Area, const int ExchId, const int DslamId, const QString dslamName);
    ~MA5616SingleAggBrasPlanDialog();
    void checkEntry();
    void fillDesc();

private slots:
    void on_bras1CB_currentIndexChanged(int index);
    void on_bras2EnCB_toggled(bool checked);
    void on_bras2CB_currentIndexChanged(int index);
    void on_okBtn_clicked();
    void on_vlan500CB_currentIndexChanged(int index);
    void on_vlan600CB_currentIndexChanged(int index);
    void on_vlan700CB_currentIndexChanged(int index);
    void on_vlan800CB_currentIndexChanged(int index);
    void on_vlanBackBtn_clicked();
    void on_next1Btn_clicked();
    void on_back2Btn_clicked();
    void on_next2Btn_clicked();

private:
    Ui::MA5616SingleAggBrasPlanDialog *ui;
    DanetDbMan *dbMan;
    const int area, dslamId;
    int motherId;
    QString agg, device;

};

#endif // MA5616SingleAggBrasPlanDialog_H
