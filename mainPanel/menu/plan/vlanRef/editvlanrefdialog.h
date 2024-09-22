#ifndef EDITVLANREFDIALOG_H
#define EDITVLANREFDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditVlanRefDialog;
}

class EditVlanRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditVlanRefDialog(QWidget *parent, DanetDbMan *db, const int Id, const int vlan, const QString desc);
    ~EditVlanRefDialog();
    void checkEntry();

private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();
    void on_vlanSB_valueChanged(int arg1);

    void on_descTE_textChanged();

private:
    Ui::EditVlanRefDialog *ui;
    DanetDbMan *dbMan;
    const int id;
    const QString oldVlan;
};

#endif // EDITVLANREFDIALOG_H
