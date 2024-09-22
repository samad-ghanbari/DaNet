#include "interfacereportdialog.h"
#include "ui_interfacereportdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/report/interface/ReportInterface.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QMap>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDesktopServices>

InterfaceReportDialog::InterfaceReportDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::InterfaceReportDialog),
    dbMan(db),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);
    ui->toolBox->setCurrentIndex(0);
    ui->exchChB->setChecked(true);
    ui->deviceChB->setChecked(true);
    ui->deviceCB->setVisible(false);
    ui->deviceCB->setEnabled(false);
    ui->allDeviceLbl->setVisible(false);
    ui->allExchLbl->setVisible(false);

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

    //type
    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);

    ui->deviceCB->blockSignals(true);
    QMap<QString, int> devMap = dbMan->selectDeviceMap(false);
    foreach (QString dev, devMap.keys())
    {
        ui->deviceCB->addItem(dev, devMap.value(dev));
    }
    ui->deviceCB->setCurrentIndex(-1);
    ui->deviceCB->blockSignals(false);


    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

    ui->exchChB->blockSignals(true);
    ui->exchChB->setChecked(false);
    ui->exchChB->blockSignals(false);
    ui->exchChB->setChecked(true);

    ui->deviceChB->blockSignals(true);
    ui->deviceChB->setChecked(true);
    ui->deviceChB->blockSignals(false);
    ui->deviceChB->setChecked(false);

    ui->peerChB->blockSignals(true);
    ui->peerChB->setChecked(true);
    ui->peerChB->blockSignals(false);
    ui->peerChB->setChecked(false);

    model->setColumnCount(9);
    model->setHeaderData(0, Qt::Horizontal,"Row");
    model->setHeaderData(1, Qt::Horizontal,"Area No");
    model->setHeaderData(2, Qt::Horizontal,"Area");
    model->setHeaderData(3, Qt::Horizontal,"Exch ID");
    model->setHeaderData(4, Qt::Horizontal,"Exchange");
    model->setHeaderData(5, Qt::Horizontal,"Device ID");
    model->setHeaderData(6, Qt::Horizontal,"Devex ID");
    model->setHeaderData(7, Qt::Horizontal,"Device");
    model->setHeaderData(8, Qt::Horizontal,"Peer");



    ui->reportTV->setModel(model);
    ui->reportTV->verticalHeader()->hide();
    ui->reportTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->reportTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->reportTV->hideColumn(0);
    ui->reportTV->hideColumn(1);
    ui->reportTV->hideColumn(3);
    ui->reportTV->hideColumn(5);
    ui->reportTV->hideColumn(6);

    // context menu slot
    ui->reportTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->reportTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionRemove_Record);

    ui->okBtn->setEnabled(false);
    ui->addBtn->setEnabled(false);

}

InterfaceReportDialog::~InterfaceReportDialog()
{
    delete ui;
}

void InterfaceReportDialog::checkAddBtn()
{
    if((!ui->exchChB->isChecked()) && (!ui->deviceChB->isChecked()))
        ui->peerChB->setChecked(true);


    bool a = !ui->areaCB->currentText().isEmpty();
    bool e = !ui->exchChB->isChecked() || (ui->exchChB->isChecked() && !ui->abbrCB->currentText().isEmpty());
    bool t;
    int type = ui->typeCB->currentData().toInt();
    if(type  == 2)
        t = true;
    else if(type == 3)
        t = !ui->siteCB->currentText().isEmpty();
    else
        t = false;

    bool d;
    if(ui->deviceChB->isChecked())
    {
        if(ui->exchChB->isChecked())
            d = !ui->devexCB->currentText().isEmpty();
        else
            d = !ui->deviceCB->currentText().isEmpty();
    }
    else
        d = true;

    bool p;
    if(ui->peerChB->isChecked())
        p = !ui->peerLE->text().isEmpty();
    else
        p = true;


    if(a && e && t && d && p)
        ui->addBtn->setEnabled(true);
    else
        ui->addBtn->setEnabled(false);
}

