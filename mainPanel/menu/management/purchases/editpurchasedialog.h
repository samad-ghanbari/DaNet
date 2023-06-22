#ifndef EDITPURCHASEDIALOG_H
#define EDITPURCHASEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditPurchaseDialog;
}

class EditPurchaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditPurchaseDialog(QWidget *parent,DanetDbMan *db, const int Id, const QString Abbr, QString Pur);
    ~EditPurchaseDialog();

private slots:
    void on_CancelBtn_clicked();

    void on_okBtn_clicked();
    void on_abbrLE_textChanged(const QString &arg1);
    void on_purLE_textChanged(const QString &arg1);

private:
    Ui::EditPurchaseDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // EDITPURCHASEDIALOG_H
