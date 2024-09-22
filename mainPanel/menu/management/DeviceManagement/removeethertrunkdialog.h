#ifndef REMOVEETHERTRUNKDIALOG_H
#define REMOVEETHERTRUNKDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveEtherTrunkDialog;
}

class RemoveEtherTrunkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveEtherTrunkDialog(QWidget *parent, DanetDbMan *db, int EtherId, QString Abbr, QString Saloon, QString Device, int Ether, QString Desc);
    ~RemoveEtherTrunkDialog();

private slots:
    void on_checkBox_toggled(bool checked);

    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::RemoveEtherTrunkDialog *ui;
    DanetDbMan *dbMan;
    const int etherId;
};

#endif // REMOVEETHERTRUNKDIALOG_H
