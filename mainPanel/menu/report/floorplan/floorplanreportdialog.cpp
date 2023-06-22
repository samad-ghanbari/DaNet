#include "floorplanreportdialog.h"
#include "ui_floorplanreportdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/sortfilterproxymodel.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include "lib/report/floorplan/TL/ReportFloorPlanTL.h"
#include "lib/report/floorplan/TR/ReportFloorPlanTR.h"
#include "lib/report/floorplan/BL/ReportFloorPlanBL.h"
#include "lib/report/floorplan/BR/ReportFloorPlanBR.h"
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTableView>

FloorPlanReportDialog::FloorPlanReportDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::FloorPlanReportDialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->toolBox->setCurrentIndex(0);
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

    ui->paperCB->addItem("A0", 0);
    ui->paperCB->addItem("A1", 1);
    ui->paperCB->addItem("A2", 2);
    ui->paperCB->addItem("A3", 3);
    ui->paperCB->addItem("A4", 4);
    ui->paperCB->addItem("A5", 5);
    ui->paperCB->setCurrentIndex(ui->paperCB->findData(3));

    ui->orientationCB->addItem("Landscape", 0);
    ui->orientationCB->addItem("Portrait", 1);
    ui->orientationCB->setCurrentIndex(ui->orientationCB->findData(0));

    ui->fontStyleCB->addItem("Normal", 0);
    ui->fontStyleCB->addItem("Bold", 1);
    ui->fontStyleCB->setCurrentText("Normal");

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

    ui->reportTV->setModel(model);
    ui->reportTV->verticalHeader()->hide();
    ui->reportTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->reportTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    ui->okBtn->setEnabled(false);
}

FloorPlanReportDialog::~FloorPlanReportDialog()
{
    delete ui;
}

void FloorPlanReportDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    model->clear();
    QSqlQuery *query = dbMan->selectAreaAbbreviations(area,true);
    int exchId;
    QString abbr;
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();

    while(query->next())
    {
        exchId = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr, exchId);
    }
    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);
    ui->okBtn->setEnabled(false);
}

void FloorPlanReportDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    model->clear();
    ui->okBtn->setEnabled(true);
    ui->toolBox->setCurrentIndex(0);

    //saloon
    ui->saloonCB->blockSignals(true);
    ui->saloonCB->clear();
    int exchId = ui->abbrCB->currentData().toInt();
    QList<QString> list = dbMan->getExistDevexSaloonNo(exchId);
    QList<QString> listOdf = dbMan->getExistOdfSaloonNo(exchId);
    foreach (QString i, listOdf)
    {
        if(!list.contains(i))
            list << i;
    }
    QSqlQuery *query = dbMan->selectSaloons(list);
    int saloonId;
    QString saloonStr;
    while(query->next())
    {
        saloonId = query->value(0).toInt();
        saloonStr = query->value(1).toString();
        ui->saloonCB->addItem(saloonStr, saloonId);
    }
    ui->saloonCB->setCurrentIndex(-1);
    ui->saloonCB->blockSignals(false);

    if(ui->abbrCB->currentText().isEmpty())
    {
        ui->okBtn->setEnabled(false);
    }
    else
    {
        if(ui->saloonCB->count() > 0)
            ui->saloonCB->setCurrentIndex(0);
    }
}

