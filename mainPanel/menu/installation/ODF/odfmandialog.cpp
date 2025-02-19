#include "odfmandialog.h"
#include "ui_odfmandialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include "lib/centeralignedmodel.h"
//ODF
#include "mainPanel/menu/installation/ODF/addodfdialog.h"
#include "mainPanel/menu/installation/ODF/removeodfdialog.h"
#include "mainPanel/menu/installation/ODF/editodfdialog.h"
//POS
#include "mainPanel/menu/installation/ODF/addposdialog.h"
#include "mainPanel/menu/installation/ODF/removeposdialog.h"
#include "mainPanel/menu/installation/ODF/editposnodialog.h"

OdfManDialog::OdfManDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::OdfManDialog),
    dbMan(db),
    odfModel(new CenterAlignedModel(this, 3)),
    posModel(new CenterAlignedModel(this)),
    addOdfDialog(nullptr),
    removeOdfDialog(nullptr),
    editOdfDialog(nullptr),
    addPosDialog(nullptr),
    removePosDialog(nullptr),
    editPosNoDialog(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->siteFrame->setVisible(false);

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
    ui->typeCB->setEnabled(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);


    ui->saloonCB->blockSignals(true);
    ui->saloonCB->clear();
    ui->saloonCB->blockSignals(false);
    ui->saloonCB->setEnabled(false);

    //odf
    ui->odfTV->setModel(odfModel);
    ui->odfTV->verticalHeader()->hide();
    ui->odfTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // pos
    ui->posTV->setModel(posModel);
    ui->posTV->verticalHeader()->hide();
    ui->posTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QString dep = QString(DEPARTMENT);
    if((dep.compare("tarahi") == 0))
    {
        // context menu ODF
        ui->odfTV->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->odfTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(odfContextMenuSlot(QPoint)));
        connect(ui->odfTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(odfTV2ClickSlot(QModelIndex)));

        odfContextMenu.addAction(ui->actionEditODF);
        odfContextMenu.addSeparator();
        odfContextMenu.addAction(ui->actionRemoveODF);

        // context menu POS
        ui->posTV->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->posTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(posContextMenuSlot(QPoint)));
        posContextMenu.addAction(ui->actionRemovePOS);
        posContextMenu.addAction(ui->actionChangePosNo);
    }

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));
}

OdfManDialog::~OdfManDialog()
{
    delete ui;
    delete odfModel;
    delete posModel;
    odfModel = nullptr;
    posModel = nullptr;
}

void OdfManDialog::fillOdfTV(int exchSiteId, int saloon)
{
    QSqlQuery *query = dbMan->selectOdf(exchSiteId, saloon);
    //`id`,`odf_row`, `odf_column`,`odf_no`,`pos_count`,`pin_per_pos`
    odfModel->setQuery(*query);
    ui->odfTV->hideColumn(0);
    odfModel->setHeaderData(1, Qt::Horizontal,"Saloon Row");
    odfModel->setHeaderData(2, Qt::Horizontal,"Saloon Column");
    odfModel->setHeaderData(3, Qt::Horizontal,"ODF NO");
    odfModel->setHeaderData(4, Qt::Horizontal,"POS Count");
    odfModel->setHeaderData(5, Qt::Horizontal,"PIN Per POS");

    //saloon
    ui->saloonCB->blockSignals(true);
    ui->saloonCB->setCurrentIndex(ui->saloonCB->findData(saloon));
    ui->saloonCB->blockSignals(false);

    int odfNo, id;
    ui->odfNoCB->blockSignals(true);
    ui->odfNoCB->clear();
    while(query->next())
    {
        odfNo = query->value(3).toInt();
        id = query->value(0).toInt();
        ui->odfNoCB->addItem(QString::number(odfNo), id);
    }
    ui->odfNoCB->setCurrentIndex(-1);
    ui->odfNoCB->blockSignals(false);

    if((ui->odfNoCB->count()) > 0)
    {
        ui->posTab->setEnabled(true);
        ui->odfTV->setEnabled(true);
        ui->odfNoCB->setCurrentIndex(0);
    }
    else
    {
        ui->posTab->setEnabled(false);
        ui->odfTV->setEnabled(false);
    }

    QString dep = QString(DEPARTMENT);
    if(!(dep.compare("tarahi") == 0))
    {
        ui->odfAddBtn->setEnabled(false);
        ui->posAddBtn->setEnabled(false);
    }
}

