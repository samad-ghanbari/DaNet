#ifndef MA5600PLANDIALOG_H
#define MA5600PLANDIALOG_H

#include <QDialog>
#include <QRegExp>


class DanetDbMan;
class QRegExpValidator;

namespace Ui {
class MA5600PlanDialog;
}

class MA5600PlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5600PlanDialog(QWidget *parent, DanetDbMan *Db,const int ExchId, const int DevexId, const QString Name);
    ~MA5600PlanDialog();
    void entryCheck();
    bool entriesCheck();
    bool insertVlan(QString vlanRanges, QString desc);
    bool insertVlan(int vlan, QString desc);
    void fillDslamParam();


private slots:
    void on_okBtn_clicked();
    void on_uplinkShelfCB_toggled(bool checked);
    void on_typeCB_currentIndexChanged(int index);
    void on_desc070LE_textChanged(const QString &arg1);
    void on_desc072LE_textChanged(const QString &arg1);
    void on_desc073LE_textChanged(const QString &arg1);
    void on_gIppCB_currentTextChanged(const QString &arg1);
    void on_mIppCB_currentTextChanged(const QString &arg1);
    void on_desc074LE_textChanged(const QString &arg1);
    void on_desc075LE_textChanged(const QString &arg1);
    void on_snSB_valueChanged(int arg1);
    void on_gIppCB_currentIndexChanged(int index);
    //void on_btvLE_textChanged(const QString &arg1);
    //void on_ipMediaLE_textChanged(const QString &arg1);
    void on_hsiLE_textChanged(const QString &arg1);
    void on_inetLE_textChanged(const QString &arg1);
    void on_sinetLE_textChanged(const QString &arg1);
    void on_voipLE_textChanged(const QString &arg1);
    //void on_ngnLE_textChanged(const QString &arg1);
    void on_serviceNextBtn_clicked();
    void on_nmsNextBtn_clicked();
    void on_nmsBackBtn_clicked();
    void on_descBackBtn_clicked();

private:
    Ui::MA5600PlanDialog *ui;
    DanetDbMan *dbMan;
    const int exchId,devexId;
    const QString name;
    QRegExpValidator *hsiValidator,*inetValidator,*sinetValidator,*voipValidator;//,*ngnValidator; *vasValidator,*ipMediaValidator, *btvValidator,
    QRegExp hsiRegex,inetRegex,sinetRegex,voipRegex;//,ngnRegex; vasRegex,btvRegex,ipMediaRegex,
    int uplinkDevexId,currentShelf, motherId, motherAgg1Id,motherCxId, cascade, node;
    QList<int> vlanList;
    bool ExistPlan, descChanged, ipChanged, dslamNameChanged;
    int oldSN,shelf;
};

#endif // MA5600PLANDIALOG_H
