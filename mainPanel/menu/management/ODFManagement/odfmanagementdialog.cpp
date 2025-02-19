#include "odfmanagementdialog.h"
#include "ui_odfmanagementdialog.h"
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include "mainPanel/menu/management/ODFManagement/assignodfpindialog.h"
#include "mainPanel/menu/management/ODFManagement/editodfpindialog.h"
#include "mainPanel/menu/management/ODFManagement/depleteodfpindialog.h"

ODFManagementDialog::ODFManagementDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ODFManagementDialog),
    dbMan(db),
    tableModel(new CenterAlignedModel(this)),
    mainDb(true),
    abbrId(-1),
    odfId(-1),
    posId(-1),
    pinId(-1)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);
    saloon = -1;
    SEARCH_FLAG = false;

    tableModel->setSpecificColumn(4);

    ui->pinTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->pinTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->pinTV->verticalHeader()->hide();
    ui->pinTV->setModel(tableModel);

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
    ui->odfLbl->clear();

    //type
    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);

    ui->abbrCB->setEnabled(false);
    ui->typeCB->setEnabled(false);
    ui->siteCB->setEnabled(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);
    ui->saloonCB->setEnabled(false);
    ui->odfCB->setEnabled(false);
    ui->posCB->setEnabled(false);

    // context menu ODF
    ui->pinTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->pinTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
    connect(ui->pinTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(pinTV2ClickSlot(QModelIndex)));

    contextMenu.addAction(ui->actionAssignODFPin);
    contextMenu.addAction(ui->actionEditODFPin);
    contextMenu.addAction(ui->actionDepleteODFPin);

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

}

ODFManagementDialog::ODFManagementDialog(QWidget *parent, DanetDbMan *db, const bool mainDB, const int Area, const int AbbrId, const int OdfId, const int PosId, const int PinId):
    QDialog(parent),
    ui(new Ui::ODFManagementDialog),
    dbMan(db),
    tableModel(new CenterAlignedModel(this)),
    mainDb(mainDB),
    abbrId(AbbrId),
    odfId(OdfId),
    posId(PosId),
    pinId(PinId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    saloon = dbMan->odfSaloonNo(odfId);
    if(saloon == -1)
    {
        QMessageBox::warning(this,"Error", "Cannot find ODF Position.");
    }

    SEARCH_FLAG = true; //after finishing it will set false
    exchId = -1;
    siteId = -1;

    tableModel->setSpecificColumn(4);

    ui->pinTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->pinTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->pinTV->verticalHeader()->hide();
    ui->pinTV->setModel(tableModel);

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
    ui->odfLbl->clear();

    //type
    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);

    ui->abbrCB->setEnabled(false);
    ui->typeCB->setEnabled(false);
    ui->siteCB->setEnabled(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);
    ui->saloonCB->setEnabled(false);
    ui->odfCB->setEnabled(false);
    ui->posCB->setEnabled(false);

    QString dep = QString(DEPARTMENT);
    if((dep.compare("tarahi") == 0))
    {
        // context menu ODF
        ui->pinTV->setContextMenuPolicy(Qt::CustomContextMenu);
        if(mainDb)
        {
            connect(ui->pinTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
            connect(ui->pinTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(pinTV2ClickSlot(QModelIndex)));
        }

        contextMenu.addAction(ui->actionAssignODFPin);
        contextMenu.addAction(ui->actionEditODFPin);
        contextMenu.addAction(ui->actionDepleteODFPin);
    }


    ui->areaCB->setCurrentIndex(ui->areaCB->findData(Area));
}

ODFManagementDialog::~ODFManagementDialog()
{
    delete ui;
    delete tableModel;
    tableModel = NULL;
}

void ODFManagementDialog::fillTable(int posId)
{
    QSqlQuery *query = dbMan->selectPin(posId);
    //  `id`,`port_id`,`local_device`,`local_label`,`pin_no`,`remote`,`remote_label`
    tableModel->setQuery(*query);
    ui->pinTV->hideColumn(0);
    ui->pinTV->hideColumn(1);
    tableModel->setHeaderData(2,Qt::Horizontal,"Local Device");
    tableModel->setHeaderData(3,Qt::Horizontal,"Local Label");
    tableModel->setHeaderData(4,Qt::Horizontal,"PIN No");
    tableModel->setHeaderData(5,Qt::Horizontal,"Remote");
    tableModel->setHeaderData(6,Qt::Horizontal,"Remote Label");
}

void ODFManagementDialog::refreshSlot(int done)
{
    int posId = ui->posCB->currentData().toInt();
    done++;
    fillTable(posId);
    ui->pinTV->selectRow(clickedRow);
}