void OdfManDialog::fillPosTV(int i)
{
    i++;
    int odfId = ui->odfNoCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectExchageOdfPos(odfId);
    posModel->setQuery(*query);
    ui->posTV->hideColumn(0);
    ui->posTV->hideColumn(1);
    posModel->setHeaderData(2, Qt::Horizontal,"POS NO");

    QString dep = QString(DEPARTMENT);
    if(!(dep.compare("tarahi") == 0))
    {
        ui->odfAddBtn->setEnabled(false);
        ui->posAddBtn->setEnabled(false);
    }
}

void OdfManDialog::removeRefreshSlot(int i)
{
    i++;
    int exchangeId;
    if(ui->typeCB->currentIndex() == 0)//exchange
        exchangeId = ui->abbrCB->currentData().toInt();
    else
        exchangeId = ui->siteCB->currentData().toInt();

    int saloonNo = ui->saloonCB->currentData().toInt();


    fillOdfTV(exchangeId, saloonNo);

    ui->saloonCB->blockSignals(true);
    ui->saloonCB->clear();
    QList<QString> list = dbMan->getExistOdfSaloonNo(exchangeId);
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

    if(ui->saloonCB->count() > 0)
        ui->saloonCB->setCurrentIndex(0);
}

void OdfManDialog::addRefreshSlot(int i)
{
    i++;
    int exchangeId;
    if(ui->typeCB->currentIndex() == 0)//exchange
        exchangeId = ui->abbrCB->currentData().toInt();
    else
        exchangeId = ui->siteCB->currentData().toInt();

    if(addOdfDialog->addedOrNot())
    {
        int saloonNo = addOdfDialog->getSaloon();
        fillOdfTV(exchangeId, saloonNo);
        ui->saloonCB->blockSignals(true);
        ui->saloonCB->clear();
        QList<QString> list = dbMan->getExistOdfSaloonNo(exchangeId);
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

        ui->saloonCB->setCurrentIndex(ui->saloonCB->findData(saloonNo));
    }
}

void OdfManDialog::editRefreshSlot(int i)
{
    i++;
    int exchangeId;
    if(ui->typeCB->currentIndex() == 0)//exchange
        exchangeId = ui->abbrCB->currentData().toInt();
    else
        exchangeId = ui->siteCB->currentData().toInt();

    if(editOdfDialog->changedOrNot())
    {
        int saloonNo = editOdfDialog->getSaloon();
        fillOdfTV(exchangeId, saloonNo);
        ui->saloonCB->blockSignals(true);
        ui->saloonCB->clear();
        QList<QString> list = dbMan->getExistOdfSaloonNo(exchangeId);
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

        ui->saloonCB->setCurrentIndex(ui->saloonCB->findData(saloonNo));
    }

    ui->odfTV->selectRow(clickedRow);
}

void OdfManDialog::refreshPosSlot(int i)
{
    i++;
    fillPosTV(1);
    ui->posTV->selectRow(clickedRow);
}

void OdfManDialog::on_areaCB_currentIndexChanged(int index)
{
    ui->tabWidget->setEnabled(false);

    odfModel->clear();
    posModel->clear();

    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();

    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);
    ui->typeCB->setEnabled(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);


    ui->saloonCB->blockSignals(true);
    ui->saloonCB->clear();
    ui->saloonCB->blockSignals(false);
    ui->saloonCB->setEnabled(false);

    ui->odfNoCB->blockSignals(true);
    ui->odfNoCB->clear();
    ui->odfNoCB->blockSignals(false);

    int area = index + 2;
    QSqlQuery *query = dbMan->selectExchanges(area);
    //`id`,`abbr`
    int i;
    QString a;
    while(query->next())
    {
        i = query->value(0).toInt();
        a = query->value(1).toString();

        ui->abbrCB->addItem(a, i);
    }

    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);
    ui->tabWidget->setCurrentIndex(0);

}

void OdfManDialog::on_abbrCB_currentIndexChanged(const QString &arg1)
{
    ui->tabWidget->setEnabled(false);

    odfModel->clear();
    posModel->clear();

    ui->odfNoCB->blockSignals(true);
    ui->odfNoCB->clear();
    ui->odfNoCB->blockSignals(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);
    ui->siteCB->setEnabled(false);

    ui->saloonCB->blockSignals(true);
    ui->saloonCB->clear();
    ui->saloonCB->setCurrentIndex(-1);
    ui->saloonCB->blockSignals(false);
    ui->saloonCB->setEnabled(false);

    mother = arg1;

    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);
    ui->typeCB->setEnabled(true);
    ui->typeCB->setCurrentIndex(0);

    ui->tabWidget->setCurrentIndex(0);
}

