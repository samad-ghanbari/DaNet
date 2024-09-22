#ifndef SPLITSUBNET1V4DIALOG_H
#define SPLITSUBNET1V4DIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class SplitSubnet1V4Dialog;
}

class SplitSubnet1V4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit SplitSubnet1V4Dialog(QWidget *parent, DanetDbMan *db,const int NetPoolId, const int SubnetPoolId, const QString Abbr, const QString Network, const int Prefix, const int Area,const int Dep, const QString Ap, const QString Desc );
    ~SplitSubnet1V4Dialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::SplitSubnet1V4Dialog *ui;
    DanetDbMan *dbMan;
    const int netPoolId, subnetPoolId, area, prefix, dep;
    const QString ap,desc;
    QString net,mask,bc;
    QString sub1, sub2,mask1,bc1, bc2;
    int prefix1, host, host1;
};

#endif // SPLITSUBNET1V4DIALOG_H
