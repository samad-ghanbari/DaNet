#ifndef UNINSTALLEDADDEDITDIALOG_H
#define UNINSTALLEDADDEDITDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class UninstalledAddEditDialog;
}

class UninstalledAddEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UninstalledAddEditDialog(DanetDbMan *db, QWidget *parent = nullptr);
    ~UninstalledAddEditDialog();
    void setValues(int Id, int area, QString exch, QString commodity, int amount, QString letter, QString desc);

private slots:
    void on_okBtn_clicked();

private:
    Ui::UninstalledAddEditDialog *ui;
    DanetDbMan *dbMan;
    int id;
};

#endif // UNINSTALLEDADDEDITDIALOG_H
