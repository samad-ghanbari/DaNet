#ifndef REMOVEVLANREFDIALOG_H
#define REMOVEVLANREFDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveVlanRefDialog;
}

class RemoveVlanRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveVlanRefDialog(QWidget *parent, DanetDbMan *db,const int Id,const int vlan,const QString Desc);
    ~RemoveVlanRefDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_removeBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveVlanRefDialog *ui;
    DanetDbMan  *dbMan;
    const int id;
    const QString oldVlan, desc;
};

#endif // REMOVEVLANREFDIALOG_H
