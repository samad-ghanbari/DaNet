#ifndef REPORTTOTALDSLAMPLANDIALOG_H
#define REPORTTOTALDSLAMPLANDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;
class QSqlQueryModel;
class QStringListModel;
class QStandardItemModel;

namespace Ui {
class ReportTotalDSLAMPlanDialog;
}

class ReportTotalDSLAMPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportTotalDSLAMPlanDialog(QWidget *parent, DanetDbMan *db);
    ~ReportTotalDSLAMPlanDialog();
    void emptyValues();
    void fillForm();
    void fillFormDslamPlan();
    void fillFormAggPlan();
    void fillFormMetroPlan();
    void fillFormBrasPlan();
    void hideGB();
    bool cxAllowance(int val);
    void initIntModel();
    void initOdfModel();
    void initTopoModel();
    QString uplinkIntraVlan(QString vlanRange, int vlan);


private slots:
    void on_okBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_dslamCB_currentIndexChanged(int index);

private:
    Ui::ReportTotalDSLAMPlanDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *portDescModel, *brasModel;
    QStringListModel *cxIntModel,  *agg1IntModel,*agg1AllowPassModel,*agg2IntModel,*agg2AllowPassModel;//*cxAllowPassModel,
    QSqlQueryModel *intModel, *odfModel,*deviceModel;
    QStandardItemModel  *topoModel ;
    int dslamId;

    QString dslamName;
    QMap<int, QString> serviceMap;//"100", "100-102"
    QMap<QString, QString> portMap;//     "0/7/0","To..."
    QMap<int, QStringList> nmsMap;// 10, ip<<gw
    QList<int> interfaceIds;
    QStringList agg1List, agg2List, aggInfo;// agg1 << ether <<port1<<port2 / vlan << desc
    QStringList bras1List, bras2List;// bras << sw1 <<sw2
    QMap<int, QString> brasInfo;// 500, "To..."
    QStringList cxList;// cx << port1 << port2 <<vlan list << desc
    //QStringList agg1CxList; // << agg1-ether << cx-eth << vlan
    bool ZhoneUplink;
};

#endif // REPORTTOTALDSLAMPLANDIALOG_H
