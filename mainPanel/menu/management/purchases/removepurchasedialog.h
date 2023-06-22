#ifndef REMOVEPURCHASEDIALOG_H
#define REMOVEPURCHASEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemovePurchaseDialog;
}

class RemovePurchaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemovePurchaseDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString abbr, const QString pur);
    ~RemovePurchaseDialog();

private slots:
    void on_CancelBtn_clicked();

    void on_okBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::RemovePurchaseDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // REMOVEPURCHASEDIALOG_H
