#include "subnetv4searchdialog.h"
#include "ui_subnetv4searchdialog.h"
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include "lib/database/danetdbman.h"
#include <QtXlsx>

SubnetV4SearchDialog::SubnetV4SearchDialog(QWidget *parent, DanetDbMan *DbMan) :
    QDialog(parent),
    ui(new Ui::SubnetV4SearchDialog),
    dbMan(DbMan),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    //area
    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("All", 1);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem("Karaj", 9);
    ui->areaCB->setCurrentIndex(0);
    ui->areaCB->blockSignals(false);

    ui->searchBtn->setEnabled(false);
    ui->exportBtn->setEnabled(false);

    model->setColumnCount(5);
    model->setHeaderData(0,Qt::Horizontal, "Subnet");
    model->setHeaderData(1,Qt::Horizontal, "Area");
    model->setHeaderData(2,Qt::Horizontal, "Abbr");
    model->setHeaderData(3,Qt::Horizontal, "Application");
    model->setHeaderData(4,Qt::Horizontal, "Description");

    ui->tv->setModel(model);
    ui->tv->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tv->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tv->verticalHeader()->hide();
}

SubnetV4SearchDialog::~SubnetV4SearchDialog()
{
    delete ui;
}

void SubnetV4SearchDialog::checkEntry()
{
    bool SUB = !ui->subnetLE->text().isEmpty();
    bool Area = (ui->areaCB->currentData().toInt()) > 1;
    bool Abb = (ui->abbrCB->currentData().toInt()) > 0;
    bool App = !ui->appLE->text().isEmpty();
    bool Desc = !ui->descLE->text().isEmpty();

    model->clear();
    ui->exportBtn->setEnabled(false);

    if(SUB || Area || Abb || App || Desc)
        ui->searchBtn->setEnabled(true);
    else
        ui->searchBtn->setEnabled(false);
}

void SubnetV4SearchDialog::on_subnetLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        checkEntry();
    else
        checkEntry();
}

void SubnetV4SearchDialog::on_areaCB_currentIndexChanged(int index)
{
    index++;
    ui->abbrCB->clear();

    int area = ui->areaCB->currentData().toInt();
    if(area > 1)
    {
        QSqlQuery *query = dbMan->selectExchAndSites(area);
        int id;
        QString exch;
        ui->abbrCB->setEnabled(true);
        ui->abbrCB->addItem("All", -2);
        while(query->next())
        {
            id = query->value(0).toInt();
            exch = query->value(1).toString();
            ui->abbrCB->addItem(exch, id);
        }
    }
    else
    {
        ui->abbrCB->setEnabled(false);
    }

    checkEntry();

}

void SubnetV4SearchDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void SubnetV4SearchDialog::on_appLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        checkEntry();
    else
        checkEntry();
}

void SubnetV4SearchDialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        checkEntry();
    else
        checkEntry();
}

void SubnetV4SearchDialog::on_searchBtn_clicked()
{
    model->clear();

    QString subnet = ui->subnetLE->text().trimmed();
    int area = ui->areaCB->currentData().toInt();
    int exchId = ui->abbrCB->currentData().toInt();
    QString app = ui->appLE->text().trimmed();
    QString desc = ui->descLE->text().trimmed();

    QString cond;
    if(!subnet.isEmpty())
        cond = "s.subnet LIKE '"+subnet+"%' AND ";

    if(area > 1)
        cond +=" s.area ="+QString::number(area) + " AND ";

    if(exchId > 0)
        cond += "s.exchange_id="+QString::number(exchId)+" AND ";

    if(!app.isEmpty())
        cond += "s.application LIKE '%"+app + "%' AND ";

    if(!desc.isEmpty())
        cond += " s.description LIKE '%"+desc +"%' AND ";

    cond += " 1 ";

    QSqlQuery *query = dbMan->searchSubnet(cond, 1);
    //subnet, area, abbr,app, desc
    int row = 0;
    QString Subnet, Area, Abbr, App, Desc;
    model->setColumnCount(5);
    model->setHeaderData(0,Qt::Horizontal, "Subnet");
    model->setHeaderData(1,Qt::Horizontal, "Area");
    model->setHeaderData(2,Qt::Horizontal, "Abbr");
    model->setHeaderData(3,Qt::Horizontal, "Application");
    model->setHeaderData(4,Qt::Horizontal, "Description");
    QModelIndex index;
    while(query->next())
    {
        Subnet = query->value(0).toString();
        Area = query->value(1).toString();
        Abbr = query->value(2).toString();
        App = query->value(3).toString();
        Desc = query->value(4).toString();

        model->insertRow(row);

        index = model->index(row,0);
        model->setData(index,Subnet);
        index = index.sibling(row,1);
        model->setData(index,Area);
        index = index.sibling(row,2);
        model->setData(index,Abbr);
        index = index.sibling(row,3);
        model->setData(index,App);
        index = index.sibling(row,4);
        model->setData(index,Desc);

        row ++;
    }

    query = dbMan->searchSubnet(cond, 2);
    //subnet, area, abbr,app, desc
    while(query->next())
    {
        Subnet = query->value(0).toString();
        Area = query->value(1).toString();
        Abbr = query->value(2).toString();
        App = query->value(3).toString();
        Desc = query->value(4).toString();

        model->insertRow(row);

        index = model->index(row,0);
        model->setData(index,Subnet);
        index = index.sibling(row,1);
        model->setData(index,Area);
        index = index.sibling(row,2);
        model->setData(index,Abbr);
        index = index.sibling(row,3);
        model->setData(index,App);
        index = index.sibling(row,4);
        model->setData(index,Desc);

        row ++;
    }

    query = dbMan->searchSubnet(cond, 3);
    //subnet, area, abbr,app, desc
    while(query->next())
    {
        Subnet = query->value(0).toString();
        Area = query->value(1).toString();
        Abbr = query->value(2).toString();
        App = query->value(3).toString();
        Desc = query->value(4).toString();

        model->insertRow(row);

        index = model->index(row,0);
        model->setData(index,Subnet);
        index = index.sibling(row,1);
        model->setData(index,Area);
        index = index.sibling(row,2);
        model->setData(index,Abbr);
        index = index.sibling(row,3);
        model->setData(index,App);
        index = index.sibling(row,4);
        model->setData(index,Desc);

        row ++;
    }

    ui->tv->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tv->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);

    if(model->rowCount() > 0)
        ui->exportBtn->setEnabled(true);
    else
        ui->exportBtn->setEnabled(false);
}

