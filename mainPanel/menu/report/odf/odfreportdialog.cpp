#include "odfreportdialog.h"
#include "ui_odfreportdialog.h"
#include "lib/database/danetdbman.h"
//#include "mainPanel/menu/report/odf/odfreportpreviewdialog.h"
#include "lib/report/odf/ReportOdf.h"
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QMap>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDesktopServices>

OdfReportDialog::OdfReportDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::OdfReportDialog),
    dbMan(db),
    model(new QStandardItemModel(this))
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

    //type
    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);


    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));
    ui->toolBox->setCurrentIndex(0);

    ui->odfChB->blockSignals(true);
    ui->odfChB->setChecked(false);
    ui->odfChB->blockSignals(false);
    ui->odfChB->setChecked(true);

    ui->posChB->blockSignals(true);
    ui->posChB->setChecked(true);
    ui->posChB->blockSignals(false);
    ui->posChB->setChecked(false);

    ui->peerChB->blockSignals(true);
    ui->peerChB->setChecked(true);
    ui->peerChB->blockSignals(false);
    ui->peerChB->setChecked(false);

    model->setColumnCount(9);
    model->setHeaderData(0, Qt::Horizontal,"ODF_id");
    model->setHeaderData(1, Qt::Horizontal,"Exchange_id");
    model->setHeaderData(2, Qt::Horizontal,"Abbr");
    model->setHeaderData(3, Qt::Horizontal,"Saloon_No");
    model->setHeaderData(4, Qt::Horizontal,"Saloon");
    model->setHeaderData(5, Qt::Horizontal,"ODF No");
    model->setHeaderData(6, Qt::Horizontal,"POS_id");
    model->setHeaderData(7, Qt::Horizontal,"POS");
    model->setHeaderData(8, Qt::Horizontal,"Peer");


    ui->reportTV->setModel(model);
    ui->reportTV->verticalHeader()->hide();
    ui->reportTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->reportTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->reportTV->hideColumn(0);
    ui->reportTV->hideColumn(1);
    ui->reportTV->hideColumn(3);
    ui->reportTV->hideColumn(6);

    // context menu slot
    ui->reportTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->reportTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionRemove_Record);

    ui->okBtn->setEnabled(false);
    ui->addBtn->setEnabled(false);

}

OdfReportDialog::~OdfReportDialog()
{
    delete ui;
}

void OdfReportDialog::checkAddBtn()
{

    bool a = !ui->areaCB->currentText().isEmpty();
    bool e = !ui->abbrCB->currentText().isEmpty();
    bool t;
    int type = ui->typeCB->currentData().toInt();
    if(type  == 2)
        t = true;
    else if(type == 3)
        t = !ui->siteCB->currentText().trimmed().isEmpty();
    else
        t = false;

    bool o;
    if(ui->odfChB->isChecked())
        o = !ui->odfCB->currentText().isEmpty();
    else
        o = true;

    bool op;
    if(ui->posChB->isChecked())
    {
        if(ui->odfChB->isChecked())
            op = !ui->posCB->currentText().isEmpty();
        else
            op = true;
    }
    else
        op = true;

    bool p;
    if(ui->peerChB->isChecked())
        p = !ui->peerLE->text().isEmpty();
    else
        p = true;


    if(a && e && t && o && op && p)
        ui->addBtn->setEnabled(true);
    else
        ui->addBtn->setEnabled(false);
}

void OdfReportDialog::checkPreviewBtn()
{
    if(model->rowCount() > 0)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void OdfReportDialog::contextMenuSlot(QPoint ptr)
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

void OdfReportDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    int exchId;
    QString abbr;
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();
    ui->siteCB->clear();
    ui->odfCB->clear();
    ui->posCB->clear();
    ui->saloonCB->clear();
    QSqlQuery *query = dbMan->selectAreaAbbreviations(area,true);
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

void OdfReportDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    ui->siteCB->clear();
    ui->odfCB->clear();
    ui->posCB->clear();

    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);
    ui->typeCB->setCurrentIndex(0);
    checkAddBtn();
}

