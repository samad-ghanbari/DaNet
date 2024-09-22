#include "accessbrasconfigdialog.h"
#include "ui_accessbrasconfigdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/netTools/nettools.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QFileDialog>
#include <QPrinter>
#include <QDateTime>
#include <QTextEdit>
#include <QFile>
#include <QDir>

AccessBRASConfigDialog::AccessBRASConfigDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AccessBRASConfigDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    //area
    ui->areaCB->blockSignals(true);
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
    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));
}

AccessBRASConfigDialog::~AccessBRASConfigDialog()
{
    delete ui;
}

void AccessBRASConfigDialog::checkEntry()
{
    bool AREA = !ui->areaCB->currentText().isEmpty();
    bool ABBR = !ui->abbrCB->currentText().isEmpty();

    bool b1 = !ui->bras1CB->currentText().isEmpty();
    bool b2 = ! (ui->bras2CB->currentText().isEmpty() && ui->bras2EnCB->isChecked() );

    if(AREA && ABBR && b1 && b2)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AccessBRASConfigDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    ui->okBtn->setEnabled(false);
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();

    ui->bras1CB->clear();
    ui->bras2CB->clear();
    ui->b1a1Lbl->clear();
    ui->b1a2Lbl->clear();
    ui->b2a1Lbl->clear();
    ui->b2a2Lbl->clear();

    int exchId;
    QString abbr;
    QSqlQuery *query = dbMan->selectAreaAbbreviations(area,true);
    while(query->next())
    {
        exchId = query->value(0).toInt();
        abbr = query->value(1).toString();

        ui->abbrCB->addItem(abbr, exchId);
    }
    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);
}

void AccessBRASConfigDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    ui->okBtn->setEnabled(false);

    ui->bras1CB->clear();
    ui->bras2CB->clear();
    ui->b1a1Lbl->clear();
    ui->b1a2Lbl->clear();
    ui->b2a1Lbl->clear();
    ui->b2a2Lbl->clear();
    ui->agg1CB->clear();

    int exchId = ui->abbrCB->currentData().toInt();

    ui->agg1CB->blockSignals(true);
    QSqlQuery *query = dbMan->selectExchangeAgg(exchId,true);
    int shelfId;
    QString device;

    while(query->next())
    {
        shelfId = query->value(0).toInt();
        device = query->value(1).toString();
        ui->agg1CB->addItem(device, shelfId);
    }
    ui->agg1CB->setCurrentIndex(-1);
    ui->agg1CB->blockSignals(false);
    if(ui->agg1CB->count() > 0) ui->agg1CB->setCurrentIndex(0);


    if(!dbMan->brasAggConnectionExistance(exchId))
    {
        QMessageBox::warning(this,"ERROR", "You need to define BRAS-Aggregation Ether-trunk connection.\n Check Plan > Connection > BRAS TO AGG.");
    }
    else
    {
        //BRAS1
        ui->bras1CB->blockSignals(true);
        QSqlQuery *query = dbMan->selectBras(exchId,true);
        //id, name
        int ID;
        QString NAME;
        while(query->next())
        {
            ID = query->value(0).toInt();
            NAME = query->value(1).toString();

            ui->bras1CB->addItem(NAME, ID);
        }
        ui->bras1CB->setCurrentIndex(-1);
        ui->bras1CB->blockSignals(false);
        if(ui->bras1CB->count() > 0) ui->bras1CB->setCurrentIndex(0);

        //bras2
        ui->bras2CB->blockSignals(true);
        query = dbMan->selectBras( exchId, false);
        //id, name
        while(query->next())
        {
            ID = query->value(0).toInt();
            NAME = query->value(1).toString();

            ui->bras2CB->addItem(NAME, ID);
        }
        ui->bras2CB->setCurrentIndex(-1);
        ui->bras2CB->blockSignals(false);
        if(ui->bras2CB->count() > 0) ui->bras2CB->setCurrentIndex(0);
    }
    checkEntry();
}

