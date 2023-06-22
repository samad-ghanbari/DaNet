#ifndef ADDVLANDIALOG_H
#define ADDVLANDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class AddVlanDialog;
}

class AddVlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddVlanDialog(QWidget *parent, DanetDbMan *db,int DevexId, QString Abbr,QString Saloon, QString Device);
    ~AddVlanDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_descLE_textChanged(const QString &arg1);

    void on_vlanSB_valueChanged(int arg1);

private:
    Ui::AddVlanDialog *ui;
    DanetDbMan *dbMan;
    const int devexId;
};

#endif // ADDVLANDIALOG_H