void OdfReportDialog::on_typeCB_currentIndexChanged(int index)
{
    ui->odfCB->clear();
    ui->posCB->clear();
    ui->siteCB->clear();

    if(index == 0) //exch
    {
        ui->siteFrame->setEnabled(false);
        ui->siteFrame->setVisible(false);

        ui->saloonCB->blockSignals(true);
        ui->saloonCB->clear();
        int exchId = ui->abbrCB->currentData().toInt();
        QList<QString> list = dbMan->getExistOdfSaloonNo(exchId);
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
        ui->saloonCB->setEnabled(true);
        ui->saloonCB->setCurrentIndex(0);

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

        ui->saloonCB->blockSignals(true);
        ui->saloonCB->clear();
        query = dbMan->selectSaloons(true,false);
        int saloonId;
        QString saloon;
        while(query->next())
        {
            saloonId = query->value(0).toInt();
            saloon = query->value(1).toString();
            ui->saloonCB->addItem(saloon, saloonId);
        }
        ui->saloonCB->setCurrentIndex(-1);
        ui->saloonCB->blockSignals(false);
        ui->saloonCB->setEnabled(false);
    }
    checkAddBtn();
}

void OdfReportDialog::on_siteCB_currentIndexChanged(int index)
{
    index++;
    if(ui->typeCB->currentData().toInt() == 3) //site
    {
        if(ui->siteCB->currentText().isEmpty())
        {
            ui->saloonCB->setEnabled(false);
        }
        else
        {
            ui->saloonCB->blockSignals(true);
            ui->saloonCB->setCurrentIndex(-1);
            ui->saloonCB->blockSignals(false);
            ui->saloonCB->setEnabled(true);
            ui->saloonCB->setCurrentIndex(0);
        }
    }

    checkAddBtn();

}

void OdfReportDialog::on_saloonCB_currentIndexChanged(int index)
{
    index++;
    int exchId = ui->abbrCB->currentData().toInt();
    if(ui->typeCB->currentData().toInt() == 3)
    {
        exchId = ui->siteCB->currentData().toInt();
    }

    int saloonNo = ui->saloonCB->currentData().toInt();

    ui->odfCB->blockSignals(true);
    ui->odfCB->clear();
    ui->odfCB->blockSignals(false);
    QMap<int, int> ODF = dbMan->getExchangeOdfMap(exchId, saloonNo);
    foreach (int o, ODF.keys())
        ui->odfCB->addItem(QString::number(o),ODF.value(o));


    checkAddBtn();
}

void OdfReportDialog::on_odfChB_toggled(bool checked)
{
    if(checked)
    {

        ui->odfCB->setVisible(true);
        ui->allOdfLbl->setVisible(false);
        if(ui->posChB->isChecked())
        {
            ui->posCB->setVisible(true);
            ui->posSB->setVisible(false);
            ui->allPosLbl->setVisible(false);
        }
        else
        {
            ui->posCB->setVisible(false);
            ui->posSB->setVisible(false);
            ui->allPosLbl->setVisible(true);
        }

    }
    else
    {
        ui->odfCB->setVisible(false);
        ui->allOdfLbl->setVisible(true);
        if(ui->posChB->isChecked())
        {
            ui->posCB->setVisible(false);
            ui->posSB->setVisible(true);
            ui->allPosLbl->setVisible(false);
        }
        else
        {
            ui->posCB->setVisible(false);
            ui->posSB->setVisible(false);
            ui->allPosLbl->setVisible(true);
        }

    }



    checkAddBtn();
}

void OdfReportDialog::on_odfCB_currentIndexChanged(int index)
{
    index++;
    int odfId = ui->odfCB->currentData().toInt();
    QMap<int, int> POS = dbMan->getOdfPosMap(odfId);
    ui->posCB->blockSignals(true);
    ui->posCB->clear();
    foreach (int p, POS.keys())
    {
        ui->posCB->addItem(QString::number(p),POS.value(p));
    }
    ui->posCB->setCurrentIndex(-1);
    ui->posCB->blockSignals(false);
    if(ui->posCB->count() > 0)
        ui->posCB->setCurrentIndex(0);
}

void OdfReportDialog::on_posChB_toggled(bool checked)
{
    if(checked)
    {
        if(ui->odfChB->isChecked())
        {
            ui->posCB->setVisible(true);
            ui->posSB->setVisible(false);
        }
        else
        {
            ui->posCB->setVisible(false);
            ui->posSB->setVisible(true);
        }
        ui->allPosLbl->setVisible(false);
    }
    else
    {
        ui->posCB->setVisible(false);
        ui->posSB->setVisible(false);
        ui->allPosLbl->setVisible(true);

    }
    checkAddBtn();
}