void InterfaceReportDialog::checkPreviewBtn()
{
    if(model->rowCount() > 0)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void InterfaceReportDialog::contextMenuSlot(QPoint ptr)
{
    clickedRow = -1;
    QModelIndex clickedIndex = ui->reportTV->indexAt(ptr);
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0);
    if(!model->data(clickedIndex).isNull())
    {
        clickedRow = clickedIndex.row();
        contextMenu.popup(ui->reportTV->mapToGlobal(ptr));
    }


    checkPreviewBtn();
}

void InterfaceReportDialog::on_cancelBtn_clicked()
{
    this->close();
}

void InterfaceReportDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    QSqlQuery *query = dbMan->selectAreaAbbreviations(area,true);
    int exchId;
    QString abbr;
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();
    ui->siteCB->clear();
    ui->devexCB->clear();
    while(query->next())
    {
        exchId = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr, exchId);
    }
    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);

    ui->typeCB->setCurrentIndex(0);
    checkAddBtn();
}

void InterfaceReportDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    ui->siteCB->clear();
    ui->devexCB->clear();

    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);
    ui->typeCB->setCurrentIndex(0);

    checkAddBtn();
}

void InterfaceReportDialog::on_typeCB_currentIndexChanged(int index)
{
    ui->devexCB->clear();
    ui->siteCB->clear();

    if(index == 0) //exch
    {
        ui->siteFrame->setEnabled(false);
        ui->siteFrame->setVisible(false);

        int exchId = ui->abbrCB->currentData().toInt();
        QSqlQuery *query = dbMan->selectDevex(exchId);
        // `id`, `exchange_id`, `saloon`, `device_id`, `rack_row`, `rack_no`, `shelf_no`, `device_name`
        int DEVEXID; ;
        QString DEVICENAME;
        ui->devexCB->blockSignals(true);
        while(query->next())
        {
            DEVEXID = query->value(0).toInt();
            DEVICENAME = query->value(7).toString();

            ui->devexCB->addItem(DEVICENAME, DEVEXID);
        }
        ui->devexCB->setCurrentIndex(-1);
        ui->devexCB->blockSignals(false);

    }
    else//site
    {
        ui->siteFrame->setEnabled(true);
        ui->siteFrame->setVisible(true);

        QString mother = ui->abbrCB->currentText();
        int area = ui->areaCB->currentData().toInt();
        QSqlQuery *query = dbMan->selectSites(area, mother);

        ui->siteCB->blockSignals(true);
        ui->siteCB->clear();
        int i;
        QString a, s,c,n;
        while(query->next())
        {
            i = query->value(0).toInt();
            a = query->value(1).toString();
            s = query->value(2).toString();
            c = query->value(3).toString();
            n = query->value(4).toString();
            ui->siteCB->addItem(c+"-"+n+"*"+a+"*"+s, i);
        }

        ui->siteCB->setCurrentIndex(-1);
        ui->siteCB->blockSignals(false);

    }

    checkAddBtn();
}

void InterfaceReportDialog::on_siteCB_currentIndexChanged(int index)
{
    index++;
    int exchId = ui->siteCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectDevex(exchId);
    //`id`, `exchange_id`, `saloon`, `device_id`, `rack_row`, `rack_no`, `shelf_no`, `device_name`
    ui->devexCB->clear();

    int DEVEXID;
    QString DEVICENAME;
    while(query->next())
    {
        DEVEXID = query->value(0).toInt();
        DEVICENAME = query->value(7).toString();

        ui->devexCB->addItem(DEVICENAME, DEVEXID);
    }

    ui->devexCB->setCurrentIndex(-1);
    ui->devexCB->blockSignals(false);

    checkAddBtn();
}

void InterfaceReportDialog::on_devexCB_currentIndexChanged(int index)
{
    index++;
    checkAddBtn();
}

void InterfaceReportDialog::on_deviceCB_currentIndexChanged(int index)
{
    index++;
    checkAddBtn();
}

