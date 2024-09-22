#ifndef EDITETHERTRUNKDIALOG_H
#define EDITETHERTRUNKDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditEtherTrunkDialog;
}

class EditEtherTrunkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditEtherTrunkDialog(QWidget *parent, DanetDbMan *db, int EtherId, QString Abbr, QString Saloon, QString Device, int Ether, QString Desc);
    ~EditEtherTrunkDialog();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void on_descLE_textChanged(const QString &arg1);

    void on_etherSB_valueChanged(int arg1);

private:
    Ui::EditEtherTrunkDialog *ui;
    DanetDbMan * dbMan;
    const int etherId, ether;
    const QString desc;

};

#endif // EDITETHERTRUNKDIALOG_H
