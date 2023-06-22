#ifndef DSLAMADDNEWVLANDIALOG_H
#define DSLAMADDNEWVLANDIALOG_H

class DanetDbMan;

#include <QDialog>

namespace Ui {
class DslamAddNewVlanDialog;
}

class DslamAddNewVlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DslamAddNewVlanDialog(QWidget *parent, DanetDbMan *db, const int DslamId, const QString DslamName);
    ~DslamAddNewVlanDialog();

private slots:
    void on_confirmChB_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    //void on_ipMediaSB_valueChanged(int arg1);

    void on_hsiSB_valueChanged(int arg1);

private:
    Ui::DslamAddNewVlanDialog *ui;
    DanetDbMan *dbMan;
    const int dslamId;
    const QString dslamName;
    int exchId,motherAgg1Id,motherCxId;
};

#endif // DSLAMADDNEWVLANDIALOG_H
