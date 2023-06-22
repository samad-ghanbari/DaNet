#include "exportexceldialog.h"
#include "ui_exportexceldialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QtXlsx>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDesktopServices>

//`int_id`, `Area`, `Abbr`, `Saloon_name`, `Device`, `Interface_type`, `Interface`,
//`Module`,`Label`, `peer_Abbr`, `peer_Device`, `peer_Interface`, `peer_Label`,
//`Ether_trunk`, `Odf_no`, `Pos_no` `Pin_no`

#define INT_ID 0
#define AREA 1
#define ABBR 2
#define SALOON 3
#define DEVICE 4
#define INT_TYPE 5
#define INT 6
#define MODULE 7
#define LABEL 8
#define PEER_ABBR 9
#define PEER_DEV 10
#define PEER_INT 11
#define PEER_LBL 12
#define ETH 13
#define ODF 14
#define POS 15
#define PIN 16

ExportExcelDialog::ExportExcelDialog(QWidget *parent, DanetDbMan *db, const QList<int> IntIds) :
    QDialog(parent),
    ui(new Ui::ExportExcelDialog),
    dbMan(db),
    intIdList(IntIds),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    QString condition = "int_id IN(";
    bool start = true;
    for(int i=0; i < intIdList.count(); i++)
    {
        if(start)
        {
            condition += QString::number(intIdList.at(i));
            start = false;
        }
        else
        {
            condition += ", "+QString::number(intIdList.at(i));
        }
    }
    condition += ");";

    QSqlQuery *query = dbMan->selectViewInterfaceOdf(condition);
    model->setQuery(*query);
    model->setHeaderData(INT_ID,Qt::Horizontal,"int ID");
    model->setHeaderData(AREA,Qt::Horizontal,"Area");
    model->setHeaderData(ABBR,Qt::Horizontal,"Abbr");
    model->setHeaderData(SALOON,Qt::Horizontal,"Saloon\nName");
    model->setHeaderData(DEVICE,Qt::Horizontal,"Device");
    model->setHeaderData(INT_TYPE,Qt::Horizontal,"Interface\nType");
    model->setHeaderData(INT,Qt::Horizontal,"Interface");
    model->setHeaderData(MODULE,Qt::Horizontal,"Module");
    model->setHeaderData(LABEL,Qt::Horizontal,"Label");
    model->setHeaderData(PEER_ABBR,Qt::Horizontal,"Peer\nAbbr");
    model->setHeaderData(PEER_DEV,Qt::Horizontal,"Peer\nDevice");
    model->setHeaderData(PEER_INT,Qt::Horizontal,"Peer\nInterface");
    model->setHeaderData(PEER_LBL,Qt::Horizontal,"Peer\nLabel");
    model->setHeaderData(ETH,Qt::Horizontal,"Ether\nTrunk");
    model->setHeaderData(ODF,Qt::Horizontal,"ODF");
    model->setHeaderData(POS,Qt::Horizontal,"POS");
    model->setHeaderData(PIN,Qt::Horizontal,"PIN");

    ui->tv->setModel(model);
    ui->tv->verticalHeader()->hide();
    ui->tv->hideColumn(INT_ID);
    ui->tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    int row = model->rowCount();
    if(row > 500)
    {
        ui->confirmChB->setVisible(true);
        ui->confirmChB->setChecked(false);
        ui->confirmLbl->setVisible(true);
        ui->okBtn->setEnabled(false);
    }
    else
    {
        ui->confirmChB->setVisible(false);
        ui->confirmLbl->setVisible(false);
        ui->okBtn->setEnabled(true);
    }

}

ExportExcelDialog::~ExportExcelDialog()
{
    delete ui;
}