void OdfManDialog::on_typeCB_currentIndexChanged(int index)
{
    ui->tabWidget->setEnabled(false);

    odfModel->clear();
    posModel->clear();

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);

    ui->saloonCB->blockSignals(true);
    ui->saloonCB->clear();
    ui->saloonCB->blockSignals(false);
    ui->saloonCB->setEnabled(false);

    ui->odfNoCB->blockSignals(true);
    ui->odfNoCB->clear();
    ui->odfNoCB->blockSignals(false);

    if(index == 1) // site
    {
        ui->siteFrame->setEnabled(true);
        ui->siteFrame->setVisible(true);


        int area = ui->areaCB->currentData().toInt();
        QString mother = ui->abbrCB->currentText();

        QSqlQuery *query = dbMan->selectSites(area, mother);
        //`id`,`abbr`, `exchange`
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
        ui->siteCB->setEnabled(true);
    }
    else // exchange
    {
        ui->siteCB->blockSignals(true);
        ui->siteCB->clear();
        ui->siteCB->blockSignals(false);
        ui->siteFrame->setEnabled(false);
        ui->siteFrame->setVisible(false);

        ui->saloonCB->blockSignals(true);
        ui->saloonCB->clear();
        exchSiteId = ui->abbrCB->currentData().toInt();
        QList<QString> list = dbMan->getExistOdfSaloonNo(exchSiteId);
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

        if(ui->saloonCB->count() == 0)
            ui->tabWidget->setEnabled(true);
    }


    ui->tabWidget->setCurrentIndex(0);
}

void OdfManDialog::on_siteCB_currentIndexChanged(int index)
{
    odfModel->clear();
    posModel->clear();
    index++;
    if(ui->typeCB->currentData().toInt() == 3)
    {
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

        ui->saloonCB->setCurrentIndex(-1);
        ui->saloonCB->blockSignals(false);
        ui->saloonCB->setEnabled(true);
        if(ui->siteCB->count() > 0)
            ui->saloonCB->setCurrentIndex(0);

        if(ui->saloonCB->count() == 0)
            ui->tabWidget->setEnabled(true);
    }

    ui->tabWidget->setCurrentIndex(0);

}

void OdfManDialog::on_saloonCB_currentIndexChanged(int index)
{
    index++;
    ui->tabWidget->setEnabled(true);

    if(ui->typeCB->currentData().toInt() == 3) //site
        exchSiteId = ui->siteCB->currentData().toInt();
    else
        exchSiteId = ui->abbrCB->currentData().toInt();

    int saloonNo = ui->saloonCB->currentData().toInt();

    fillOdfTV(exchSiteId, saloonNo);
    ui->tabWidget->setCurrentIndex(0);
}

void OdfManDialog::odfTV2ClickSlot(QModelIndex index)
{
    index = index.sibling(index.row(), 0); // id
    odfId = odfModel->data(index).toInt();
    ui->odfNoCB->setCurrentIndex(ui->odfNoCB->findData(odfId));
    ui->tabWidget->setCurrentIndex(1);

}

void OdfManDialog::odfContextMenuSlot(QPoint ptr)
{
    //`id`,`odf_row`, `odf_column`,`odf_no`,`pos_count`,`pin_per_pos`


    QModelIndex clickedIndex = ui->odfTV->indexAt(ptr);
    clickedRow = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    odfId = odfModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); //
    row = odfModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); //
    column = odfModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3); // odf no
    odfNo = odfModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 4); // pos count
    posCount = odfModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 5); // ppp
    ppp = odfModel->data(clickedIndex).toInt();

    if(odfId > 0)
        odfContextMenu.popup(ui->odfTV->mapToGlobal(ptr));

}

void OdfManDialog::posContextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->posTV->indexAt(ptr);
    clickedRow = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // pos_id
    posId = posModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // pos_no
    posNo = posModel->data(clickedIndex).toInt();

    if(posId > 0)
        posContextMenu.popup(ui->posTV->mapToGlobal(ptr));
}

void OdfManDialog::on_odfNoCB_currentIndexChanged(int index)
{
    index++;
    posModel->clear();
    fillPosTV(1);
}

