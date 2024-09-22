#include "reportnetworkv4dialog.h"
#include "ui_reportnetworkv4dialog.h"
#include "lib/database/danetdbman.h"
#include "lib/report/ipv4plan/net/ReportNetworkIpV4Plan.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFile>

ReportNetworkV4Dialog::ReportNetworkV4Dialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ReportNetworkV4Dialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

ui->toolBox->setCurrentIndex(0);

    ui->prefixSB->setEnabled(true);
    ui->prefixSB->setVisible(true);
    ui->prefixLbl->setVisible(false);
    ui->prefixChB->setChecked(true);


    ui->reportTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->reportTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->reportTV->verticalHeader()->hide();
    ui->reportTV->setModel(model);
}

ReportNetworkV4Dialog::~ReportNetworkV4Dialog()
{
    delete ui;
}

void ReportNetworkV4Dialog::on_okBtn_clicked()
{
    QString customInfo = ui->customInfoLE->text().trimmed();
    if(customInfo.isEmpty()) customInfo = "";

    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Network Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {

        if(!path.endsWith(".pdf")) path += ".pdf";

        ReportNetworkIpV4Plan reportNetworkIpPlan(this,model, path, customInfo);
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

void ReportNetworkV4Dialog::on_cancelBtn_clicked()
{
    this->close();

}

void ReportNetworkV4Dialog::on_prefixChB_toggled(bool checked)
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

    model->clear();
    ui->okBtn->setEnabled(false);
}

void ReportNetworkV4Dialog::on_searchBtn_clicked()
{
    model->clear();

    QString net = ui->netLE->text().trimmed();
    int prefix = ui->prefixSB->value();
    QString desc = ui->descLE->text().trimmed();

    if(!ui->prefixChB->isChecked())
        prefix = 0;

    QString cond = " `net` LIKE '"+net+"%' ";
    if(prefix > 0)
        cond += " AND `prefix`="+QString::number(prefix)+" ";
    if(!desc.isEmpty())
        cond += " AND `description` LIKE '%"+desc+"%' ";


    QSqlQuery *query = dbMan->selectNetPool(cond);
//0`id`, 1`net`, 2`prefix`, 3`mask`, 4`bc`, 5`hosts`, 6`description`

    model->setQuery(*query);


    model->setHeaderData(1, Qt::Horizontal, "Network");
    model->setHeaderData(2, Qt::Horizontal, "Prefix");
    model->setHeaderData(3, Qt::Horizontal, "Mask");
    model->setHeaderData(4, Qt::Horizontal, "Broadcast");
    model->setHeaderData(5, Qt::Horizontal, "Hosts");
    model->setHeaderData(6, Qt::Horizontal, "Description");

    ui->reportTV->hideColumn(0);

    if(model->rowCount() > 0)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);


}

void ReportNetworkV4Dialog::on_netLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        model->clear();
    else
        model->clear();
    ui->okBtn->setEnabled(false);
}

void ReportNetworkV4Dialog::on_prefixSB_valueChanged(int arg1)
{
    arg1++;
    model->clear();
    ui->okBtn->setEnabled(false);
}

void ReportNetworkV4Dialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        model->clear();
    else
        model->clear();
    ui->okBtn->setEnabled(false);
}

