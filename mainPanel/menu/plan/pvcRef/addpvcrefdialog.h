#ifndef ADDPVCREFDIALOG_H
#define ADDPVCREFDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class AddPvcRefDialog;
}

class AddPvcRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPvcRefDialog(QWidget *parent, DanetDbMan *db);
    ~AddPvcRefDialog();
    void checkEntry();

private:
    Ui::AddPvcRefDialog *ui;
    DanetDbMan *dbMan;


private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();
    void on_vpiSB_valueChanged(int arg1);
    void on_vciSB_valueChanged(int arg1);
    void on_descTE_textChanged();
};

#endif // ADDPVCREFDIALOG_H