void OdfReportDialog::on_posCB_currentIndexChanged(int index)
{
    index++;

}

void OdfReportDialog::on_peerChB_toggled(bool checked)
{
    if(checked)
    {
        ui->peerLE->clear();
        ui->peerLE->setEnabled(true);
        ui->peerLE->setFocus();
    }
    else
    {
        ui->peerLE->clear();
        ui->peerLE->setEnabled(false);
    }

    checkAddBtn();
}

void OdfReportDialog::on_peerLE_textChanged(const QString &arg1)
{
    if(ui->peerChB->isChecked())
    {
        if(arg1.isEmpty())
            ui->addBtn->setEnabled(false);
        else
            checkAddBtn();
    }
    else
        checkAddBtn();
}

void OdfReportDialog::on_addBtn_clicked()
{
    QString area = ui->areaCB->currentText();
    int exchId, odfId, posId, saloonNo;
    QString exch,saloon, peer, odfNo, posNo;

    exchId = ui->abbrCB->currentData().toInt();
    exch = area+"-"+ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3)
    {
        exchId = ui->siteCB->currentData().toInt();
        exch = exch +"-"+ ui->siteCB->currentText();
    }

    saloonNo = ui->saloonCB->currentData().toInt();
    saloon = ui->saloonCB->currentText();

    if(ui->odfChB->isChecked())
    {
        odfId = ui->odfCB->currentData().toInt();
        odfNo = ui->odfCB->currentText();
    }
    else
    {
        odfId = -1;
        odfNo = "All ODFs";
    }

    if(ui->posChB->isChecked())
    {
        if(ui->odfChB->isChecked())
        {
            posId = ui->posCB->currentData().toInt();
            posNo = ui->posCB->currentText();
        }
        else
        {
            posId = -1;
            posNo = QString::number(ui->posSB->value());
        }
    }
    else
    { //all pos
        posId = -2;
        posNo = "All POS";
    }

    if(ui->peerChB->isChecked())
        peer = ui->peerLE->text().trimmed();
    else
        peer = "";


    int row = model->rowCount();
    model->insertRow(row);
    //fields:
    // ODF_id-Exchange_id-Abbr-Saloon_No-Saloon-ODF No-POS_id-POS-peer
    index = model->index(row,0);
    model->setData(index, QVariant(odfId));
    index = index.sibling(row, 1);
    model->setData(index, QVariant(exchId));
    index = index.sibling(row, 2);
    model->setData(index, QVariant(exch));
    index = index.sibling(row, 3);
    model->setData(index, QVariant(saloonNo));
    index = index.sibling(row, 4);
    model->setData(index, QVariant(saloon));
    index = index.sibling(row, 5);
    model->setData(index, QVariant(odfNo));
    index = index.sibling(row, 6);
    model->setData(index, QVariant(posId));
    index = index.sibling(row, 7);
    model->setData(index, QVariant(posNo));
    index = index.sibling(row, 8);
    model->setData(index, QVariant(peer));

    ui->reportTV->hideColumn(0);
    ui->reportTV->hideColumn(1);
    ui->reportTV->hideColumn(3);
    ui->reportTV->hideColumn(6);

    checkPreviewBtn();
    ui->okBtn->setFocus();


}

void OdfReportDialog::on_cancelBtn_clicked()
{
    this->close();
}