void OdfManDialog::on_odfAddBtn_clicked()
{

    int exchangeId;
    if(ui->typeCB->currentIndex() == 0)//exchange
        exchangeId = ui->abbrCB->currentData().toInt();
    else
        exchangeId = ui->siteCB->currentData().toInt();

    QString areaAbbr = ui->areaCB->currentText() +'-'+ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) //site
        areaAbbr += '-'+ui->siteCB->currentText();

    int saloonNo = ui->saloonCB->currentData().toInt();

    delete addOdfDialog;
    addOdfDialog = new AddOdfDialog(this,dbMan, exchangeId, areaAbbr, saloonNo);
    connect(addOdfDialog,SIGNAL(finished(int)),this,SLOT(addRefreshSlot(int)));
    addOdfDialog->show();
}

void OdfManDialog::on_actionRemoveODF_triggered()
{
    QString areaAbbr;

    areaAbbr = ui->areaCB->currentText() +'-'+ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) //site
        areaAbbr += '-'+ui->siteCB->currentText();
    int saloonNo = ui->saloonCB->currentData().toInt();

    int exchId = ui->abbrCB->currentData().toInt();
    delete removeOdfDialog;
    removeOdfDialog = new RemoveOdfDialog(this, dbMan, exchId, areaAbbr,saloonNo, odfId, odfNo, posCount, ppp);
    connect(removeOdfDialog,SIGNAL(finished(int)),this,SLOT(removeRefreshSlot(int)));
    removeOdfDialog->show();
}

void OdfManDialog::on_actionEditODF_triggered()
{
    QString areaAbbr = ui->areaCB->currentText() +'-'+ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) //site
        areaAbbr += '-'+ui->siteCB->currentText();
    int saloonNo = ui->saloonCB->currentData().toInt();

    int exchId;
    if(ui->typeCB->currentIndex() == 0)//exchange
        exchId = ui->abbrCB->currentData().toInt();
    else
        exchId = ui->siteCB->currentData().toInt();

    if(ui->typeCB->currentData().toInt() == 3)
        exchId = ui->siteCB->currentData().toInt();

    delete editOdfDialog;
    editOdfDialog = new EditOdfDialog(this, dbMan,exchId,areaAbbr,saloonNo,row,column, odfId, odfNo, posCount, ppp);
    connect(editOdfDialog,SIGNAL(finished(int)),this,SLOT(editRefreshSlot(int)));
    editOdfDialog->show();
}

//POS
void OdfManDialog::on_posAddBtn_clicked()
{
    if(ui->odfNoCB->count() == 0)
        return;

    int OdfId, OdfNo;


    QString AreaAbbr = ui->areaCB->currentText() +'-'+ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) //site
        AreaAbbr += '-'+ui->siteCB->currentText();

    int saloonNo = ui->saloonCB->currentData().toInt();
    OdfId = ui->odfNoCB->currentData().toInt();
    OdfNo = ui->odfNoCB->currentText().toInt();

    delete addPosDialog;
    addPosDialog = new AddPosDialog(this, dbMan, AreaAbbr,saloonNo, OdfId, OdfNo);
    connect(addPosDialog,SIGNAL(finished(int)),this,SLOT(fillPosTV(int)));
    addPosDialog->show();
}

void OdfManDialog::on_actionRemovePOS_triggered()
{
    int OdfNo;
    QString AreaAbbr = ui->areaCB->currentText() +'-'+ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) //site
        AreaAbbr += '-'+ui->siteCB->currentText();

    OdfNo = ui->odfNoCB->currentText().toInt();
    int saloon = ui->saloonCB->currentData().toInt();

    delete removePosDialog;
    removePosDialog = new RemovePosDialog(this, dbMan, AreaAbbr, saloon, posId, OdfNo, posNo);
    connect(removePosDialog,SIGNAL(finished(int)),this,SLOT(fillPosTV(int)));
    removePosDialog->show();
}

void OdfManDialog::on_actionChangePosNo_triggered()
{
    int OdfNo;
    QString AreaAbbr = ui->areaCB->currentText() +'-'+ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) //site
        AreaAbbr += '-'+ui->siteCB->currentText();

    OdfNo = ui->odfNoCB->currentText().toInt();

    int saloon = ui->saloonCB->currentData().toInt();

    delete editPosNoDialog;
    editPosNoDialog = new EditPosNoDialog(this,dbMan, AreaAbbr,saloon, posId, OdfNo, posNo);
    connect(editPosNoDialog,SIGNAL(finished(int)),this,SLOT(refreshPosSlot(int)));
    editPosNoDialog->show();
}
