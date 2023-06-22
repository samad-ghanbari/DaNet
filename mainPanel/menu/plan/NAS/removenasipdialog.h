#ifndef REMOVENASIPDIALOG_H
#define REMOVENASIPDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveNasIpDialog;
}

class RemoveNasIpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveNasIpDialog(QWidget *parent, DanetDbMan *db, const int Id, const int Area, const QString BRAS, const QString Nas);
    ~RemoveNasIpDialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::RemoveNasIpDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // REMOVENASIPDIALOG_H
