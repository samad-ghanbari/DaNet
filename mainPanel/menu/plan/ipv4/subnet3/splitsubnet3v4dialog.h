#ifndef SPLITSUBNET3V4DIALOG_H
#define SPLITSUBNET3V4DIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class SplitSubnet3V4Dialog;
}

class SplitSubnet3V4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit SplitSubnet3V4Dialog(QWidget *parent, DanetDbMan *db,const int SubnetPool2Id, const int SubnetPool3Id, const QString Abbr, const QString Network, const int Prefix, const int Area, const QString Ap, const QString Desc );
    ~SplitSubnet3V4Dialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::SplitSubnet3V4Dialog *ui;
    DanetDbMan *dbMan;
    const int subnetPool2Id, subnetPool3Id, area, prefix;
    const QString ap,desc;
    QString net,mask,bc;
    QString sub1, sub2,mask1,bc1, bc2;
    int prefix1, host, host1;
};

#endif // SPLITSUBNET3V4DIALOG_H
