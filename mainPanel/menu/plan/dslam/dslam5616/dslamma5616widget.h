#ifndef DSLAMMA5616WIDGET_H
#define DSLAMMA5616WIDGET_H

#include <QWidget>
#include <QMenu>

class DanetDbMan;
class QSqlQueryModel;
class QStringListModel;

namespace Ui {
class DslamMa5616Widget;
}

class DslamMa5616Widget : public QWidget
{
    Q_OBJECT

public:
    explicit DslamMa5616Widget(QWidget *parent, DanetDbMan *Db,const int Area, const int ExchId, const int DslamId);
    ~DslamMa5616Widget();
    void emptyValues();
    void hideGB();
    bool cxAllowance(int val);

private slots:
    void fillForm(int i);
    void fillFormDslamPlan(int i);
    void fillFormAggMetroPlan(int i);
    void fillFormBrasPlan(int i);
    void refreshAggMetroSlot(int i);
    void refreshPortDescSlot(int i);
    void on_dslamPlanBtn_clicked();
    void on_aggMetroPlanBtn_clicked();
    void on_RemoveAggMetroPlanBtn_clicked();
    void on_removeBrasBtn_clicked();
    void on_brasPlanBtn_clicked();
    void on_removePlanBtn_clicked();
    void contextMenuSlot(QPoint ptr);
    void on_actionEdit_Port_Description_triggered();
    void on_dslamAddVlanBtn_clicked();

signals:
    void finished(int i);
    void dslamNameSignal(int i);

private:
    Ui::DslamMa5616Widget *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *portDescModel, *brasModel;
    QStringListModel *agg1AllowPassModel, *agg1IntModel;//*agg1CxAllowPassModel,
    const int area, exchId, dslamId;
    QString dslamName;

    int clickedRow, clickedId;
    QMenu contextMenu;
    QString clickedInterface, clickedDesc;
    bool ZhoneUplink, singleAgg;
};

#endif // DSLAMMA5616WIDGET_H
