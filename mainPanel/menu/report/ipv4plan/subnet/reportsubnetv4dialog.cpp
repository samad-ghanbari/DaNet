#include "reportsubnetv4dialog.h"
#include "ui_reportsubnetv4dialog.h"
#include "lib/database/danetdbman.h"
#include "lib/report/ipv4plan/subnet/ReportSubnetIpV4Plan.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFile>

ReportSubnetV4Dialog::ReportSubnetV4Dialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ReportSubnetV4Dialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);
    ui->toolBox->setCurrentIndex(0);

    //area
    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("All", 0);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem("Karaj", 9);
    ui->areaCB->setCurrentIndex(-1);
    ui->areaCB->blockSignals(false);


    ui->prefixSB->setEnabled(true);
    ui->prefixSB->setVisible(true);
    ui->prefixLbl->setVisible(false);
    ui->prefixChB->setChecked(true);


    ui->reportTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->reportTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->reportTV->verticalHeader()->hide();
    ui->reportTV->setModel(model);

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

}

ReportSubnetV4Dialog::~ReportSubnetV4Dialog()
{
    delete ui;
}

void ReportSubnetV4Dialog::on_cancelBtn_clicked()
{
    this->close();
}

void ReportSubnetV4Dialog::on_prefixChB_toggled(bool checked)
{
    if(checked)
    {
        ui->prefixSB->setEnabled(true);
        ui->prefixSB->setVisible(true);
        ui->prefixLbl->setVisible(false);
    }
    else
    {
        ui->prefixSB->setEnabled(false);
        ui->prefixSB->setVisible(false);
        ui->prefixLbl->setVisible(true);
    }

    ui->okBtn->setEnabled(false);
    model->clear();
}

void ReportSubnetV4Dialog::on_searchBtn_clicked()
{
    model->clear();

    QString subnet = ui->netLE->text().trimmed();
    int prefix = ui->prefixSB->value();
    int area = ui->areaCB->currentData().toInt();
    int exchId = ui->abbrCB->currentData().toInt();
    QString app = ui->appLE->text();
    QString desc = ui->descLE->text().trimmed();
    subnetPoolNo = -1;
    bool s1 = ui->subnet1RB->isChecked();
    bool s2 = ui->subnet2RB->isChecked();
    bool s3 = ui->subnet3RB->isChecked();
    if(s1)
        subnetPoolNo = 1;
    else if(s2)
        subnetPoolNo = 2;
    else if(s3)
        subnetPoolNo = 3;


    if(!ui->prefixChB->isChecked())
        prefix = 0;

    QString cond = " `subnet` LIKE '"+subnet+"%' ";
    if(prefix > 0)
        cond += " AND `prefix` = "+QString::number(prefix)+" ";
    if(area > 0)
        cond += " AND `area_no` = "+QString::number(area)+" ";
    if(exchId > -1)
        cond += " AND `exchange_id` = "+QString::number(exchId)+" ";
    if(!app.isEmpty())
        cond += " AND `application` like '%"+app+"%' ";
    if(!desc.isEmpty())
        cond += " AND `description` LIKE '%"+desc+"%' ";


    QSqlQuery *query = dbMan->selectSubnetPoolView(subnetPoolNo, cond);

    //pool 1
    //`id`, `subnet`, `prefix`, `mask`, `bc`, `hosts`, `area_no`, `area`, `exchange_id`, `abbr`, `exchange`,`department_no`,`department`, `application`, `description`
    // pool 2,3
    //`id`, `subnet`, `prefix`, `mask`, `bc`, `hosts`, `area_no`, `area`, `exchange_id`, `abbr`, `exchange`, `application`, `description`

    model->setQuery(*query);


    model->setHeaderData(1, Qt::Horizontal, "Subnet");
    model->setHeaderData(2, Qt::Horizontal, "Prefix");
    model->setHeaderData(3, Qt::Horizontal, "Mask");
    model->setHeaderData(4, Qt::Horizontal, "Broadcast");
    model->setHeaderData(5, Qt::Horizontal, "Hosts");
    model->setHeaderData(7, Qt::Horizontal, "Area");
    model->setHeaderData(9, Qt::Horizontal, "Abbr");
    model->setHeaderData(10, Qt::Horizontal, "Exchange");
    if(subnetPoolNo == 1)
    {
        model->setHeaderData(12, Qt::Horizontal, "Department");
        model->setHeaderData(13, Qt::Horizontal, "Application");
        model->setHeaderData(14, Qt::Horizontal, "Description");
        ui->reportTV->hideColumn(11);
    }
    else
    {
        model->setHeaderData(11, Qt::Horizontal, "Application");
        model->setHeaderData(12, Qt::Horizontal, "Description");
    }

    ui->reportTV->hideColumn(0);
    ui->reportTV->hideColumn(6);
    ui->reportTV->hideColumn(8);

    if(model->rowCount() > 0)
        ui->okBtn->setEnabled(true);
}

void ReportSubnetV4Dialog::on_areaCB_currentIndexChanged(int index)
{
    index++;
    int area = ui->areaCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectExchAndSites(area);
    ui->abbrCB->clear();
    ui->abbrCB->addItem("All", -1);
    int id;
    QString name;
    while(query->next())
    {
        id = query->value(0).toInt();
        name = query->value(1).toString();
        ui->abbrCB->addItem(name, id);
    }

    ui->okBtn->setEnabled(false);
}

void ReportSubnetV4Dialog::on_okBtn_clicked()
{

    QString customInfo = ui->customInfoLE->text().trimmed();
    if(customInfo.isEmpty()) customInfo = "";

    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Subnet Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {

        if(!path.endsWith(".pdf")) path += ".pdf";

        ReportSubnetIpV4Plan reportNetworkIpPlan(this,model,subnetPoolNo, path, customInfo);
        if(reportNetworkIpPlan.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportNetworkIpPlan.lastError());
        }

        this->close();
    }

}

void ReportSubnetV4Dialog::on_subnet1RB_toggled(bool checked)
{
    if(checked)
        model->clear();
    else
        model->clear();
    ui->okBtn->setEnabled(false);
}

void ReportSubnetV4Dialog::on_subnet2RB_toggled(bool checked)
{
    if(checked)
        model->clear();
    else
        model->clear();
    ui->okBtn->setEnabled(false);
}

void ReportSubnetV4Dialog::on_subnet3RB_toggled(bool checked)
{
    if(checked)
        model->clear();
    else
        model->clear();
    ui->okBtn->setEnabled(false);
}

void ReportSubnetV4Dialog::on_netLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        model->clear();
    else
        model->clear();

    ui->okBtn->setEnabled(false);
}

void ReportSubnetV4Dialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    ui->okBtn->setEnabled(false);
    model->clear();
}

void ReportSubnetV4Dialog::on_appLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        model->clear();
    else
        model->clear();

    ui->okBtn->setEnabled(false);
}

void ReportSubnetV4Dialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        model->clear();
    else
        model->clear();

    ui->okBtn->setEnabled(false);
}

void ReportSubnetV4Dialog::on_prefixSB_valueChanged(int arg1)
{
    arg1++;
    model->clear();
    ui->okBtn->setEnabled(false);
}

