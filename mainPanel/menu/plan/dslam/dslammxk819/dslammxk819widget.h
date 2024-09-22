#ifndef DSLAMMXK819WIDGET_H
#define DSLAMMXK819WIDGET_H

#include <QWidget>
#include <QMenu>

class DanetDbMan;
class QSqlQueryModel;
class QStringListModel;

namespace Ui {
class DslamMxk819Widget;
}

class DslamMxk819Widget : public QWidget
{
    Q_OBJECT

public:
    explicit DslamMxk819Widget(QWidget *parent, DanetDbMan *Db,const int Area, const int ExchId, const int DslamId);
    ~DslamMxk819Widget();
    void emptyValues();
    void hideGB();
    bool cxAllowance(int val);

signals:
    void finished(int);
    void dslamNameSignal(int);

private slots:
    void refreshAggMetroSlot(int i);
    void refreshPortDescSlot(int i);
    void fillForm(int i);
    void fillFormDslamPlan(int i);
    void fillFormAggPlan(int i);
    void fillFormMetroPlan(int i);
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
    Ui::DslamMxk819Widget *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *portDescModel, *brasModel;
    QStringListModel *cxIntModel,  *agg1IntModel,*agg1AllowPassModel,*agg2IntModel,*agg2AllowPassModel;//*cxAllowPassModel,
    const int area,exchId, dslamId;
    QString dslamName;

    int clickedRow, clickedId;
    QMenu contextMenu;
    QString clickedInterface, clickedDesc;
    bool ZhoneUplink, singleAgg;
};

#endif // DSLAMMXK819WIDGET_H
