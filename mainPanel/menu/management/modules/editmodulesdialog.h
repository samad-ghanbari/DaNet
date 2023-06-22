#ifndef EDITMODULESDIALOG_H
#define EDITMODULESDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditModulesDialog;
}

class EditModulesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditModulesDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString Module, const QString vendor, const double km);
    ~EditModulesDialog();
    void entryCheck();
private slots:
    void on_modLE_textChanged(const QString &arg1);
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();
    void on_vendorLE_textChanged(const QString &arg1);
    void on_kmSB_valueChanged(double arg1);

private:
    Ui::EditModulesDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // EDITMODULESDIALOG_H
