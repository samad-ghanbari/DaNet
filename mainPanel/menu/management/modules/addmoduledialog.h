#ifndef ADDMODULEDIALOG_H
#define ADDMODULEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AddModuleDialog;
}

class AddModuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddModuleDialog(QWidget *parent, DanetDbMan *db);
    ~AddModuleDialog();
    void entryCheck();

private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();
    void on_modLE_textChanged(const QString &arg1);
    void on_vendorLE_textChanged(const QString &arg1);
    void on_kmSB_valueChanged(double arg1);

private:
    Ui::AddModuleDialog *ui;
    DanetDbMan *dbMan;
};

#endif // ADDMODULEDIALOG_H
