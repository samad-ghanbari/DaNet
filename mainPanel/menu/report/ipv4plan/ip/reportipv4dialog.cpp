#include "reportipv4dialog.h"
#include "ui_reportipv4dialog.h"
#include "lib/database/danetdbman.h"
#include "lib/report/ipv4plan/ip/ReportIpV4Plan.h"
#include <QMessageBox>
#include <QSqlQuery>
#include "lib/netTools/nettools.h"
#include <QSqlQueryModel>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFile>

ReportIpV4Dialog::ReportIpV4Dialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ReportIpV4Dialog),
    dbMan(db),
    model(new QSqlQueryModel(this)),
    ipRange("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])"),
    ipRegex("^" + ipRange+ "\\." + ipRange+ "\\." + ipRange+"\\." + ipRange + "$"),
    ipValidator(new QRegExpValidator(ipRegex, this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->netLE->setValidator(ipValidator);
    ui->toolBox->setCurrentIndex(0);

    ui->reportTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->reportTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->reportTV->verticalHeader()->hide();
    ui->reportTV->setModel(model);
    ui->prefixSB->setEnabled(false);

}

ReportIpV4Dialog::~ReportIpV4Dialog()
{
    delete ui;
}

void ReportIpV4Dialog::checkEntry()
{
    bool n = !ui->netLE->text().trimmed().isEmpty() && ui->netLE->hasAcceptableInput();
    bool i = !ui->ipLE->text().trimmed().isEmpty();
    bool d = !ui->descLE->text().trimmed().isEmpty();

    if(n || i || d)
        ui->searchBtn->setEnabled(true);
    else
        ui->searchBtn->setEnabled(false);


    ui->okBtn->setEnabled(false);
    model->clear();
}

void ReportIpV4Dialog::on_searchBtn_clicked()
{
    model->clear();
    subnetInfo.clear();

    QString subnet = ui->netLE->text();
    int prefix = ui->prefixSB->value();


    QString ip = ui->ipLE->text().trimmed();
    QString desc = ui->descLE->text().trimmed();

    QString cond="";
    if(!ip.isEmpty())
        cond= " `ip` LIKE '"+ip+"%' ";
    if(!desc.isEmpty())
    {
        if(cond.isEmpty())
            cond += " `description` LIKE '%"+desc+"%' ";
        else
            cond += " AND `description` LIKE '%"+desc+"%' ";
    }

    if(ui->netLE->hasAcceptableInput())
    {
        subnet = NetTools::netId(subnet, prefix);
        QList<int> subnetPoolNoId = dbMan->getSubnetPoolNoId(subnet, prefix); // 1:id   2:id   3:id
        if( subnetPoolNoId.count() == 2)
        {
            subnetInfo = dbMan->getSubnetPoolInfo( subnetPoolNoId.at(0), subnetPoolNoId.at(1) );

            if(cond.isEmpty())
                cond += " `subnet_pool"+QString::number(subnetPoolNoId.at(0))+"_id`="+QString::number(subnetPoolNoId.at(1))+" ";
            else
                cond += " AND `subnet_pool"+QString::number(subnetPoolNoId.at(0))+"_id`="+QString::number(subnetPoolNoId.at(1))+" ";
        }
        else
        {
            if(cond.isEmpty())
                cond += " `subnet_pool1_id`=-1 ";
            else
                cond += " AND `subnet_pool1_id`=-1 ";
        }
    }


    QSqlQuery *query = dbMan->selectIpPool(cond);
    //`id`, `ip`, `description`, editable

    model->setQuery(*query);


    model->setHeaderData(1, Qt::Horizontal, "IP");
    model->setHeaderData(2, Qt::Horizontal, "Description");
    model->setHeaderData(3, Qt::Horizontal, "Editable");

    ui->reportTV->hideColumn(0);
    ui->reportTV->hideColumn(3);

    if(model->rowCount() > 0)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void ReportIpV4Dialog::on_okBtn_clicked()
{

    QString customInfo = ui->customInfoLE->text().trimmed();
    if(customInfo.isEmpty()) customInfo = "";
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet IP Pool Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {

        if(!path.endsWith(".pdf")) path += ".pdf";

        ReportIpV4Plan reportIpPlan(this,model,subnetInfo, path, customInfo);
        if(reportIpPlan.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportIpPlan.lastError());
        }

        this->close();
    }

}

void ReportIpV4Dialog::on_cancelBtn_clicked()
{
    this->close();
}

void ReportIpV4Dialog::on_netLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->prefixSB->setEnabled(false);
    else
    {
        if(ui->netLE->hasAcceptableInput())
        {
            ui->prefixSB->setEnabled(true);
            ui->netLE->setStyleSheet("QLineEdit{color: rgb(85, 0, 127);}");
        }
        else
        {
            ui->prefixSB->setEnabled(false);
            ui->netLE->setStyleSheet("QLineEdit{color: rgb(175, 0, 0);}");
        }

    }

    checkEntry();
}

void ReportIpV4Dialog::on_ipLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        checkEntry();
    else
        checkEntry();
}

void ReportIpV4Dialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        checkEntry();
    else
        checkEntry();
}

void ReportIpV4Dialog::on_prefixSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

