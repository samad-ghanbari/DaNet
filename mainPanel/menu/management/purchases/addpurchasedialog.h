#ifndef ADDPURCHASEDIALOG_H
#define ADDPURCHASEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AddPurchaseDialog;
}

class AddPurchaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPurchaseDialog(QWidget *parent, DanetDbMan *db);
    ~AddPurchaseDialog();

private slots:
    void on_abbrLE_textChanged(const QString &arg1);

    void on_purLE_textChanged(const QString &arg1);

    void on_CancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::AddPurchaseDialog *ui;
    DanetDbMan *dbMan;
};

#endif // ADDPURCHASEDIALOG_H