void InterfaceReportDialog::on_addBtn_clicked()
{
    int areaNo = ui->areaCB->currentData().toInt();
    QString area = ui->areaCB->currentText();
    int exchId, deviceId, devexId;
    QString exch, device;
    if(ui->exchChB->isChecked())
    {
        exchId = ui->abbrCB->currentData().toInt();
        exch = area+"-"+ui->abbrCB->currentText();
        if(ui->typeCB->currentData().toInt() == 3)
        {
            exchId = ui->siteCB->currentData().toInt();
            exch = exch + ui->siteCB->currentText();
        }
    }
    else
    {
        exchId = -1;
        exch = "All Exchanges";
    }

    if(ui->deviceChB->isChecked())
    {
        if(ui->exchChB->isChecked())
        {
            devexId = ui->devexCB->currentData().toInt();
            deviceId = -1;
            device = ui->devexCB->currentText();
        }
        else
        {
            deviceId = ui->deviceCB->currentData().toInt();
            devexId = -1;
            device = ui->deviceCB->currentText();
        }

    }
    else
    {
        deviceId = -1;
        devexId = -1;
        device = "All Devices";
    }

    QString peer;
    if(ui->peerChB->isChecked())
        peer = ui->peerLE->text().trimmed();
    else
        peer = "";

    int row = model->rowCount();
    model->insertRow(row);
    //fields:
    // row-areaNo-area-exchId-exch-deviceId-devexId-device
    index = model->index(row,0);
    model->setData(index, QVariant(row));
    index = index.sibling(row, 1);
    model->setData(index, QVariant(areaNo));
    index = index.sibling(row, 2);
    model->setData(index, QVariant(area));
    index = index.sibling(row, 3);
    model->setData(index, QVariant(exchId));
    index = index.sibling(row, 4);
    model->setData(index, QVariant(exch));
    index = index.sibling(row, 5);
    model->setData(index, QVariant(deviceId));
    index = index.sibling(row, 6);
    model->setData(index, QVariant(devexId));
    index = index.sibling(row, 7);
    model->setData(index, QVariant(device));
    index = index.sibling(row, 8);
    model->setData(index, QVariant(peer));

    ui->reportTV->hideColumn(0);
    ui->reportTV->hideColumn(1);
    ui->reportTV->hideColumn(3);
    ui->reportTV->hideColumn(5);
    ui->reportTV->hideColumn(6);

    checkPreviewBtn();
    ui->okBtn->setFocus();
}

void InterfaceReportDialog::on_okBtn_clicked()
{
    int areaNo,exchId,deviceId,devexId;
    QString peer;
    QString condition;
    bool start;
    for(int i=0; i < model->rowCount(); i++)
    {
        index = model->index(i,1);// areaNo
        areaNo = model->data(index).toInt();

        index = index.sibling(i,3);
        exchId = model->data(index).toInt();

        index = index.sibling(i,5);
        deviceId = model->data(index).toInt();

        index = index.sibling(i,6);
        devexId = model->data(index).toInt();

        index = index.sibling(i,8);
        peer = model->data(index).toString().trimmed();

        start = true;
        if(!condition.isEmpty())
            condition += " OR";
        condition += " (";
        if(areaNo > 1)
        {
            if(start)
                condition += " area_no="+QString::number(areaNo);
            else
                condition += " AND area_no="+QString::number(areaNo);

            start = false;
        }

        if(exchId > 0)
        {
            if(start)
                condition += " exchange_id="+QString::number(exchId);
            else
                condition += " AND exchange_id="+QString::number(exchId);

            start = false;
        }

        if(deviceId > 0)
        {
            if(start)
                condition += " device_id="+QString::number(deviceId);
            else
                condition += " AND device_id="+QString::number(deviceId);

            start = false;
        }

        if(devexId > 0)
        {
            if(start)
                condition += " devex_id="+QString::number(devexId);
            else
                condition += " AND devex_id="+QString::number(devexId);

            start = false;
        }

        if(!peer.isEmpty())
        {
            if(start)
                condition += " peer_abbr LIKE '%"+peer +"%' OR peer_device LIKE '%"+peer+"%' OR peer_label LIKE '%"+peer+"%'";
            else
                condition += " AND (peer_abbr LIKE '%"+peer +"%' OR peer_device LIKE '%"+peer+"%' OR peer_label LIKE '%"+peer+"%')";

            start = false;
        }

        condition += ")";

    }

    condition += ";";

    QSqlQuery *query = dbMan->selectViewInterfacesWhere(condition);
//`area`,`abbr`,`exchange`,`saloon_name`,`device`, `interface_type`,`interface`,`module`,`label`, `peer_abbr`,`peer_device`,`peer_interface`,`peer_label`,`ether_trunk`

    QSqlQueryModel *repModel = new QSqlQueryModel(this);
    repModel->setQuery(*query);

    QString customInfo = ui->customInfoLE->text().trimmed();

    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Interfaces Report AS", "", "pdf (*.pdf)");
    if(!path.isEmpty())
    {

        if(!path.endsWith(".pdf", Qt::CaseInsensitive))
            path += ".pdf";

        if(customInfo.isEmpty())
        {
            ReportInterface reportInterface(this,repModel, path);
            if(reportInterface.print())
            {
                QDesktopServices::openUrl(path);
            }
            else
            {
                QMessageBox::warning(this, "ERROR", reportInterface.lastError());
            }
        }
        else
        {
            ReportInterface reportInterface(this,repModel, path, customInfo);
            if(reportInterface.print())
            {
                QDesktopServices::openUrl(path);
            }
            else
            {
                QMessageBox::warning(this, "ERROR", reportInterface.lastError());
            }
        }

        this->close();
    }
}

