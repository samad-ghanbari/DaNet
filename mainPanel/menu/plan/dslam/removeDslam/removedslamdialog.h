#ifndef REMOVEDSLAMDIALOG_H
#define REMOVEDSLAMDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;
class QSqlQueryModel;
class QStringListModel;
class QStandardItemModel;

namespace Ui {
class RemoveDSLAMDialog;
}

class RemoveDSLAMDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveDSLAMDialog(QWidget *parent, DanetDbMan *db);
    ~RemoveDSLAMDialog();
    void emptyValues();
    void fillForm();
    void fillFormDslamPlan();
    void fillFormAggPlan();
    void fillFormMetroPlan();
    void fillFormBrasPlan();
    void hideGB();
    bool cxAllowance(int val);
    QString uplinkIntraVlan(QString vlanRange, int vlan);
    void initTopoModel();

private slots:
    void on_okBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_dslamCB_currentIndexChanged(int index);

private:
    Ui::RemoveDSLAMDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *portDescModel, *deviceModel, *brasModel;
    QStringListModel *cxIntModel, *cxAllowPassModel, *agg1IntModel,*agg1AllowPassModel,*agg2IntModel,*agg2AllowPassModel;
    QStandardItemModel  *topoModel ;
    int dslamId;
    bool singleAgg;

    QString dslamName;
    bool ZhoneUplink;
    QMap<int, QString> serviceMap;//"100", "100-102"
    QMap<QString, QString> portMap;//     "0/7/0","To..."
    QMap<int, QStringList> nmsMap;// 10, ip<<gw
    QStringList agg1List, agg2List, aggInfo;// agg1 << ether <<port1<<port2 / vlan << desc
    QStringList Agg1, Agg2, Cx;// Agg/cx << eth << int1 << int2 << int1Id << int2Id
                               // Agg1 << eth << int1 << int2 << int3 << int4 << int1Id << int2Id << int3Id << int4Id
    QStringList bras1List, bras2List;// bras << sw1 <<sw2
    QMap<int, QString> brasInfo;// 500, "To..."
    QStringList cxList;// cx << port1 << port2 <<vlan list << desc
    QStringList agg1CxList; // << agg1-ether << cx-eth << vlan
};

#endif // REMOVEDSLAMDIALOG_H
