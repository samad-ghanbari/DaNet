#ifndef REMOVEVSIREFDIALOG_H
#define REMOVEVSIREFDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveVsiRefDialog;
}

class RemoveVsiRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveVsiRefDialog(QWidget *parent, DanetDbMan *db,int Id,const int Vsi,const QString Desc);
    ~RemoveVsiRefDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_removeBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveVsiRefDialog *ui;
    DanetDbMan  *dbMan;
    const int id;
    const QString vsi, desc;
};

#endif // REMOVEVSIREFDIALOG_H
