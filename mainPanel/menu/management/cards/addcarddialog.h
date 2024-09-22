#ifndef ADDCARDDIALOG_H
#define ADDCARDDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class AddCardDialog;
}

class AddCardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCardDialog(QWidget *parent, DanetDbMan *db);
    ~AddCardDialog();
    bool inputsNotEmpty();


private slots:
    void on_cancelBtn_clicked();
    void on_typeLE_textChanged();
    void on_okBtn_clicked();
    void on_deviceCB_currentIndexChanged(int index);
    void on_adslSB_valueChanged(int arg1);
    void on_vdslSB_valueChanged(int arg1);
    void on_shdslSB_valueChanged(int arg1);
    void on_eSB_valueChanged(int arg1);
    void on_G1SB_valueChanged(int arg1);
    void on_G10SB_valueChanged(int arg1);
    void on_G40SB_valueChanged(int arg1);
    void on_G100SB_valueChanged(int arg1);
    void on_cardLE_textChanged(const QString &arg1);

private:
    Ui::AddCardDialog *ui;
    DanetDbMan *dbMan;
};

#endif // ADDCARDDIALOG_H
