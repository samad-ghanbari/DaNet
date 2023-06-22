#ifndef MERGESUBNET2V4DIALOG_H
#define MERGESUBNET2V4DIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MergeSubnet2V4Dialog;
}

class MergeSubnet2V4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit MergeSubnet2V4Dialog(QWidget *parent, DanetDbMan *db,const int SubnetPoolId, const QString Abbr, const QString App,const QString Subnet, const int Prefix);
    ~MergeSubnet2V4Dialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::MergeSubnet2V4Dialog *ui;
    DanetDbMan *dbMan;
    const int subnetPoolId, prefix;
    QString subnet, mask,bc, adjacentSubnet, adjacentBc, mSubnet,mMask,mBc;
    int host,adjacentSubnetPoolId,mPrefix, mHost;
};

#endif // MERGESUBNET2V4DIALOG_H
