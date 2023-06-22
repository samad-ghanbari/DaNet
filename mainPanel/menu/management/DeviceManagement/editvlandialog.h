#ifndef EDITVLANDIALOG_H
#define EDITVLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditVlanDialog;
}

class EditVlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditVlanDialog(QWidget *parent, DanetDbMan *db, int vlanId, QString Abbr, QString Saloon, QString Device, int Vlan, QString Desc);
    ~EditVlanDialog();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void on_vlanSB_valueChanged(int arg1);

    void on_descLE_textChanged(const QString &arg1);

private:
    Ui::EditVlanDialog *ui;
    DanetDbMan *dbMan;
    const int vlan, vlanId;
    const QString desc;

};

#endif // EDITVLANDIALOG_H
