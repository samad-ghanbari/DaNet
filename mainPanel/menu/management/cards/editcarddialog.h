#ifndef EDITCARDDIALOG_H
#define EDITCARDDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditCardDialog;
}

class EditCardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditCardDialog(QWidget *parent, DanetDbMan *db, int Id, QString card, QString device, QString type, int adsl, int vdsl, int shdsl, int e, int G, int G10, int G40, int G100);
    ~EditCardDialog();
    bool lineEditsNotEmpty();


private slots:
    void on_cancelBtn_clicked();
    void on_cardLE_textChanged();
    void on_typeLE_textChanged();
    void on_modifyBtn_clicked();
    void on_deviceCB_currentIndexChanged(int index);
    void on_adslSB_valueChanged(int arg1);
    void on_vdslSB_valueChanged(int arg1);
    void on_shdslSB_valueChanged(int arg1);
    void on_eSB_valueChanged(int arg1);
    void on_G1SB_valueChanged(int arg1);
    void on_G10SB_valueChanged(int arg1);
    void on_G40SB_valueChanged(int arg1);
    void on_G100SB_valueChanged(int arg1);

private:
    Ui::EditCardDialog *ui;
    DanetDbMan *dbMan;
    const int cardId;
};

#endif // EDITCARDDIALOG_H
