#ifndef MXK819BRASPLANDIALOG_H
#define MXK819BRASPLANDIALOG_H

#include <QDialog>
class DanetDbMan;


namespace Ui {
class Mxk819BrasPlanDialog;
}

class Mxk819BrasPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Mxk819BrasPlanDialog(QWidget *parent, DanetDbMan *Db, const int Area, const int ExchId, const int DslamId, const QString dslamName);
    ~Mxk819BrasPlanDialog();
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

    void on_vlanBackBtn_clicked();

    void on_next1Btn_clicked();

    void on_Back2Btn_clicked();

    void on_next2Btn_clicked();

private:
    Ui::Mxk819BrasPlanDialog *ui;
    DanetDbMan *dbMan;
    const int area, dslamId;
    int motherId;
    QString agg, device;
};

#endif // MXK819BRASPLANDIALOG_H
