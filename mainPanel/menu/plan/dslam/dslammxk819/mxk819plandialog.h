#ifndef MXK819PLANDIALOG_H
#define MXK819PLANDIALOG_H

#include <QDialog>
#include <QRegExp>

class DanetDbMan;
class QRegExpValidator;

namespace Ui {
class Mxk819PlanDialog;
}

class Mxk819PlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Mxk819PlanDialog(QWidget *parent, DanetDbMan *Db,const int ExchId, const int DevexId, const QString Name);
    ~Mxk819PlanDialog();
    void entryCheck();
    bool entriesCheck();
    bool insertVlan(QString vlanRanges, QString desc);
    bool insertVlan(int vlan, QString desc);
    void fillDslamParam();


private slots:
    void on_okBtn_clicked();
    void on_uplinkShelfCB_toggled(bool checked);
    void on_typeCB_currentIndexChanged(int index);
    void on_desc1a4LE_textChanged(const QString &arg1);
    void on_desc1a5LE_textChanged(const QString &arg1);
    void on_desc1a6LE_textChanged(const QString &arg1);
    void on_gIppCB_currentTextChanged(const QString &arg1);
    void on_mIppCB_currentTextChanged(const QString &arg1);
    void on_desc1a7LE_textChanged(const QString &arg1);
    void on_desc1a8LE_textChanged(const QString &arg1);
    void on_desc1a9LE_textChanged(const QString &arg1);
    void on_snSB_valueChanged(int arg1);
    void on_gIppCB_currentIndexChanged(int index);
    //void on_btvLE_textChanged(const QString &arg1);

    //void on_ipMediaLE_textChanged(const QString &arg1);

    void on_hsiLE_textChanged(const QString &arg1);

    void on_inetLE_textChanged(const QString &arg1);

    void on_sinetLE_textChanged(const QString &arg1);

    void on_voipLE_textChanged(const QString &arg1);

    //void on_ngnLE_textChanged(const QString &arg1);

    void on_desc1a2LE_textChanged(const QString &arg1);

    void on_desc1a3LE_textChanged(const QString &arg1);
    void on_nmsNextBtn_clicked();
    void on_nmsBackBtn_clicked();
    void on_dslamNext_clicked();
    void on_desc7BackBtn_clicked();

    void on_desc7NextBtn_clicked();

    void on_desc8BackBtn_clicked();

    void on_desc1a10LE_textChanged(const QString &arg1);

    void on_desc1a11LE_textChanged(const QString &arg1);

    void on_desc1b2LE_textChanged(const QString &arg1);

    void on_desc1b3LE_textChanged(const QString &arg1);

    void on_desc1b4LE_textChanged(const QString &arg1);

    void on_desc1b5LE_textChanged(const QString &arg1);

    void on_desc1b6LE_textChanged(const QString &arg1);

    void on_desc1b7LE_textChanged(const QString &arg1);

    void on_desc1b8LE_textChanged(const QString &arg1);

    void on_desc1b9LE_textChanged(const QString &arg1);

    void on_desc1b10LE_textChanged(const QString &arg1);

    void on_desc1b11LE_textChanged(const QString &arg1);

private:
    Ui::Mxk819PlanDialog *ui;
    DanetDbMan *dbMan;
    const int exchId,devexId;
    const QString name;
    QRegExpValidator *hsiValidator,*inetValidator,*sinetValidator,*voipValidator;//,*ngnValidator;//*vasValidator,vasRegex, *btvValidator,*ipMediaValidator,
    QRegExp hsiRegex,inetRegex,sinetRegex,voipRegex;//,ngnRegex; btvRegex,ipMediaRegex,
    int uplinkDevexId,currentShelf, motherId, motherAgg1Id,motherCxId, cascade, node;
    QList<int> vlanList;
    bool ExistPlan, descChanged, ipChanged, dslamNameChanged;
    int oldSN, shelf;
};

#endif // MXK819PLANDIALOG_H
