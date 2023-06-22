#ifndef REMOVEVLANDIALOG_H
#define REMOVEVLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveVlanDialog;
}

class RemoveVlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveVlanDialog(QWidget *parent, DanetDbMan *db, int VlanId, QString Abbr, QString Saloon, QString Device, int Vlan, QString Desc);
    ~RemoveVlanDialog();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveVlanDialog *ui;
    DanetDbMan *dbMan;
    const int vlanId;
};

#endif // REMOVEVLANDIALOG_H
