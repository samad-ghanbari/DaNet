#ifndef REMOVEPVCREFDIALOG_H
#define REMOVEPVCREFDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemovePvcRefDialog;
}

class RemovePvcRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemovePvcRefDialog(QWidget *parent, DanetDbMan *db,const int Id,const int vpi,const int vci,QString desc);
    ~RemovePvcRefDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_removeBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::RemovePvcRefDialog *ui;
    DanetDbMan  *dbMan;
    int id;
};

#endif // REMOVEPVCREFDIALOG_H
