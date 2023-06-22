#ifndef TELEPHONEADDEDITDIALOG_H
#define TELEPHONEADDEDITDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class TelephoneAddEditDialog;
}

class TelephoneAddEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TelephoneAddEditDialog(DanetDbMan *db, QWidget *parent = nullptr);
    ~TelephoneAddEditDialog();
    void setValues(int Id,QString name, QString dep, QString pos, QString tel);

private slots:
    void on_okBtn_clicked();

private:
    Ui::TelephoneAddEditDialog *ui;
    DanetDbMan *dbMan;
    int id;
};

#endif // TELEPHONEADDEDITDIALOG_H
