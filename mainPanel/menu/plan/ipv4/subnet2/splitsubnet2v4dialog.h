#ifndef SPLITSUBNET2V4DIALOG_H
#define SPLITSUBNET2V4DIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class SplitSubnet2V4Dialog;
}

class SplitSubnet2V4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit SplitSubnet2V4Dialog(QWidget *parent, DanetDbMan *db,const int SubnetPool1Id, const int SubnetPool2Id, const QString Abbr, const QString Network, const int Prefix, const int Area, const QString Ap, const QString Desc );
    ~SplitSubnet2V4Dialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::SplitSubnet2V4Dialog *ui;
    DanetDbMan *dbMan;
    const int subnetPool1Id, subnetPool2Id, area, prefix;
    const QString ap,desc;
    QString net,mask,bc;
    QString sub1, sub2,mask1,bc1, bc2;
    int prefix1, host, host1;
};

#endif // SPLITSUBNET2V4DIALOG_H
