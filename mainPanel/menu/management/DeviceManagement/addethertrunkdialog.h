#ifndef ADDETHERTRUNKDIALOG_H
#define ADDETHERTRUNKDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class AddEtherTrunkDialog;
}

class AddEtherTrunkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEtherTrunkDialog(QWidget *parent, DanetDbMan *db,int DevexId, QString Abbr,QString Saloon, QString Device);
    ~AddEtherTrunkDialog();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void on_descLE_textChanged(const QString &arg1);

    void on_etherSB_valueChanged(int arg1);

private:
    Ui::AddEtherTrunkDialog *ui;
    DanetDbMan *dbMan;
    const int devexId;
};

#endif // ADDETHERTRUNKDIALOG_H