void ExportExcelDialog::on_confirmChB_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void ExportExcelDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ExportExcelDialog::on_okBtn_clicked()
{
    bool Done=false;
    QString Path, Area,Abbr, Saloon, Device, InterfaceType, Interface,Module,Label,PeerAbbr,PeerDev,PeerInt,PeerLbl, Eth,Odf,Pos,Pin;

    Path = QFileDialog::getSaveFileName(this, "Save DaNet Interface-Odf Export As", "","xlsx (*.xlsx)");
    if(Path.isEmpty())
        return;
    else
    {
        if(!Path.endsWith(".xlsx", Qt::CaseInsensitive)) Path += ".xlsx";

        //############################### excel file
        //xsl file : fist create then save
        QXlsx::Document xlsx;
        xlsx.addSheet("DaNet Interface-Odf Report");
        xlsx.deleteSheet("sheet1");

        xlsx.setDocumentProperty("title", "Interface-Odf Excel Report");
        xlsx.setDocumentProperty("subject", "Export");
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
        format.setPatternBackgroundColor(QColor(240, 240, 250));
        format.setBorderStyle(QXlsx::Format::BorderDouble);

        xlsx.setRowHeight(1, 40);
        xlsx.mergeCells("A1:P1", format);
        xlsx.write("A1", "Data Network Planning Department");

        format.setFontColor(Qt::darkBlue);
        format.setPatternBackgroundColor(QColor(220, 220, 230));

        xlsx.setRowHeight(2, 40);
        xlsx.mergeCells("A2:P2", format);
        xlsx.write("A2", "DaNet Interface-Odf Export");
        //`int_id`, `Area`, `Abbr`, `Saloon_name`, `Device`, `Interface_type`, `Interface`,
        //`Module`,`Label`, `peer_Abbr`, `peer_Device`, `peer_Interface`, `peer_Label`,
        //`Ether_trunk`, `Odf_no`, `Pos_no` `Pin_no`
        //headers
        format.setFontSize(10);
        format.setFontColor(Qt::darkMagenta);
        format.setPatternBackgroundColor(QColor(220, 220, 220));
        format.setBorderStyle(QXlsx::Format::BorderHair);
        xlsx.setRowHeight(3, 30);
        xlsx.setColumnWidth(1, 5);
        xlsx.write("A3", "Area", format);
        xlsx.setColumnWidth(2, 15);
        xlsx.write("B3", "Abbr", format);
        xlsx.setColumnWidth(3, 10);
        xlsx.write("C3", "Saloon", format);
        xlsx.setColumnWidth(4, 20);
        xlsx.write("D3", "Device", format);
        xlsx.setColumnWidth(5, 10);
        xlsx.write("E3", "Interface Type", format);
        xlsx.setColumnWidth(6, 15);
        xlsx.write("F3", "Interface", format);
        xlsx.setColumnWidth(7, 12);
        xlsx.write("G3", "Module", format);
        xlsx.setColumnWidth(8, 30);
        xlsx.write("H3", "Label", format);
        xlsx.setColumnWidth(9, 15);
        xlsx.write("I3", "Peer Abbr", format);
        xlsx.setColumnWidth(10, 20);
        xlsx.write("J3", "Peer Device", format);
        xlsx.setColumnWidth(11, 15);
        xlsx.write("K3", "Peer Interface", format);
        xlsx.setColumnWidth(12, 30);
        xlsx.write("L3", "Peer Label", format);
        xlsx.setColumnWidth(13, 10);
        xlsx.write("M3", "Eth Trunk", format);
        xlsx.setColumnWidth(14, 8);
        xlsx.write("N3", "ODF", format);
        xlsx.setColumnWidth(15, 8);
        xlsx.write("O3", "POS", format);
        xlsx.setColumnWidth(16, 10);
        xlsx.write("P3", "PIN", format);

        //records
        format.setFontBold(false);
        format.setFontColor(Qt::black);
        format.setPatternBackgroundColor(QColor(255, 255, 255));
        format.setBorderStyle(QXlsx::Format::BorderHair);

        int row = 3, modelR = 0;
        QModelIndex index = model->index(modelR, 0);
        while(modelR < model->rowCount())
        {
            row++;
            xlsx.setRowHeight(row,25);

            index = index.sibling(modelR, AREA);
            Area = model->data(index).toString();
            index = index.sibling(modelR, ABBR);
            Abbr = model->data(index).toString();
            index = index.sibling(modelR, SALOON);
            Saloon = model->data(index).toString();
            index = index.sibling(modelR, DEVICE);
            Device = model->data(index).toString();
            index = index.sibling(modelR, INT_TYPE);
            InterfaceType = model->data(index).toString();
            index = index.sibling(modelR, INT);
            Interface = model->data(index).toString();
            index = index.sibling(modelR, MODULE);
            Module = model->data(index).toString();
            index = index.sibling(modelR, LABEL);
            Label = model->data(index).toString();
            index = index.sibling(modelR, PEER_ABBR);
            PeerAbbr = model->data(index).toString();
            index = index.sibling(modelR, PEER_DEV);
            PeerDev = model->data(index).toString();
            index = index.sibling(modelR, PEER_INT);
            PeerInt = model->data(index).toString();
            index = index.sibling(modelR, PEER_LBL);
            PeerLbl = model->data(index).toString();
            index = index.sibling(modelR, ETH);
            Eth = model->data(index).toString();
            index = index.sibling(modelR, ODF);
            Odf = model->data(index).toString();
            index = index.sibling(modelR, POS);
            Pos = model->data(index).toString();
            index = index.sibling(modelR, PIN);
            Pin = model->data(index).toString();


            xlsx.write("A"+QString::number(row), Area, format);
            xlsx.write("B"+QString::number(row), Abbr, format);
            xlsx.write("C"+QString::number(row), Saloon, format);
            xlsx.write("D"+QString::number(row), Device, format);
            xlsx.write("E"+QString::number(row), InterfaceType, format);
            xlsx.write("F"+QString::number(row), Interface, format);
            xlsx.write("G"+QString::number(row), Module, format);
            xlsx.write("H"+QString::number(row), Label, format);
            xlsx.write("I"+QString::number(row), PeerAbbr, format);
            xlsx.write("J"+QString::number(row), PeerDev, format);
            xlsx.write("K"+QString::number(row), PeerInt, format);
            xlsx.write("L"+QString::number(row), PeerLbl, format);
            xlsx.write("M"+QString::number(row), Eth, format);
            xlsx.write("N"+QString::number(row), Odf, format);
            xlsx.write("O"+QString::number(row), Pos, format);
            xlsx.write("P"+QString::number(row), Pin, format);

            modelR++;
        }

        xlsx.saveAs(Path);
        Done = true;
    }


    if(Done)
    {
        QDesktopServices::openUrl(Path);
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot export due to some critical errors.");

}