void ODFManagementDialog::on_areaCB_currentIndexChanged(int index)
{
    tableModel->clear();
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();
    ui->odfLbl->clear();

    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->setEnabled(false);


    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);

    ui->saloonCB->blockSignals(true);
    ui->saloonCB->clear();
    ui->saloonCB->setEnabled(false);
    ui->saloonCB->blockSignals(false);

    ui->odfCB->blockSignals(true);
    ui->odfCB->clear();
    ui->odfCB->setEnabled(false);
    ui->odfCB->blockSignals(false);

    ui->posCB->blockSignals(true);
    ui->posCB->clear();
    ui->posCB->setEnabled(false);
    ui->posCB->blockSignals(false);


    int area = index + 2;
    QSqlQuery *query = dbMan->selectExchanges(area);
    int id;
    QString abbr;
    while(query->next())
    {
        id = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr, id);
    }

    ui->abbrCB->setCurrentIndex(-1);
    if(ui->abbrCB->count() > 0)
        ui->abbrCB->setEnabled(true);

    ui->abbrCB->blockSignals(false);

    if(SEARCH_FLAG)
    {
        int id = ui->abbrCB->findData(abbrId);
        if(id > -1) // exch
        {
            ui->abbrCB->setCurrentIndex(id);
            exchId = abbrId;
            siteId = -1;
        }
        else // site ID
        {// find exch of site
            exchId = dbMan->getSiteMotherId(abbrId);
            siteId = abbrId;
            ui->abbrCB->setCurrentIndex(ui->abbrCB->findData(exchId));
        }
    }
}

void ODFManagementDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    tableModel->clear();
    ui->odfLbl->clear();

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);

    ui->odfCB->blockSignals(true);
    ui->odfCB->clear();
    ui->odfCB->setEnabled(false);
    ui->odfCB->blockSignals(false);

    ui->posCB->blockSignals(true);
    ui->posCB->clear();
    ui->posCB->setEnabled(false);
    ui->posCB->blockSignals(false);

    ui->saloonCB->blockSignals(true);
    ui->saloonCB->clear();
    ui->saloonCB->setEnabled(false);
    ui->saloonCB->blockSignals(false);

    ui->typeCB->setEnabled(true);
    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);
    ui->typeCB->setCurrentIndex(0);


    if(SEARCH_FLAG)
    {
        if(siteId == -1)// exch
            ui->typeCB->setCurrentIndex(0);
        else
            ui->typeCB->setCurrentIndex(1);
    }
}

void ODFManagementDialog::on_typeCB_currentIndexChanged(int index)
{
    tableModel->clear();
    ui->odfLbl->clear();
    if(index == 1) // site
    {
        ui->odfCB->blockSignals(true);
        ui->odfCB->clear();
        ui->odfCB->setEnabled(false);
        ui->odfCB->blockSignals(false);

        ui->posCB->blockSignals(true);
        ui->posCB->clear();
        ui->posCB->setEnabled(false);
        ui->posCB->blockSignals(false);

        ui->siteFrame->setEnabled(true);
        ui->siteFrame->setVisible(true);
        ui->siteCB->setEnabled(true);
        ui->siteCB->blockSignals(true);
        ui->siteCB->clear();

        ui->saloonCB->setEnabled(true);
        ui->saloonCB->blockSignals(true);
        ui->saloonCB->clear();
        QSqlQuery *query = dbMan->selectSaloons(true,false);
        int saloonId;
        QString saloonStr;
        while(query->next())
        {
            saloonId = query->value(0).toInt();
            saloonStr = query->value(1).toString();
            ui->saloonCB->addItem(saloonStr, saloonId);
        }
        ui->saloonCB->setCurrentIndex(0);
        ui->saloonCB->blockSignals(false);

        QString mother = ui->abbrCB->currentText();
        int area = ui->areaCB->currentData().toInt();
        query = dbMan->selectSites(area, mother);
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

        if(SEARCH_FLAG)
        {
            ui->siteCB->setCurrentIndex(ui->siteCB->findData(siteId));
        }
    }
    else // exch
    {

        ui->odfCB->blockSignals(true);
        ui->odfCB->clear();
        ui->odfCB->setEnabled(false);
        ui->odfCB->blockSignals(false);

        ui->posCB->blockSignals(true);
        ui->posCB->clear();
        ui->posCB->setEnabled(false);
        ui->posCB->blockSignals(false);

        ui->siteCB->blockSignals(true);
        ui->siteCB->clear();
        ui->siteCB->blockSignals(false);
        ui->siteFrame->setEnabled(false);
        ui->siteFrame->setVisible(false);

        ui->saloonCB->blockSignals(true);
        ui->saloonCB->clear();
        exchId = ui->abbrCB->currentData().toInt();
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



        if(SEARCH_FLAG)
        {
            ui->saloonCB->setCurrentIndex(ui->saloonCB->findData(saloon));
        }

    }
}

