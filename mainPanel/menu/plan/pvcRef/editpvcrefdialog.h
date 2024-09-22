#ifndef EDITPVCREFDIALOG_H
#define EDITPVCREFDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditPvcRefDialog;
}

class EditPvcRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditPvcRefDialog(QWidget *parent, DanetDbMan *db, const int Id, const int vpi,const int vci, const QString desc);
    ~EditPvcRefDialog();
    void checkEntry();

private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();
    void on_descTE_textChanged();

    void on_vpiSB_valueChanged(int arg1);

    void on_vciSB_valueChanged(int arg1);

private:
    Ui::EditPvcRefDialog *ui;
    DanetDbMan *dbMan;
    const int id;
    QString oldPvc;
};

#endif // EDITPVCREFDIALOG_H
