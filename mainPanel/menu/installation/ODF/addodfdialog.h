#ifndef ADDODFDIALOG_H
#define ADDODFDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AddOdfDialog;
}

class AddOdfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddOdfDialog( QWidget *parent,DanetDbMan *db, int exchId, QString areaAbbr, int saloonNo);
    ~AddOdfDialog();
    void setInfoLabel(QString info);
    void clearInfoLable();
    void checkEntry();
    bool addedOrNot();
    int getSaloon();


private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_pppCB_currentIndexChanged(int index);
    void on_posCountSB_valueChanged(int arg1);
    void on_odfNoSB_valueChanged(int arg1);
    void on_odfRowSB_valueChanged(int arg1);
    void on_odfColumnSB_valueChanged(int arg1);
    void on_saloonCB_currentIndexChanged(int index);

private:
    Ui::AddOdfDialog *ui;
    DanetDbMan *dbMan;
    int exchangeId, SaloonNo;
    bool Added;
};

#endif // ADDODFDIALOG_H