void FloorPlanReportDialog::on_saloonCB_currentIndexChanged(int index)
{
    index++;
    model->clear();
    ui->okBtn->setEnabled(false);
    if(ui->abbrCB->currentText().isEmpty() || ui->saloonCB->currentText().isEmpty())
    {
        return;
    }
    else
    {
        int exchId = ui->abbrCB->currentData().toInt();
        int saloon = ui->saloonCB->currentData().toInt();

        QSqlQuery *query = dbMan->selectViewDevex(exchId, saloon);
        //`devex_id`, `exchange_id`, `saloon_no`, `device_id`, `saloon_name`,
        //`row`, `rack`, `shelf`, `type`,`device`, `device_name`, `purchase`, `description`

        model->setQuery(*query);
        ui->reportTV->hideColumn(0);
        ui->reportTV->hideColumn(1);
        ui->reportTV->hideColumn(2);
        ui->reportTV->hideColumn(3);
        ui->reportTV->hideColumn(4);

        model->setHeaderData(5, Qt::Horizontal, "Row");
        model->setHeaderData(6, Qt::Horizontal, "Rack No");
        model->setHeaderData(7, Qt::Horizontal, "Shelf No");
        model->setHeaderData(8, Qt::Horizontal, "Type");
        model->setHeaderData(9, Qt::Horizontal, "Device");
        model->setHeaderData(10, Qt::Horizontal, "Device Name");
        model->setHeaderData(11, Qt::Horizontal, "Purchase");
        model->setHeaderData(12, Qt::Horizontal, "Description");

        odfMap = dbMan->getOdfMap(exchId, saloon); //`odf_row`, `odf_column`, `odf_no`, `pin_per_pos`
        if( (model->rowCount() > 0) || (odfMap.count() > 0) ) ui->okBtn->setEnabled(true); else ui->okBtn->setEnabled(false);

    }
}

void FloorPlanReportDialog::on_cancelBtn_clicked()
{
    this->close();
}

