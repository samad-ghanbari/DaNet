#include "moduleanalyserdialog.h"
#include "ui_moduleanalyserdialog.h"
#include <QFileDialog>
#include <QStandardItemModel>
#include <QTextStream>
#include <QDesktopServices>
#include <QMessageBox>
#include <QtXlsx>
#include <QString>

ModuleAnalyserDialog::ModuleAnalyserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleAnalyserDialog),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    //os
    ui->osCB->addItem("HUAWEI OS", 0);
    ui->osCB->addItem("CISCO IOS", 1);
    ui->osCB->addItem("CISCO IOS-XR", 2);
    redBrush.setColor(Qt::darkRed);
    ui->infoLE->setVisible(false);
    INFO_FLAG = false;
    ui->exportBtn->setEnabled(false);
}

ModuleAnalyserDialog::~ModuleAnalyserDialog()
{
    delete ui;
}

void ModuleAnalyserDialog::huaweiModulesAnalyser()
{
    model->setColumnCount(3);
    model->setRowCount(0);
    model->setHeaderData(0, Qt::Horizontal,"Interface");
    model->setHeaderData(1, Qt::Horizontal,"Status");
    model->setHeaderData(2, Qt::Horizontal,"Module");

    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();
    ui->TV->setModel(model);

    QModelIndex index;
    int Row = 0, ind;


    QTextStream configStream(&configFile);

    QString Line,temp, Sysname,Interface, Status, Module;

    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Line = configStream.readLine().trimmed();
        bool SYSNAME = true;
        while(!Line.isNull())
        {
            //purify remove ---- More ----  and space
            if(Line.contains("---- More ----",Qt::CaseInsensitive))
                Line = Line.remove("---- More ----",Qt::CaseInsensitive);
            Line = Line.trimmed();

            if(SYSNAME)
            {
                if(Line.startsWith("sysname", Qt::CaseInsensitive))
                {
                    Line = Line.remove("sysname", Qt::CaseInsensitive).trimmed();
                    Sysname = Line;
                    ui->sysNameLbl->setText(Sysname);
                    SYSNAME = false;
                }
            }

            //interfaces start
            if( (Line.contains("current state :", Qt::CaseInsensitive)) && (!Line.contains("protocol current state :", Qt::CaseInsensitive)) )
            {
                Interface = "";
                Status = "";
                Module = "";

                temp = Line;

                ind = temp.indexOf("current state :",Qt::CaseInsensitive);
                temp = temp.remove(ind,temp.length());
                Interface = temp.trimmed();

                ind = Line.indexOf(":",Qt::CaseInsensitive);
                Line = Line.remove(0, ind+1);
                Status = Line;

                Line = configStream.readLine().trimmed();
                //purify remove ---- More ----  and space
                if(Line.contains("---- More ----",Qt::CaseInsensitive))
                    Line = Line.remove("---- More ----",Qt::CaseInsensitive);
                Line = Line.trimmed();

                if(Line.contains("protocol current state :", Qt::CaseInsensitive))
                {
                    ind = Line.indexOf(":",Qt::CaseInsensitive);
                    Line = Line.remove(0, ind+1);
                    Status += "/"+ Line;

                    Line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(Line.contains("---- More ----",Qt::CaseInsensitive))
                        Line = Line.remove("---- More ----",Qt::CaseInsensitive);
                    Line = Line.trimmed();
                }

                while( (!Line.isNull()) && (!Line.contains("current state :", Qt::CaseInsensitive)))
                {
                    if(Line.contains("WaveLength:", Qt::CaseInsensitive))
                    {
                        ind = Line.indexOf("WaveLength:",Qt::CaseInsensitive);
                        Line = Line.remove(0, ind);
                        Line = Line.remove("WaveLength:");
                        Line = Line.remove("Transmission Distance:", Qt::CaseInsensitive);
                        if(Line.contains(","))
                            Line = Line.replace(",", "-");
                        Line = Line.replace(" ", "");

                        Module = Line;

                    }

                    Line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(Line.contains("---- More ----",Qt::CaseInsensitive))
                        Line = Line.remove("---- More ----",Qt::CaseInsensitive);
                    Line = Line.trimmed();
                }

                if( (!Interface.isNull()) && (!Status.isNull()) )
                {
                    if(Status.contains("down", Qt::CaseInsensitive))
                    {
                        model->insertRow(Row);
                        index = model->index(Row,0);
                        model->setData(index,redBrush, Qt::ForegroundRole);
                        model->setData(index,Interface);
                        index = index.sibling(Row, 1);
                        model->setData(index,redBrush, Qt::ForegroundRole);
                        model->setData(index, Status);
                        index = index.sibling(Row, 2);
                        model->setData(index,redBrush, Qt::ForegroundRole);
                        model->setData(index, Module);
                    }
                    else
                    {
                        model->insertRow(Row);
                        index = model->index(Row,0);
                        model->setData(index,Interface);
                        index = index.sibling(Row, 1);
                        model->setData(index, Status);
                        index = index.sibling(Row, 2);
                        model->setData(index, Module);
                    }
                    Row++;
                }
            }
            else
                Line = configStream.readLine().trimmed();
        }
    }

    configFile.close();

    if(model->rowCount() > 0)
        ui->exportBtn->setEnabled(true);
    else
        ui->exportBtn->setEnabled(false);
}