void SubnetV4SearchDialog::on_exportBtn_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Save As", "","xlsx (*.xlsx)");
    if(!path.endsWith("xlsx"))
        path += ".xlsx";

    //xsl file : fist create then save
    QXlsx::Document xlsx;
    xlsx.addSheet("DaNet Report");
    xlsx.deleteSheet("sheet1");

    xlsx.setDocumentProperty("title", "Subnet Search Result");
    xlsx.setDocumentProperty("subject", "Search");
    xlsx.setDocumentProperty("creator", "DaNet Software");
    xlsx.setDocumentProperty("company", "DaNet");
    xlsx.setDocumentProperty("category", "Report");
    xlsx.setDocumentProperty("keywords", "Data Network");
    xlsx.setDocumentProperty("description", "Developer Samad Ghanbari");


    QXlsx::Format format;
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format.setBorderColor(Qt::darkBlue);
    format.setFontBold(true);
    format.setFontSize(14);
    format.setTextWarp(true);
    format.setFontColor(Qt::blue);
    format.setPatternBackgroundColor(QColor(200, 200, 230));
    format.setBorderStyle(QXlsx::Format::BorderDouble);

    xlsx.setRowHeight(1, 40);
    xlsx.mergeCells("A1:E1", format);
    xlsx.write("A1", "Data Network Planning Department");

    format.setFontColor(Qt::darkBlue);
    format.setPatternBackgroundColor(QColor(220, 220, 230));

    xlsx.setRowHeight(2, 30);
    xlsx.mergeCells("A2:E2", format);
    xlsx.write("A2", "Subnet Search Result");

    //headers
    format.setFontSize(10);
    format.setFontColor(Qt::darkRed);
    format.setPatternBackgroundColor(QColor(200, 220, 230));
    format.setBorderStyle(QXlsx::Format::BorderHair);
    xlsx.setRowHeight(3, 30);
    xlsx.setColumnWidth(1,50);
    xlsx.write("A3", "Subnet", format);
    xlsx.setColumnWidth(2, 10);
    xlsx.write("B3", "Area", format);
    xlsx.setColumnWidth(3, 15);
    xlsx.write("C3", "Abbr", format);
    xlsx.setColumnWidth(4, 20);
    xlsx.write("D3", "Application", format);
    xlsx.setColumnWidth(5, 40);
    xlsx.write("E3", "Description", format);

    //records
    format.setFontBold(false);
    format.setFontColor(Qt::black);
    format.setPatternBackgroundColor(QColor(255, 255, 255));
    format.setBorderStyle(QXlsx::Format::BorderHair);

    int row;
    QModelIndex idx;
    QString val;
    for(row = 0; row < model->rowCount(); row++)
    {
        idx = model->index(row,0);
        val = model->data(idx).toString();
        xlsx.write("A"+QString::number(row+4),val, format);

        idx = idx.sibling(row,1);
        val = model->data(idx).toString();
        xlsx.write("B"+QString::number(row+4),val, format);

        idx = idx.sibling(row,2);
        val = model->data(idx).toString();
        xlsx.write("C"+QString::number(row+4),val, format);

        idx = idx.sibling(row,3);
        val = model->data(idx).toString();
        xlsx.write("D"+QString::number(row+4),val, format);

        idx = idx.sibling(row,4);
        val = model->data(idx).toString();
        xlsx.write("E"+QString::number(row+4),val, format);

    }



    xlsx.saveAs(path);
}
