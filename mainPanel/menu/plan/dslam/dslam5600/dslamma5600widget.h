#ifndef DSLAMMA5600WIDGET_H
#define DSLAMMA5600WIDGET_H

#include <QWidget>
#include <QMenu>

class DanetDbMan;
class QSqlQueryModel;
class QStringListModel;

namespace Ui {
class DslamMa5600Widget;
}

class DslamMa5600Widget : public QWidget
{
    Q_OBJECT

public:
    explicit DslamMa5600Widget(QWidget *parent, DanetDbMan *Db,const int Area,const int ExchId,const int DslamId);
    ~DslamMa5600Widget();
    void emptyValues();
    void hideGB();
    bool cxAllowance(int val);

signals:
    void finished(int i);
    void dslamNameSignal(int i);

private slots:
    void fillForm(int i);
    void refreshAggMetroSlot(int i);
    void refreshPortDescSlot(int i);
    void fillFormDslamPlan(int i);
    void fillFormAggPlan();
    void fillFormMetroPlan();
    void fillFormBrasPlan(int i);
    void on_dslamPlanBtn_clicked();
    void on_brasPlanBtn_clicked();
    void on_removePlanBtn_clicked();
    void on_aggMetroPlanBtn_clicked();
    void on_RemoveAggMetroPlanBtn_clicked();
    void on_removeBrasBtn_clicked();
    void contextMenuSlot(QPoint ptr);
    void on_actionEdit_Port_Description_triggered();

    void on_dslamAddVlanBtn_clicked();

private:
    Ui::DslamMa5600Widget *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *portDescModel, *brasModel;
    QStringListModel *cxIntModel,  *agg1IntModel,*agg1AllowPassModel,*agg2IntModel,*agg2AllowPassModel;//*cxAllowPassModel,
    const int area, exchId, dslamId;
    QString dslamName;


    int clickedRow, clickedId;
    QMenu contextMenu;
    QString clickedInterface, clickedDesc;
};

#endif // DSLAMMA5600WIDGET_H