void ModuleAnalyserDialog::IOSModulesAnalyser()
{
    model->clear();

    model->setColumnCount(3);
    model->setRowCount(0);
    model->setHeaderData(0, Qt::Horizontal,"Interface");
    model->setHeaderData(1, Qt::Horizontal,"Status");
    model->setHeaderData(2, Qt::Horizontal,"Module");

    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();
    ui->TV->setModel(model);

    QModelIndex index;
    int Row = 0, ind;


    QTextStream configStream(&configFile);

    QString Line,temp, Sysname,Interface, Status, Module;

    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Line = configStream.readLine().trimmed();
        bool SYSNAME = true;
        while(!Line.isNull())
        {
            //purify remove ---- More ----  and space
            if(Line.contains("--More--",Qt::CaseInsensitive))
                Line = Line.remove("--More--",Qt::CaseInsensitive);
            Line = Line.trimmed();

            if(SYSNAME)
            {
                if(Line.startsWith("hostname", Qt::CaseInsensitive))
                {
                    Line = Line.remove("hostname", Qt::CaseInsensitive).trimmed();
                    Sysname = Line;
                    ui->sysNameLbl->setText(Sysname);
                    SYSNAME = false;
                }
            }

            //interfaces start
            if( Line.contains("line protocol is", Qt::CaseInsensitive) )
            {
                Interface = "";
                Status = "";
                Module = "";

                temp = Line;

                ind = temp.indexOf("is",0,Qt::CaseInsensitive);
                temp = temp.remove(ind,temp.length());
                Interface = temp.trimmed();

                Line = Line.remove(0, ind);
                Line = Line .remove("line protocol is administratively ", Qt::CaseInsensitive);
                Line = Line .remove("line protocol is ", Qt::CaseInsensitive);
                Line = Line .remove("is", Qt::CaseInsensitive);
                Line = Line.replace(",","/");
                Status = Line;

                Line = configStream.readLine().trimmed();
                //purify remove ---- More ----  and space
                if(Line.contains("--More--",Qt::CaseInsensitive))
                    Line = Line.remove("--More--",Qt::CaseInsensitive);
                Line = Line.trimmed();

                while( (!Line.isNull()) && (!Line.contains("line protocol is", Qt::CaseInsensitive)))
                {
                    if(Line.startsWith("Full-duplex", Qt::CaseInsensitive))
                    {
                        ind = Line.indexOf("duplex",0,Qt::CaseInsensitive);
                        Line = Line.remove(0, ind);
                        Line = Line.remove("duplex,", Qt::CaseInsensitive);
                        ind = Line.indexOf("b/s",0,Qt::CaseInsensitive);
                        Line = Line.remove(0, ind);
                        Line = Line.remove("b/s", Qt::CaseInsensitive);
                        ind = Line.indexOf("link type", 0, Qt::CaseInsensitive);
                        Line = Line.remove(ind,Line.length());
                        Line = Line.remove(",");
                        Module = Line;
                    }


                    Line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(Line.contains("--More--",Qt::CaseInsensitive))
                        Line = Line.remove("--More--",Qt::CaseInsensitive);
                    Line = Line.trimmed();
                }

                if( (!Interface.isNull()) && (!Status.isNull()) )
                {
                    if(Status.contains("down", Qt::CaseInsensitive))
                    {
                        model->insertRow(Row);
                        index = model->index(Row,0);
                        model->setData(index,redBrush, Qt::ForegroundRole);
                        model->setData(index,Interface);
                        index = index.sibling(Row, 1);
                        model->setData(index,redBrush, Qt::ForegroundRole);
                        model->setData(index, Status);
                        index = index.sibling(Row, 2);
                        model->setData(index,redBrush, Qt::ForegroundRole);
                        model->setData(index, Module);
                    }
                    else
                    {
                        model->insertRow(Row);
                        index = model->index(Row,0);
                        model->setData(index,Interface);
                        index = index.sibling(Row, 1);
                        model->setData(index, Status);
                        index = index.sibling(Row, 2);
                        model->setData(index, Module);
                    }
                    Row++;
                }
            }
            else
                Line = configStream.readLine().trimmed();
        }
    }

    configFile.close();

    if(model->rowCount() > 0)
        ui->exportBtn->setEnabled(true);
    else
        ui->exportBtn->setEnabled(false);
}

