#ifndef EDITVSIREFDIALOG_H
#define EDITVSIREFDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditVsiRefDialog;
}

class EditVsiRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditVsiRefDialog(QWidget *parent, DanetDbMan *db, const int Id, const int vsi, const QString desc);
    ~EditVsiRefDialog();
    void checkEntry();

private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();
    void on_vsiSB_valueChanged(int arg1);
    void on_descTE_textChanged();

private:
    Ui::EditVsiRefDialog *ui;
    DanetDbMan *dbMan;
    const int id;
    const QString oldVSI;
};

#endif // EDITVSIREFDIALOG_H