void AccessBRASConfigDialog::on_okBtn_clicked()
{   
    QString hsiVlan = QString::number(ui->hsiVlanSB->value());
    QString sinetVlan = QString::number(ui->sinetVlanSB->value());
    QString voipVlan = QString::number(ui->voipVlanSB->value());


    QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd  hh:mm");
    QString name = dbMan->getLoggedinLastname();

    QString exchange = QString::number(ui->areaCB->currentData().toInt()) +"-"+ui->abbrCB->currentText();
    QString str = exchange +"-Access Plan BRAS Config";
    QString path = QFileDialog::getExistingDirectory(this, "Select Output Directory");
    if(!path.isEmpty())
    {

        QString reportPath; //for pdfs
        if(!path.endsWith('/'))
            path = path+"/";
        path += str;

        if(QDir(path).exists())
        {
            int i=2;
            while(QDir(path+QString::number(i)).exists())
                i++;

            path += QString::number(i);
        }


        if(QDir().mkdir(path))
        {
            reportPath = path + "/"+"Data Plan-"+ exchange +"-BRAS CONFIG";
            if(QDir().mkdir(reportPath))
            {
                QTextEdit doc;
                //######## BRAS1
                bool bras1Done = false;
                int bras1Id = ui->bras1CB->currentData().toInt();
                //check vendor
                int deviceId = dbMan->getDevexDeviceId(bras1Id);
                int aggId = ui->agg1CB->currentData().toInt();
                int aggDeviceId = dbMan->getDevexDeviceId(aggId);
                if(dbMan->isDeviceHuaweiVendor(deviceId))
                {
                    QString bras1Path = path+"/"+"BRAS1-Config.txt";
                    QFile bras1Source(":/configurations/HUAWEI/AccessDep/CONFIG-BRAS");
                    if(bras1Source.open(QIODevice::ReadOnly | QIODevice::Text))
                    {
                        QString text = QString(bras1Source.readAll());
                        bras1Source.close();
                        QFile bras1File(bras1Path);

                        if(bras1File.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                        {
                            QString brasExchParam = dbMan->getDevexAreaExchange(bras1Id);
                            QString exchParam = ui->abbrCB->currentText();
                            QString deviceParam = dbMan->getDevice(deviceId, false);
                            QString switchParam = dbMan->getDevice(aggDeviceId, false);
                            if(switchParam.endsWith("-1"))
                            {
                              switchParam = switchParam.replace("-1", "-");
                            }


                            QString EthAgg1Param = QString::number(ui->b1a1SB->value());
                            QString EthAgg2Param = QString::number(ui->b1a2SB->value());
                            QString AreaNoParam = QString::number(ui->areaCB->currentData().toInt());
                            QString NASIpParam = dbMan->getBrasNasIp(bras1Id);

                            text.replace("brasExchParam", brasExchParam);
                            text.replace("exchParam", exchParam);
                            text.replace("deviceParam",deviceParam);
                            text.replace("switchParam",switchParam);
                            text.replace("EthAgg1Param",EthAgg1Param);
                            text.replace("EthAgg2Param",EthAgg2Param);
                            text.replace("AreaNoParam",AreaNoParam);
                            text.replace("NASIpParam",NASIpParam);

                            text.replace("hsiVlan",hsiVlan);
                            text.replace("sinetVlan",sinetVlan);
                            text.replace("voipVlan",voipVlan);

                            bras1File.seek(0); // go to the beginning of the file
                            text = "\t\tDaNet Configuration Report\n\nReported By "  +name + "\n" +now+"\n_______________________________________\n\n" + text;
                            bras1File.write(text.toUtf8()); // write the new text back to the file

                            bras1File.close(); // close the file handle.
                            bras1Done = true;
                            doc.setText(text);

                        }
                    }


                    if(bras1Done)
                    {
                        //print pdfs
                        //print config
                        QString bras1PdfPath = reportPath+"/"+"BRAS1-Config.pdf";
                        QPrinter printer(QPrinter::PrinterResolution);
                        printer.setOutputFormat(QPrinter::PdfFormat);
                        printer.setPaperSize(QPrinter::A4);
                        printer.setOutputFileName(bras1PdfPath);
                        doc.print(&printer);

                    }
                }

                //######## BRAS2
                bool bras2Done = false;
                if(ui->bras2EnCB->isChecked())
                {
                    int bras2Id = ui->bras2CB->currentData().toInt();

                    //check vendor
                    deviceId = dbMan->getDevexDeviceId(bras2Id);
                    aggId = ui->agg1CB->currentData().toInt();
                    aggDeviceId = dbMan->getDevexDeviceId(aggId);
                    if(dbMan->isDeviceHuaweiVendor(deviceId))
                    {
                        QString bras2Path = path+"/"+"BRAS2-Config.txt";
                        QFile bras2Source(":/configurations/HUAWEI/AccessDep/CONFIG-BRAS");
                        if(bras2Source.open(QIODevice::ReadOnly | QIODevice::Text))
                        {
                            QString text = QString(bras2Source.readAll());
                            bras2Source.close();
                            QFile bras2File(bras2Path);

                            if(bras2File.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                            {
                                QString brasExchParam = dbMan->getDevexAreaExchange(bras2Id);
                                QString exchParam = ui->abbrCB->currentText();
                                QString deviceParam = dbMan->getDevice(deviceId, false);
                                QString switchParam = dbMan->getDevice(aggDeviceId, false);
                                if(switchParam.endsWith("-1"))
                                {
                                  switchParam = switchParam.replace("-1", "-");
                                }

                                QString EthAgg1Param = QString::number(ui->b2a1SB->value());
                                QString EthAgg2Param = QString::number(ui->b2a2SB->value());
                                QString AreaNoParam = QString::number(ui->areaCB->currentData().toInt());
                                QString NASIpParam = dbMan->getBrasNasIp(bras1Id);

                                text.replace("brasExchParam", brasExchParam);
                                text.replace("exchParam", exchParam);
                                text.replace("deviceParam",deviceParam);
                                text.replace("switchParam",switchParam);
                                text.replace("EthAgg1Param",EthAgg1Param);
                                text.replace("EthAgg2Param",EthAgg2Param);
                                text.replace("AreaNoParam",AreaNoParam);
                                text.replace("NASIpParam",NASIpParam);

                                text.replace("hsiVlan",hsiVlan);
                                text.replace("sinetVlan",sinetVlan);
                                text.replace("voipVlan",voipVlan);

                                bras2File.seek(0); // go to the beginning of the file
                                text = "\t\tDaNet Configuration Report\n\nReported By "  +name + "\n" +now+"\n_______________________________________\n\n" + text;
                                bras2File.write(text.toUtf8()); // write the new text back to the file

                                bras2File.close(); // close the file handle.
                                bras2Done = true;
                                doc.setText(text);

                            }
                        }


                        if(bras2Done)
                        {
                            //print pdfs
                            //print config
                            QString bras2PdfPath = reportPath+"/"+"BRAS2-Config.pdf";
                            QPrinter printer(QPrinter::PrinterResolution);
                            printer.setOutputFormat(QPrinter::PdfFormat);
                            printer.setPaperSize(QPrinter::A4);
                            printer.setOutputFileName(bras2PdfPath);
                            doc.print(&printer);

                        }
                    }

                }
                else
                    bras2Done = true;
                //##########
                if(bras1Done && bras2Done)
                {
                    QMessageBox::information(this, "Succesfully Done.", "Check the exported files in path:\n"+path);
                    this->close();
                }
                else
                    QMessageBox::warning(this,"error", "Please try again.");

            }
            else
                QMessageBox::warning(this,"error", "Please try again.");
        }
        else
            QMessageBox::warning(this,"error", "Please try again.");
    }


}

void AccessBRASConfigDialog::on_bras2EnCB_toggled(bool checked)
{
    if(checked)
        ui->bras2GB->show();
    else
        ui->bras2GB->hide();

    checkEntry();
}

void AccessBRASConfigDialog::on_bras1CB_currentIndexChanged(int index)
{
    index++;
    int brasId = ui->bras1CB->currentData().toInt();
    int exchId = ui->abbrCB->currentData().toInt();
    QList<int> list = dbMan->getBrasAggIdEth(brasId, exchId);
    //`agg1_shelf_id`,`agg1_eth`,`agg2_shelf_id`,`agg2_eth`
    if(list.at(0) > -1)
    {
        ui->b1a1SB->setValue(list.at(1));
        ui->b1a2SB->setValue(list.at(3));

        QString lbl = "Eth To "+ ui->abbrCB->currentText() +" Agg ";
        ui->b1a1Lbl->setText(lbl+"1 :");
        ui->b1a2Lbl->setText(lbl+"2 :");

    }
    else
    {
        ui->b1a1SB->setValue(-1);
        ui->b1a2SB->setValue(-1);
    }

}

void AccessBRASConfigDialog::on_bras2CB_currentIndexChanged(int index)
{
    index++;
    int brasId = ui->bras2CB->currentData().toInt();
    int exchId = ui->abbrCB->currentData().toInt();
    QList<int> list = dbMan->getBrasAggIdEth(brasId, exchId);
    //`agg1_shelf_id`,`agg1_eth`,`agg2_shelf_id`,`agg2_eth`
    if(list.at(0) > -1)
    {
        ui->b2a1SB->setValue(list.at(1));
        ui->b2a2SB->setValue(list.at(3));

        QString lbl = "Eth To "+ ui->abbrCB->currentText() +" Agg ";
        ui->b2a1Lbl->setText(lbl+"1 :");
        ui->b2a2Lbl->setText(lbl+"2 :");

    }
    else
    {
        ui->b2a1SB->setValue(-1);
        ui->b2a2SB->setValue(-1);
    }
}