void ModuleAnalyserDialog::IOSXRModulesAnalyser()
{
    model->clear();

    model->setColumnCount(3);
    model->setRowCount(0);
    model->setHeaderData(0, Qt::Horizontal,"Interface");
    model->setHeaderData(1, Qt::Horizontal,"Status");
    model->setHeaderData(2, Qt::Horizontal,"Module");

    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();
    ui->TV->setModel(model);

    QModelIndex index;
    int Row = 0, ind;


    QTextStream configStream(&configFile);

    QString Line,temp, Sysname,Interface, Status, Module;

    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Line = configStream.readLine().trimmed();
        bool SYSNAME = true;
        while(!Line.isNull())
        {
            //purify remove ---- More ----  and space
            if(Line.contains("--More--",Qt::CaseInsensitive))
                Line = Line.remove("--More--",Qt::CaseInsensitive);
            Line = Line.trimmed();

            if(SYSNAME)
            {
                if(Line.startsWith("hostname", Qt::CaseInsensitive))
                {
                    Line = Line.remove("hostname", Qt::CaseInsensitive).trimmed();
                    Sysname = Line;
                    ui->sysNameLbl->setText(Sysname);
                    SYSNAME = false;
                }
            }

            //interfaces start
            if( Line.contains("line protocol is", Qt::CaseInsensitive) )
            {
                Interface = "";
                Status = "";
                Module = "";

                temp = Line;

                ind = temp.indexOf("is",0,Qt::CaseInsensitive);
                temp = temp.remove(ind,temp.length());
                Interface = temp.trimmed();

                Line = Line.remove(0, ind);
                Line = Line .remove("line protocol is administratively ", Qt::CaseInsensitive);
                Line = Line .remove("line protocol is ", Qt::CaseInsensitive);
                Line = Line .remove("is", Qt::CaseInsensitive);
                Line = Line.replace(",","/");
                Status = Line;

                Line = configStream.readLine().trimmed();
                //purify remove ---- More ----  and space
                if(Line.contains("--More--",Qt::CaseInsensitive))
                    Line = Line.remove("--More--",Qt::CaseInsensitive);
                Line = Line.trimmed();

                while( (!Line.isNull()) && (!Line.contains("line protocol is", Qt::CaseInsensitive)))
                {
                    if(Line.startsWith("Full-duplex", Qt::CaseInsensitive))
                    {
                        ind = Line.indexOf("duplex",0,Qt::CaseInsensitive);
                        Line = Line.remove(0, ind);
                        Line = Line.remove("duplex,", Qt::CaseInsensitive);
                        ind = Line.indexOf("b/s",0,Qt::CaseInsensitive);
                        Line = Line.remove(0, ind);
                        Line = Line.remove("b/s", Qt::CaseInsensitive);
                        ind = Line.indexOf("link type", 0, Qt::CaseInsensitive);
                        Line = Line.remove(ind,Line.length());
                        Line = Line.remove(",");
                        Module = Line;
                    }


                    Line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(Line.contains("--More--",Qt::CaseInsensitive))
                        Line = Line.remove("--More--",Qt::CaseInsensitive);
                    Line = Line.trimmed();
                }

                if( (!Interface.isNull()) && (!Status.isNull()) )
                {
                    if(Status.contains("down", Qt::CaseInsensitive))
                    {
                        model->insertRow(Row);
                        index = model->index(Row,0);
                        model->setData(index,redBrush, Qt::ForegroundRole);
                        model->setData(index,Interface);
                        index = index.sibling(Row, 1);
                        model->setData(index,redBrush, Qt::ForegroundRole);
                        model->setData(index, Status);
                        index = index.sibling(Row, 2);
                        model->setData(index,redBrush, Qt::ForegroundRole);
                        model->setData(index, Module);
                    }
                    else
                    {
                        model->insertRow(Row);
                        index = model->index(Row,0);
                        model->setData(index,Interface);
                        index = index.sibling(Row, 1);
                        model->setData(index, Status);
                        index = index.sibling(Row, 2);
                        model->setData(index, Module);
                    }
                    Row++;
                }
            }
            else
                Line = configStream.readLine().trimmed();
        }
    }

    configFile.close();

    if(model->rowCount() > 0)
        ui->exportBtn->setEnabled(true);
    else
        ui->exportBtn->setEnabled(false);
}

void ModuleAnalyserDialog::on_osCB_currentIndexChanged(int index)
{
    index++;
    model->clear();
    ui->fileLbl->clear();
    ui->sysNameLbl->clear();
    ui->infoLE->clear();
    ui->infoLE->setVisible(false);
    INFO_FLAG = false;

    int os = ui->osCB->currentData().toInt();
    switch (os)
    {
    case 0://huawei
        ui->cmdLbl->setText("display interface ");
        break;
    case 1://ios
        ui->cmdLbl->setText("show interface");
        break;
    case 2://ios xr
        ui->cmdLbl->setText("show interface");
        break;
    }
}