void ODFManagementDialog::on_siteCB_currentIndexChanged(int index)
{
    index++;
    tableModel->clear();
    ui->odfLbl->clear();
    odfPPP.clear();
    int exchId = ui->siteCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectOdf(exchId);
    // `id`,`odf_no`,`pos_count`,`pin_per_pos`
    ui->odfCB->setEnabled(true);
    ui->odfCB->blockSignals(true);
    ui->odfCB->clear();

    ui->posCB->setEnabled(false);
    ui->posCB->blockSignals(true);
    ui->posCB->clear();
    ui->posCB->blockSignals(false);

    int OdfId;
    QString odfNo,ppp;
    while(query->next())
    {
        OdfId = query->value(0).toInt();
        odfNo = query->value(1).toString();
        ppp = query->value(3).toString()+" Pin Per Pos";

        ui->odfCB->addItem(odfNo, OdfId);
        odfPPP.insert(OdfId,ppp);
    }
    ui->odfCB->setCurrentIndex(-1);
    ui->odfCB->blockSignals(false);
    if(ui->odfCB->count() > 0)
        ui->odfCB->setCurrentIndex(0);

    if(SEARCH_FLAG)
    {
        ui->odfCB->setCurrentIndex(ui->odfCB->findData(odfId));
    }
}

void ODFManagementDialog::on_saloonCB_currentIndexChanged(int index)
{
    index++;
    tableModel->clear();
    ui->odfLbl->clear();
    odfPPP.clear();
    int exchId = ui->abbrCB->currentData().toInt();
    int saloon = ui->saloonCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectOdf(exchId, saloon, true);
    // `id`,`odf_row`, `odf_column`,`odf_no`,`pos_count`,`pin_per_pos`
    ui->odfCB->setEnabled(true);
    ui->odfCB->blockSignals(true);
    ui->odfCB->clear();

    ui->posCB->blockSignals(true);
    ui->posCB->clear();
    ui->posCB->setEnabled(false);
    ui->posCB->blockSignals(false);


    int OdfId;
    QString odfNo,ppp;
    while(query->next())
    {
        OdfId = query->value(0).toInt();
        odfNo = query->value(3).toString();
        ppp = query->value(5).toString()+" Pin Per Pos";

        ui->odfCB->addItem(odfNo, OdfId);
        odfPPP.insert(OdfId,ppp);
    }
    ui->odfCB->setCurrentIndex(-1);
    ui->odfCB->blockSignals(false);
    if(ui->odfCB->count() > 0)
        ui->odfCB->setCurrentIndex(0);

    if(SEARCH_FLAG)
    {
        ui->odfCB->setCurrentIndex(ui->odfCB->findData(odfId));
    }
}

void ODFManagementDialog::contextMenuSlot(QPoint ptr)
{
    QString dep = QString(DEPARTMENT);
    if(!(dep.compare("tarahi") == 0))
    {
        return;
    }
    clickedIndex = ui->pinTV->indexAt(ptr);
    clickedRow = clickedIndex.row();
    //`id`,`port_id`,`local_device`,`local_label`,`pin_no`,`remote`,`remote_label`

    QModelIndex idIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // portId

    if( (tableModel->data(clickedIndex).isNull()) && (!tableModel->data(idIndex).isNull()) )
    {
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
        clickedPinId = tableModel->data(clickedIndex).toInt();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // portId
        clickedPortId = tableModel->data(clickedIndex).toInt();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // local device
        clickedLocalDevice = tableModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3); // local label
        clickedLocalLabel = tableModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 4); // pin no
        clickedPinNo = tableModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 5); // remote
        clickedRemote = tableModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 6); // remote label
        clickedRemoteLabel = tableModel->data(clickedIndex).toString();


        if(clickedLocalDevice.isEmpty() && clickedLocalLabel.isEmpty() && clickedRemote.isEmpty() && clickedRemoteLabel.isEmpty())
        {
            ui->actionAssignODFPin->setEnabled(true);
            ui->actionEditODFPin->setEnabled(false);
            ui->actionDepleteODFPin->setEnabled(false);
        }
        else
        {
            ui->actionAssignODFPin->setEnabled(false);
            ui->actionEditODFPin->setEnabled(true);
            ui->actionDepleteODFPin->setEnabled(true);
        }

        contextMenu.popup(ui->pinTV->mapToGlobal(ptr));
    }
}