void InterfaceReportDialog::on_exchChB_toggled(bool checked)
{
    ui->abbrCB->setCurrentIndex(-1);
    ui->peerChB->setChecked(false);

    if(checked)
    {
        ui->abbrCB->setEnabled(true);
        ui->abbrCB->setVisible(true);
        ui->allExchLbl->setVisible(false);
        ui->typeCB->setCurrentIndex(0);
        ui->typeCB->setEnabled(true);

        ui->deviceChB->blockSignals(true);
        ui->deviceChB->setChecked(true);
        ui->deviceChB->blockSignals(false);
        ui->deviceChB->setChecked(false);
    }
    else
    {
        ui->abbrCB->setEnabled(false);
        ui->abbrCB->setVisible(false);
        ui->allExchLbl->setVisible(true);
        ui->typeCB->setCurrentIndex(0);
        ui->typeCB->setEnabled(false);

        ui->deviceChB->blockSignals(true);
        ui->deviceChB->setChecked(false);
        ui->deviceChB->blockSignals(false);
        ui->deviceChB->setChecked(true);
    }

    checkAddBtn();
}

void InterfaceReportDialog::on_deviceChB_toggled(bool checked)
{
    ui->devexCB->setCurrentIndex(-1);
    ui->deviceCB->setCurrentIndex(-1);
    ui->peerChB->setChecked(false);


    if(checked)
    {
        ui->allDeviceLbl->setVisible(false);
        if(ui->exchChB->isChecked())
        {
            ui->devexCB->setEnabled(true);
            ui->devexCB->setVisible(true);
            ui->deviceCB->setEnabled(false);
            ui->deviceCB->setVisible(false);
        }
        else
        { //all
            ui->devexCB->setEnabled(false);
            ui->devexCB->setVisible(false);
            ui->deviceCB->setEnabled(true);
            ui->deviceCB->setVisible(true);
        }
    }
    else
    {
        //all
        ui->allDeviceLbl->setVisible(true);

        ui->devexCB->setEnabled(false);
        ui->devexCB->setVisible(false);
        ui->deviceCB->setEnabled(false);
        ui->deviceCB->setVisible(false);
        if(!ui->exchChB->isChecked())
        {
            ui->peerChB->setChecked(true);
            ui->peerLE->setFocus();
        }

    }

    checkAddBtn();
}

void InterfaceReportDialog::on_peerChB_toggled(bool checked)
{
    ui->peerLE->clear();
    if(checked)
        ui->peerLE->setEnabled(true);
    else
        ui->peerLE->setEnabled(false);

    checkAddBtn();
}

void InterfaceReportDialog::on_peerLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->addBtn->setEnabled(false);
    else
        checkAddBtn();
}

void InterfaceReportDialog::on_actionRemove_Record_triggered()
{
    model->removeRow(clickedRow);
    if(model->rowCount() == 0)
        ui->okBtn->setEnabled(false);
}