void ModuleAnalyserDialog::on_btn_clicked()
{
    ui->infoLE->clear();
    ui->infoLE->setVisible(false);
    INFO_FLAG = false;

    QString configFileName = QFileDialog::getOpenFileName(this,"Select Captured File(dis/show int)");
    if(configFileName.isEmpty())
    {
        model->clear();
        ui->fileLbl->clear();
        ui->sysNameLbl->clear();
        return;
    }
    else
    {
        ui->fileLbl->setText(configFileName);

        configFile.setFileName(configFileName);

        int os = ui->osCB->currentData().toInt();
        switch (os)
        {
        case 0:
            huaweiModulesAnalyser();
            break;
        case 1:
            IOSModulesAnalyser();
            break;
        case 2:
            IOSXRModulesAnalyser();
            break;
        }
    }
}

void ModuleAnalyserDialog::on_exportBtn_clicked()
{
    if(!INFO_FLAG)
    {
        INFO_FLAG = true;
        ui->infoLE->setVisible(true);
        return;
    }
    else
    {
        bool Done=false;

        QString Path, Interface,Status,module, Info;

        Info = ui->infoLE->text().trimmed();

        Path = QFileDialog::getSaveFileName(this, "Save DaNet Device Modules Export As", "","xlsx (*.xlsx)");
        if(Path.isEmpty())
            return;
        else
        {
            if(!Path.endsWith(".xlsx", Qt::CaseInsensitive)) Path += ".xlsx";

            //############################### excel file
            //xsl file : fist create then save
            QXlsx::Document xlsx;
            xlsx.addSheet("DaNet Modules Report");
            xlsx.deleteSheet("sheet1");

            xlsx.setDocumentProperty("title", "Device Modules Excel Report");
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
            xlsx.mergeCells("A1:C1", format);
            xlsx.write("A1", "Data Network Planning Department");

            format.setFontColor(Qt::darkBlue);
            format.setPatternBackgroundColor(QColor(220, 220, 230));

            xlsx.setRowHeight(2, 40);
            xlsx.mergeCells("A2:C2", format);
            xlsx.write("A2", "DaNet Device Modules Export");

            int row, modelR = 0;

            //info
            if(Info.isEmpty())
            {
                row = 3;
                //int  status  module
                //headers
                format.setFontSize(10);
                format.setFontColor(Qt::darkMagenta);
                format.setPatternBackgroundColor(QColor(220, 220, 220));
                format.setBorderStyle(QXlsx::Format::BorderHair);
                xlsx.setRowHeight(3, 30);
                xlsx.setColumnWidth(1, 20);
                xlsx.write("A3", "Interface", format);
                xlsx.setColumnWidth(2, 30);
                xlsx.write("B3", "Status", format);
                xlsx.setColumnWidth(3, 20);
                xlsx.write("C3", "Module", format);
            }
            else
            {
                row = 4;
                format.setPatternBackgroundColor(QColor(220, 220, 220));
                xlsx.setRowHeight(3, 40);
                xlsx.mergeCells("A3:C3", format);
                xlsx.write("A3", Info);

                //int  status  module
                //headers
                format.setFontSize(10);
                format.setFontColor(Qt::darkMagenta);
                format.setPatternBackgroundColor(QColor(220, 220, 220));
                format.setBorderStyle(QXlsx::Format::BorderHair);
                xlsx.setRowHeight(4, 30);
                xlsx.setColumnWidth(1, 20);
                xlsx.write("A4", "Interface", format);
                xlsx.setColumnWidth(2, 30);
                xlsx.write("B4", "Status", format);
                xlsx.setColumnWidth(3, 20);
                xlsx.write("C4", "Module", format);
            }




            //records
            format.setFontBold(false);
            format.setFontColor(Qt::black);
            format.setPatternBackgroundColor(QColor(255, 255, 255));
            format.setBorderStyle(QXlsx::Format::BorderHair);

            QModelIndex index = model->index(modelR, 0);
            while(modelR < model->rowCount())
            {
                row++;
                xlsx.setRowHeight(row,25);

                index = index.sibling(modelR, 0);
                Interface = model->data(index).toString();
                index = index.sibling(modelR, 1);
                Status = model->data(index).toString();
                index = index.sibling(modelR, 2);
                module = model->data(index).toString();

                if(Status.contains("down", Qt::CaseInsensitive))
                    format.setFontColor(Qt::red);
                else
                    format.setFontColor(Qt::black);

                xlsx.write("A"+QString::number(row), Interface, format);
                xlsx.write("B"+QString::number(row), Status, format);
                xlsx.write("C"+QString::number(row), module, format);

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
}

