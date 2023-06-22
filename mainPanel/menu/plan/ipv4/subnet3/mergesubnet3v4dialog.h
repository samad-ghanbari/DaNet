#ifndef MERGESUBNET3V4DIALOG_H
#define MERGESUBNET3V4DIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MergeSubnet3V4Dialog;
}

class MergeSubnet3V4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit MergeSubnet3V4Dialog(QWidget *parent, DanetDbMan *db,const int SubnetPoolId, const QString Abbr, const QString App,const QString Subnet, const int Prefix);
    ~MergeSubnet3V4Dialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::MergeSubnet3V4Dialog *ui;
    DanetDbMan *dbMan;
    const int subnetPoolId, prefix;
    QString subnet, mask,bc, adjacentSubnet, adjacentBc, mSubnet,mMask,mBc;
    int host,adjacentSubnetPoolId,mPrefix, mHost;
};

#endif // MERGESUBNET3V4DIALOG_H
