#ifndef MERGESUBNET1V4DIALOG_H
#define MERGESUBNET1V4DIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MergeSubnet1V4Dialog;
}

class MergeSubnet1V4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit MergeSubnet1V4Dialog(QWidget *parent, DanetDbMan *db,const int SubnetPoolId, const QString Abbr, const QString App,const QString Subnet, const int Prefix);
    ~MergeSubnet1V4Dialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::MergeSubnet1V4Dialog *ui;
    DanetDbMan *dbMan;
    const int subnetPoolId, prefix;
    QString subnet, mask,bc, adjacentSubnet, adjacentBc, mSubnet,mMask,mBc;
    int host,adjacentSubnetPoolId,mPrefix, mHost;
};

#endif // MERGESUBNET1V4DIALOG_H