void ODFManagementDialog::pinTV2ClickSlot(QModelIndex ind)
{
    QString dep = QString(DEPARTMENT);
    if(!(dep.compare("tarahi") == 0))
    {
        return;
    }
    clickedIndex = ind;
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // portId
    ind = ind.sibling(ind.row(), 0); // id

    if( (tableModel->data(clickedIndex).isNull()) && (!tableModel->data(ind).isNull()) )
    {
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
        clickedPinId = tableModel->data(clickedIndex).toInt();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // portId
        clickedPortId = tableModel->data(clickedIndex).toInt();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // local device
        clickedLocalDevice = tableModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3); // local label
        clickedLocalLabel = tableModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 4); // pin no
        clickedPinNo = tableModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 5); // remote
        clickedRemote = tableModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 6); // remote label
        clickedRemoteLabel = tableModel->data(clickedIndex).toString();


        if(clickedLocalDevice.isEmpty() && clickedLocalLabel.isEmpty() && clickedRemote.isEmpty() && clickedRemoteLabel.isEmpty())
            ui->actionAssignODFPin->trigger();
        else
        {
            ui->actionEditODFPin->trigger();
        }
    }
}

void ODFManagementDialog::on_odfCB_currentIndexChanged(int index)
{
    index++;
    tableModel->clear();
    int OdfId = ui->odfCB->currentData().toInt();
    ui->odfLbl->setText(odfPPP.value(OdfId));

    ui->posCB->blockSignals(true);
    ui->posCB->clear();
    ui->posCB->setEnabled(true);


    QSqlQuery *query = dbMan->selectExchageOdfPos(OdfId);
    //`id`, `odf_id`, `pos_no`
    int PosId;
    QString posNo;
    while(query->next())
    {
        PosId = query->value(0).toInt();
        posNo = query->value(2).toString();
        ui->posCB->addItem(posNo, PosId);
    }
    ui->posCB->setCurrentIndex(-1);
    ui->posCB->blockSignals(false);

    if(SEARCH_FLAG)
    {
        ui->posCB->setCurrentIndex(ui->posCB->findData(posId));
    }
}

void ODFManagementDialog::on_posCB_currentIndexChanged(int index)
{
    index++;
    int posId = ui->posCB->currentData().toInt();

    fillTable(posId);

    if(SEARCH_FLAG)
    {

        int id;
        for(int i = 0; i < tableModel->rowCount(); i++)
        {
            id = tableModel->record(i).value(0).toInt();
            if(id == pinId)
            {
                ui->pinTV->selectRow(i);
                break;
            }
        }


        SEARCH_FLAG = false;
    }

    ui->pinTV->setFocus();

}

void ODFManagementDialog::on_actionAssignODFPin_triggered()
{
    //DanetDbMan *db,const int PinId, QString exch, QString saloon, QString odf, QString pos, QString pin
    QString exch = ui->areaCB->currentText() + "-"+ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3)//site
        exch = exch + "-"+ui->siteCB->currentText();
    QString saloon = ui->saloonCB->currentText();
    QString odf = ui->odfCB->currentText();
    QString pos = ui->posCB->currentText();

    AssignOdfPinDialog *assignOdfPinDialog = new AssignOdfPinDialog(this, dbMan, clickedPinId,exch,saloon,odf,pos, clickedPinNo);
    assignOdfPinDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(assignOdfPinDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    assignOdfPinDialog->show();
}

void ODFManagementDialog::on_actionDepleteODFPin_triggered()
{
    //DanetDbMan *db,const int PinId, QString exch, QString saloon, QString odf, QString pos, QString pin
    QString exch = ui->areaCB->currentText() + "-"+ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3)//site
        exch = exch + "-"+ui->siteCB->currentText();
    QString saloon = ui->saloonCB->currentText();
    QString odf = ui->odfCB->currentText();
    QString pos = ui->posCB->currentText();

    DepleteOdfPinDialog *depleteOdfPinDialog = new DepleteOdfPinDialog(this, dbMan, clickedPinId,exch,saloon,odf,pos, clickedPinNo, clickedLocalDevice, clickedLocalLabel, clickedRemote, clickedRemoteLabel);
    depleteOdfPinDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(depleteOdfPinDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    depleteOdfPinDialog->show();
}

void ODFManagementDialog::on_actionEditODFPin_triggered()
{
    //DanetDbMan *db,const int PinId, QString exch, QString saloon, QString odf, QString pos, QString pin
    QString exch = ui->areaCB->currentText() + "-"+ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3)//site
        exch = exch + "-"+ui->siteCB->currentText();
    QString saloon = ui->saloonCB->currentText();
    QString odf = ui->odfCB->currentText();
    QString pos = ui->posCB->currentText();

    EditOdfPinDialog *editOdfPinDialog = new EditOdfPinDialog(this, dbMan, clickedPinId,exch,saloon,odf,pos, clickedPinNo, clickedLocalDevice, clickedLocalLabel, clickedRemote, clickedRemoteLabel);
    editOdfPinDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editOdfPinDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editOdfPinDialog->show();
}
