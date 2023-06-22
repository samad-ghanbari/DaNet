#ifndef ADDVLANREFDIALOG_H
#define ADDVLANREFDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class AddVlanRefDialog;
}

class AddVlanRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddVlanRefDialog(QWidget *parent, DanetDbMan *db);
    ~AddVlanRefDialog();
    void checkEntry();

private:
    Ui::AddVlanRefDialog *ui;
    DanetDbMan *dbMan;


private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();
    void on_descTE_textChanged();
};

#endif // ADDVLANREFDIALOG_H
