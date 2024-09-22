#ifndef MA5616PLANDIALOG_H
#define MA5616PLANDIALOG_H

#include <QDialog>
#include <QRegExp>

class DanetDbMan;
class QRegExpValidator;

namespace Ui {
class MA5616PlanDialog;
}

class MA5616PlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5616PlanDialog(QWidget *parent,DanetDbMan *Db, const int ExchId,const int DslamId,const QString Name);
    ~MA5616PlanDialog();
    void entryCheck();
    bool entriesCheck();
    bool insertVlan(QString vlanRanges, QString desc);
    bool insertVlan(int vlan, QString desc);
    void fillDslamParam();

private slots:
    void on_typeCB_currentIndexChanged(int index);
    void on_snSB_valueChanged(int arg1);
    void on_desc0LE_textChanged(const QString &arg1);
    void on_desc1LE_textChanged(const QString &arg1);
    void on_okBtn_clicked();
    void on_uplinkShelfCB_toggled(bool checked);
    void on_gIppCB_currentTextChanged(const QString &arg1);
    void on_mIppCB_currentTextChanged(const QString &arg1);
    void on_gIppCB_currentIndexChanged(int index);
    //void on_btvLE_textChanged(const QString &arg1);
    //void on_ipMediaLE_textChanged(const QString &arg1);
    void on_hsiLE_textChanged(const QString &arg1);
    void on_inetLE_textChanged(const QString &arg1);
    void on_sinetLE_textChanged(const QString &arg1);
    void on_voipLE_textChanged(const QString &arg1);
    //void on_ngnLE_textChanged(const QString &arg1);

    void on_dslamNextBtn_clicked();

    void on_descBackBtn_clicked();

    void on_nmsNextBtn_clicked();

    void on_nmsBackBtn_clicked();

private:
    Ui::MA5616PlanDialog *ui;
    DanetDbMan *dbMan;
    const int exchId;
    const int dslamId;
    const QString name;//*vasValidator,vasRegex,
    QRegExpValidator *hsiValidator,*inetValidator,*sinetValidator,*voipValidator;//,*ngnValidator; *btvValidator,*ipMediaValidator,
    QRegExp hsiRegex,inetRegex,sinetRegex,voipRegex;//,ngnRegex; btvRegex,ipMediaRegex,
    int motherId,cascade, node,motherAgg1Id,motherCxId;
    QList<int> vlanList;
    bool ExistPlan, descChanged, ipChanged, dslamNameChanged;
    int oldSN;
};

#endif // MA5616PLANDIALOG_H
