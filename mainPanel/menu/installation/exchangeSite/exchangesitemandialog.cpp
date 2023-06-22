#include "exchangesitemandialog.h"
#include "ui_exchangesitemandialog.h"
#include "lib/headerview.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "mainPanel/menu/installation/exchangeSite/addexchangesitedialog.h"
#include "mainPanel/menu/installation/exchangeSite/editexchangesitedialog.h"
#include "mainPanel/menu/installation/exchangeSite/removeexchangesitedialog.h"
#include "lib/report/exchange/ReportExchange.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>


// table index
#define ID 0
#define AREA_NO 1
#define AREA 2
#define EXCHANGE 3
#define ABBR 4
#define TYPE_NO 5
#define TYPE 6
#define MOTHER_ID 7
#define MOTHER 8
#define UPLINK_ID 9
#define UPLINK 10
#define CASCADE 11
#define NODE 12
#define DATA_SALOON 13
#define SWITCH_SALOON 14
#define PCM_SALOON 15
#define ADDRESS 16

ExchangeSiteManDialog::ExchangeSiteManDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ExchangeSiteManDialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);
    this->setModal(false);

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(17,AREA, ui->exchSiteTV);
    ui->exchSiteTV->setHorizontalHeader(header);

    header->hideWidgets(ID);
    header->hideWidgets(AREA_NO);
    header->hideWidgets(TYPE_NO);
    header->hideWidgets(MOTHER_ID);
    header->hideWidgets(UPLINK_ID);
    header->hideWidgets(DATA_SALOON);
    header->hideWidgets(SWITCH_SALOON);
    header->hideWidgets(PCM_SALOON);

    header->hideLineEdit(AREA);
    header->setLableText(AREA, "Area");
    header->addComboBoxItem(AREA,"همه", 0);
    header->addComboBoxItem(AREA,"2", 2);
    header->addComboBoxItem(AREA,"3", 3);
    header->addComboBoxItem(AREA,"4", 4);
    header->addComboBoxItem(AREA,"5", 5);
    header->addComboBoxItem(AREA,"6", 6);
    header->addComboBoxItem(AREA,"7", 7);
    header->addComboBoxItem(AREA,"8", 8);
    header->addComboBoxItem(AREA,"کرج", 9);

    header->hideComboBox(EXCHANGE);
    header->setLableText(EXCHANGE, "Exch/Site");
    header->setLineEditPlaceHolder(EXCHANGE,"Search Exchange/Site");

    header->hideComboBox(ABBR);
    header->setLableText(ABBR, "Abbr");
    header->setLineEditPlaceHolder(ABBR, "Search Abbr.");

    header->hideLineEdit(TYPE);
    header->setLableText(TYPE,"Type");
    header->addComboBoxItem(TYPE,"همه",0);
    header->addComboBoxItem(TYPE,"اداره کل", 1);
    header->addComboBoxItem(TYPE,"مرکز", 2);
    header->addComboBoxItem(TYPE,"سایت", 3);

    header->hideComboBox(MOTHER);
    header->setLableText(MOTHER, "Mother\nExchange");
    header->setLineEditPlaceHolder(MOTHER, "Search");

    header->hideComboBox(UPLINK);
    header->setLableText(UPLINK, "Uplink\nExch/Site");
    header->setLineEditPlaceHolder(UPLINK, "Search");

    header->hideComboBox(CASCADE);
    header->setLableText(CASCADE, "Cascade\nNo");
    header->setLineEditPlaceHolder(CASCADE, "Search");

    header->hideComboBox(NODE);
    header->setLableText(NODE, "Node\nNo");
    header->setLineEditPlaceHolder(NODE, "Search");

    header->hideComboBox(ADDRESS);
    header->setLableText(ADDRESS, "Address");
    header->setLineEditPlaceHolder(ADDRESS, "Search Address");

    ui->exchSiteTV->setModel(proxyModel);
    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));


    workingArea = dbMan->getLoggedInDefaultArea();
    fillExchSiteTV(true);

    ui->exchSiteTV->verticalHeader()->hide();
    ui->exchSiteTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->exchSiteTV->horizontalHeader()->setSectionResizeMode(ABBR,QHeaderView::Stretch);
    //ui->exchSiteTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->exchSiteTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    /////////////////////

    ui->exchSiteTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->exchSiteTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionEditExchangeSite);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemoveExchangeSite);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionExport_PDF);
}