void OdfReportDialog::on_okBtn_clicked()
{
    //fields:
    // ODF_id-Exchange_id-Abbr-Saloon_No-Saloon-ODF No-POS_id-POS-peer
    int odfId,exchId,saloonNo, posId;
    QString abbr,saloon,odfNo,posNo,peer;
    QString condition ="";
    bool start;
    for(int i=0; i < model->rowCount(); i++)
    {
        index = model->index(i,0);// odfId
        odfId = model->data(index).toInt();

        index = index.sibling(i,1);
        exchId = model->data(index).toInt();

        index = index.sibling(i,2);
        abbr = model->data(index).toString();

        index = index.sibling(i,3);
        saloonNo = model->data(index).toInt();

        index = index.sibling(i,4);
        saloon = model->data(index).toString();

        index = index.sibling(i,5);
        odfNo = model->data(index).toString();

        index = index.sibling(i,6);
        posId = model->data(index).toInt();

        index = index.sibling(i,7);
        posNo = model->data(index).toString();

        index = index.sibling(i,8);
        peer = model->data(index).toString().trimmed();

        start = true;
        if(!condition.isEmpty())
            condition += " OR";
        condition += " (";

        if(odfId > -1)
        {
            if(start)
                condition += " odf_id="+QString::number(odfId);
            else
                condition += " AND odf_id="+QString::number(odfId);

            start = false;
        }

        if(exchId > -1)
        {
            if(start)
                condition += " exchange_id="+QString::number(exchId);
            else
                condition += " AND exchange_id="+QString::number(exchId);

            start = false;
        }

        if(saloonNo > -1)
        {
            if(start)
                condition += " saloon_no="+QString::number(saloonNo);
            else
                condition += " AND saloon_no="+QString::number(saloonNo);

            start = false;
        }

        if(posId > -1)
        {
            if(start)
                condition += " pos_id="+QString::number(posId);
            else
                condition += " AND pos_id="+QString::number(posId);

            start = false;
        }
        else if(posId == -1)
        {
            if(start)
                condition += " pos_no="+posNo;
            else
                condition += " AND pos_no="+posNo;

            start = false;
        }

        if(!peer.isEmpty())
        {
            if(start)
                condition += " remote LIKE '%"+peer +"%' OR remote_label LIKE '%"+peer+"%'";
            else
                condition += " AND (remote LIKE '%"+peer +"%' OR remote_label LIKE '%"+peer+"%')";

            start = false;
        }

        condition += ")";

    }

    condition += ";";

    QSqlQuery *query = dbMan->selectViewOdfPosPin(condition);
    //`odf_id`, `pos_id`, `pin_id`, `exchange_id`, `saloon_no`, `area`, `exchange`, `abbr`, `saloon_name`, `pin_per_pos`,
    //`odf_no`, `pos_no`, `local_device`, `local_label`, `pin_no`, `remote`, `remote_label`

    //    OdfReportPreviewDialog odfReportPreviewDialog(this, query);
    //    odfReportPreviewDialog.exec();
    //________________________________
    //print

    QString path = QFileDialog::getSaveFileName(this, "Save DaNet ODF Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf",Qt::CaseInsensitive)) path += ".pdf";

        QString customInfo = ui->customInfoLE->text().trimmed();

        QSqlQueryModel *printModel = new QSqlQueryModel(this);
        printModel->setQuery(*query);

        if(customInfo.isEmpty())
        {
            ReportOdf reportOdf(this, printModel, path);
            if(reportOdf.print())
            {
                QDesktopServices::openUrl(path);
            }
            else
            {
                QMessageBox::warning(this, "ERROR", reportOdf.lastError());
            }
        }
        else
        {
            ReportOdf reportOdf(this, printModel, path, customInfo);
            if(reportOdf.print())
            {
                QDesktopServices::openUrl(path);
            }
            else
            {
                QMessageBox::warning(this, "ERROR", reportOdf.lastError());
            }
        }


        this->close();
    }

    //end print
    //________________________________

//    query->clear();
//    model->clear();
//    model->setColumnCount(9);
//    model->setHeaderData(0, Qt::Horizontal,"ODF_id");
//    model->setHeaderData(1, Qt::Horizontal,"Exchange_id");
//    model->setHeaderData(2, Qt::Horizontal,"Abbr");
//    model->setHeaderData(3, Qt::Horizontal,"Saloon_No");
//    model->setHeaderData(4, Qt::Horizontal,"Saloon");
//    model->setHeaderData(5, Qt::Horizontal,"ODF No");
//    model->setHeaderData(6, Qt::Horizontal,"POS_id");
//    model->setHeaderData(7, Qt::Horizontal,"POS");
//    model->setHeaderData(8, Qt::Horizontal,"Peer");
//    ui->okBtn->setEnabled(false);

}

void OdfReportDialog::on_posSB_valueChanged(int arg1)
{
    arg1++;
    checkAddBtn();
}

void OdfReportDialog::on_actionRemove_Record_triggered()
{
    model->removeRow(clickedRow);
    if(model->rowCount() == 0)
        ui->okBtn->setEnabled(false);
}
