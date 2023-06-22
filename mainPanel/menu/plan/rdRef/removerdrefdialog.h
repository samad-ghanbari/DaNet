#ifndef REMOVERDREFDIALOG_H
#define REMOVERDREFDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveRdRefDialog;
}

class RemoveRdRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveRdRefDialog(QWidget *parent, DanetDbMan *db,int Id,const QString Rd,const QString Desc);
    ~RemoveRdRefDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_removeBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveRdRefDialog *ui;
    DanetDbMan  *dbMan;
    const int id;
    const QString rd,desc;
};

#endif // REMOVERDREFDIALOG_H