ExchangeSiteManDialog::~ExchangeSiteManDialog()
{
    delete ui;
}

void ExchangeSiteManDialog::fillExchSiteTV(bool RESET)
{
    QSqlQuery *query ;
    if(workingArea == 0)
        query= dbMan->selectViewExchanges();
    else
        query = dbMan->selectViewExchanges(workingArea);

    //`id`,`area_no`,`area`,`exchange`,`abbr`,`type_no`,`type`,`mother_exchange_id`,`mother_exchange_abbr`,`uplink_exchange_id`,`uplink_exchange_abbr`,`site_cascade`,`site_node`,data, switch, pcm,`address`

    //header->blockSignals(true);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*query);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->exchSiteTV->hideColumn(ID);
    ui->exchSiteTV->hideColumn(AREA_NO);
    ui->exchSiteTV->hideColumn(TYPE_NO);
    ui->exchSiteTV->hideColumn(MOTHER_ID);
    ui->exchSiteTV->hideColumn(UPLINK_ID);
    ui->exchSiteTV->hideColumn(DATA_SALOON);
    ui->exchSiteTV->hideColumn(SWITCH_SALOON);
    ui->exchSiteTV->hideColumn(PCM_SALOON);

    if(RESET)
    {
        header->resetHeaderWidgets();
        header->setComboBoxCurrentData(AREA,workingArea);
        header->setComboBoxCurrentData(TYPE,0);
    }

    //header->blockSignals(false);

    ui->exchSiteTV->verticalHeader()->hide();
    ui->exchSiteTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->exchSiteTV->horizontalHeader()->setSectionResizeMode(ABBR,QHeaderView::Stretch);
    //ui->exchSiteTV->horizontalHeader()->setSectionResizeMode(ADDRESS,QHeaderView::Stretch);

    ui->exchSiteTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


}

void ExchangeSiteManDialog::refreshSlot(int confirm)
{

    confirm++;
    fillExchSiteTV(false);
    ui->exchSiteTV->selectRow(row);
}

