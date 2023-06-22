#ifndef MA5600BRASPLANDIALOG_H
#define MA5600BRASPLANDIALOG_H

#include <QDialog>
class DanetDbMan;


namespace Ui {
class MA5600BrasPlanDialog;
}

class MA5600BrasPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5600BrasPlanDialog(QWidget *parent, DanetDbMan *Db, const int Area, const int ExchId, const int DslamId, const QString dslamName );
    ~MA5600BrasPlanDialog();
    void checkEntry();
    void fillDesc();

private slots:
    void on_bras1CB_currentIndexChanged(int index);
    void on_bras2CB_currentIndexChanged(int index);
    void on_bras2EnCB_toggled(bool checked);
    void on_okBtn_clicked();
    void on_vlan500CB_currentIndexChanged(int index);
    void on_vlan600CB_currentIndexChanged(int index);
    void on_vlan700CB_currentIndexChanged(int index);
    void on_vlan800CB_currentIndexChanged(int index);

    void on_back3Btn_clicked();

    void on_next2Btn_clicked();

    void on_back2Btn_clicked();

    void on_next1Btn_clicked();

private:
    Ui::MA5600BrasPlanDialog *ui;
    DanetDbMan *dbMan;
    const int area, dslamId;
    int motherId;
    QString agg, device;
};

#endif // MA5600BRASPLANDIALOG_H