void FloorPlanReportDialog::on_okBtn_clicked()
{

    QString customInfo = ui->customInfoLE->text().trimmed();
    if(customInfo.isEmpty()) customInfo = "";

    QString abbr = ui->areaCB->currentText()+"-"+ui->abbrCB->currentText();
    QString exchange = dbMan->getExchangeName(ui->abbrCB->currentData().toInt());
    QString saloon = ui->saloonCB->currentText();
    int ps = ui->paperCB->currentData().toInt();
    int orient = ui->orientationCB->currentData().toInt();
    QFont font;
    font.setBold(ui->fontStyleCB->currentData().toInt());
    font.setPixelSize(ui->fontSizeSB->value());
    font.setFamily("Times");

    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Floor Plan Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {

        if(!path.endsWith(".pdf")) path += ".pdf";

        int exchId = ui->abbrCB->currentData().toInt();
        QMap<int,QString> saloonOrientation = dbMan->getExchangeSaloonsOrientation(exchId);
        int saloonNo = ui->saloonCB->currentData().toInt();
        if(saloonOrientation.count() == 0)
        {
            QMessageBox::warning(this, "Error", "Cannot specify saloon orientation.Try again later. ");
            return;
        }


        //create Model > proxymodel>sort by row column
        QStandardItemModel *standardModel = new QStandardItemModel(this);
        standardModel->setColumnCount(7);
        QModelIndex ind1, ind2;
        //row-rack-shelf-device-RO(rack/Odf)-odfNo-ppp

        //0`devex_id`, 1`exchange_id`, 2`saloon_no`, 3`device_id`, 4`saloon_name`,
        //5`row`, 6`rack`, 7`shelf`, 8type, 9`device`, 10`device_name`, 11`purchase`, 12`description`

        int rowCout = model->rowCount();
        int r = 0;
        QString dev;
        while(r < rowCout)
        {
            standardModel->insertRow(r);

            ind1 = model->index(r, 5);//row
            ind2 = standardModel->index(r, 0);
            standardModel->setData(ind2,model->data(ind1));

            ind1 = model->index(r, 6);//rack
            ind2 = standardModel->index(r, 1);
            standardModel->setData(ind2,model->data(ind1));

            ind1 = model->index(r, 7);//shelf
            ind2 = standardModel->index(r, 2);
            standardModel->setData(ind2,model->data(ind1));

            ind1 = model->index(r, 9);//device
            dev = model->data(ind1).toString();
            ind2 = standardModel->index(r, 3);
            standardModel->setData(ind2,dev);

            ind2 = standardModel->index(r, 4);//RO
            if( (dev.compare("OCDF", Qt::CaseInsensitive) == 0) || (dev.compare("IDF", Qt::CaseInsensitive) == 0) )
                standardModel->setData(ind2,1);
            else
                standardModel->setData(ind2,0);

            r++;
        }
        //odf
        QList<int> list;
        foreach (int o, odfMap.keys())
        {
            //`odf_row`, `odf_column`, `odf_no`, `pin_per_pos`

            list = odfMap.value(o);
            standardModel->insertRow(r);

            ind2 = standardModel->index(r, 0);//row
            standardModel->setData(ind2,list.at(0));

            ind2 = standardModel->index(r, 1);//column(rack)
            standardModel->setData(ind2,list.at(1));

            ind2 = standardModel->index(r, 3);//dev
            standardModel->setData(ind2,"ODF");

            ind2 = standardModel->index(r, 4);//RO
            standardModel->setData(ind2,1);

            ind2 = standardModel->index(r, 5);//odfNo
            standardModel->setData(ind2,list.at(2));

            ind2 = standardModel->index(r, 6);//ppp
            standardModel->setData(ind2,list.at(3));

            r++;
        }

        //sort based on row-column (because of two columns > have too use this class)
        SortFilterProxyModel *proxyModel = new SortFilterProxyModel(this);
        proxyModel->setSourceModel(standardModel);
        proxyModel->sort(2, Qt::AscendingOrder);
        proxyModel->sort(1, Qt::AscendingOrder);
        proxyModel->sort(0, Qt::AscendingOrder);


        QString sOrient = saloonOrientation.value(saloonNo);
        if(sOrient.compare("TR", Qt::CaseInsensitive) == 0)
        {
            //top right
            ReportFloorPlanTR reportFloorPlan(this, proxyModel, abbr,exchange,saloon, path,ps, orient,font, customInfo);
            if(reportFloorPlan.print())
            {
                QDesktopServices::openUrl(path);
//                ui->saloonCB->setCurrentIndex(-1);
//                ui->okBtn->setEnabled(false);
//                printFrameFlag = true;
//                ui->printFrame->setVisible(false);
            }
            else
            {
                QMessageBox::warning(this, "Error", reportFloorPlan.lastError());
            }
        }
        else if(sOrient.compare("BL", Qt::CaseInsensitive) == 0)
        {
            //bottom left
            ReportFloorPlanBL reportFloorPlan(this, proxyModel, abbr,exchange,saloon, path,ps, orient,font,customInfo);
            if(reportFloorPlan.print())
            {
                QDesktopServices::openUrl(path);
//                ui->saloonCB->setCurrentIndex(-1);
//                ui->okBtn->setEnabled(false);
//                printFrameFlag = true;
//                ui->printFrame->setVisible(false);
            }
            else
            {
                QMessageBox::warning(this, "Error", reportFloorPlan.lastError());
            }
        }
        else if(sOrient.compare("BR", Qt::CaseInsensitive) == 0)
        {
            //bottom right
            ReportFloorPlanBR reportFloorPlan(this, proxyModel, abbr,exchange,saloon, path,ps, orient,font,customInfo);
            if(reportFloorPlan.print())
            {
                QDesktopServices::openUrl(path);
//                ui->saloonCB->setCurrentIndex(-1);
//                ui->okBtn->setEnabled(false);
//                printFrameFlag = true;
//                ui->printFrame->setVisible(false);
            }
            else
            {
                QMessageBox::warning(this, "Error", reportFloorPlan.lastError());
            }
        }
        else
            {

                //top left and others
                ReportFloorPlanTL reportFloorPlan(this, proxyModel, abbr,exchange,saloon, path,ps, orient,font,customInfo);
                if(reportFloorPlan.print())
                {
                    QDesktopServices::openUrl(path);
//                    ui->saloonCB->setCurrentIndex(-1);
//                    ui->okBtn->setEnabled(false);
//                    printFrameFlag = true;
//                    ui->printFrame->setVisible(false);
                }
                else
                {
                    QMessageBox::warning(this, "Error", reportFloorPlan.lastError());
                }
            }


        this->close();
    }

}