void ExchangeSiteManDialog::headerWidgetsChagedSlot()
{
    bool ok;
    area = header->sections[AREA]->comboBox->currentData().toInt();
    exch = header->sections[EXCHANGE]->lineEdit->text();
    abbr = header->sections[ABBR]->lineEdit->text();
    type_no = header->sections[TYPE]->comboBox->currentData().toInt();
    mother = header->sections[MOTHER]->lineEdit->text();
    uplink = header->sections[UPLINK]->lineEdit->text();
    cascadeString = header->sections[CASCADE]->lineEdit->text();
    nodeString = header->sections[NODE]->lineEdit->text();
    if(header->sections[CASCADE]->lineEdit->text().isEmpty())
        cascade = -1000;
    else
    {
        cascade = header->sections[CASCADE]->lineEdit->text().toInt(&ok);
        if(!ok) cascade = -2000;
    }

    if(header->sections[NODE]->lineEdit->text().isEmpty())
        node = -1000;
    else
    {
        node = header->sections[NODE]->lineEdit->text().toInt(&ok);
        if(!ok) node = -2000;
    }

    address = header->sections[ADDRESS]->lineEdit->text();


    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(area != workingArea)
    {
        workingArea = area;
        fillExchSiteTV(false);
//        mapString.insert(AREA_NO, QString::number(area));
    }
    if(type_no != 0)
        mapString.insert(TYPE_NO, QString::number(type_no));
//    if(cascade != -1000)
//        mapString.insert(CASCADE, QString::number(cascade));
//    if(node != -1000)
//        mapString.insert(NODE, QString::number(node));

    if(!address.isEmpty())
    {
        reg.setPattern(address);
        mapRegExp.insert(ADDRESS, reg);
    }
    if(!abbr.isEmpty())
    {
        reg.setPattern(abbr);
        mapRegExp.insert(ABBR, reg);
    }
    if(!exch.isEmpty())
    {
        reg.setPattern(exch);
        mapRegExp.insert(EXCHANGE, reg);
    }
    if(!mother.isEmpty())
    {
        reg.setPattern(mother);
        mapRegExp.insert(MOTHER, reg);
    }
    if(!uplink.isEmpty())
    {
        reg.setPattern(uplink);
        mapRegExp.insert(UPLINK, reg);
    }
    if(!cascadeString.isEmpty())
    {
        reg.setPattern(cascadeString);
        mapRegExp.insert(CASCADE, reg);
    }
    if(!nodeString.isEmpty())
    {
        reg.setPattern(nodeString);
        mapRegExp.insert(NODE, reg);
    }

    proxyModel->setColumnsFilter(mapString, mapRegExp);
    ui->exchSiteTV->hideColumn(ID);
    ui->exchSiteTV->hideColumn(AREA_NO);
    ui->exchSiteTV->hideColumn(TYPE_NO);
    ui->exchSiteTV->hideColumn(MOTHER_ID);
    ui->exchSiteTV->hideColumn(UPLINK_ID);
    ui->exchSiteTV->hideColumn(DATA_SALOON);
    ui->exchSiteTV->hideColumn(SWITCH_SALOON);
    ui->exchSiteTV->hideColumn(PCM_SALOON);

    ui->exchSiteTV->verticalHeader()->hide();
    ui->exchSiteTV->horizontalHeader()->setSectionResizeMode(EXCHANGE,QHeaderView::Stretch);
//    ui->exchSiteTV->horizontalHeader()->setSectionResizeMode(ABBR,QHeaderView::Stretch);
    ui->exchSiteTV->horizontalHeader()->setSectionResizeMode(ADDRESS,QHeaderView::Stretch);

    ui->exchSiteTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

void ExchangeSiteManDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->exchSiteTV->indexAt(ptr);
    row = clickedIndex.row();
    ptr.setY(ptr.y() + 80);

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID); // id
    id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), AREA_NO); // area
    area = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), EXCHANGE); // exchange
    exch = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ABBR); // abbr
    abbr = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), TYPE_NO); // type
    type_no = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), TYPE); // type string
    type = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), MOTHER_ID); // mother id
    motherId = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), MOTHER); // mother
    mother = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), UPLINK_ID); // uplink id
    uplinkId = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), UPLINK); // uplink
    uplink = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), CASCADE); // cascade
    cascade = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), NODE); // node
    node = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DATA_SALOON); //
    dataSaloon = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), SWITCH_SALOON); //
    switchSaloon = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), PCM_SALOON); //
    pcmSaloon = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ADDRESS); // address
    address = proxyModel->data(clickedIndex).toString();

    if(area > 0)
        contextMenu.popup(ui->exchSiteTV->mapToGlobal(ptr));
}

void ExchangeSiteManDialog::on_actionEditExchangeSite_triggered()
{
    EditExchangeSiteDialog *editExchangeSiteDialog = new EditExchangeSiteDialog(this, dbMan, id, area, exch, abbr, type_no,dataSaloon, switchSaloon, pcmSaloon,motherId,uplinkId, cascade, node, address);
    editExchangeSiteDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editExchangeSiteDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editExchangeSiteDialog->show();
}

void ExchangeSiteManDialog::on_actionRemoveExchangeSite_triggered()
{
    RemoveExchangeSiteDialog *removeExchangeSiteDialog = new RemoveExchangeSiteDialog(this, dbMan,id, area,exch,abbr, type );
    removeExchangeSiteDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeExchangeSiteDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    removeExchangeSiteDialog->show();
}

void ExchangeSiteManDialog::on_addBtn_clicked()
{
    AddExchangeSiteDialog *addExchangeSiteDialog = new AddExchangeSiteDialog(this,dbMan);
    addExchangeSiteDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addExchangeSiteDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addExchangeSiteDialog->show();
}

void ExchangeSiteManDialog::on_actionExport_PDF_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Exchange Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf")) path += ".pdf";

        path = path.replace(" ", "_");

        ReportExchange reportExchange(this,proxyModel, path);
        if(reportExchange.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportExchange.lastError());
        }
    }

}

