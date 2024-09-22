#include "vlanrefdialog.h"
#include "ui_vlanrefdialog.h"
#include "lib/headerview.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "mainPanel/menu/plan/vlanRef/addvlanrefdialog.h"
#include "mainPanel/menu/plan/vlanRef/editvlanrefdialog.h"
#include "mainPanel/menu/plan/vlanRef/removevlanrefdialog.h"
#include "lib/report/general/Report.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>

// table index
#define ID 0
#define VLAN 1
#define DESC 2

VlanRefDialog::VlanRefDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::VlanRefDialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(3,VLAN, ui->vlanTV);
    ui->vlanTV->setHorizontalHeader(header);
    ui->vlanTV->setModel(proxyModel);

    header->hideWidgets(ID);

    header->hideLineEdit(VLAN);
    header->setLableText(VLAN, "VLAN");
    header->addComboBoxItem(VLAN,"xx", 0);
    header->addComboBoxItem(VLAN,"1xx", 100);
    header->addComboBoxItem(VLAN,"2xx", 200);
    header->addComboBoxItem(VLAN,"3xx", 300);
    header->addComboBoxItem(VLAN,"4xx", 400);
    header->addComboBoxItem(VLAN,"5xx", 500);
    header->addComboBoxItem(VLAN,"6xx", 600);
    header->addComboBoxItem(VLAN,"7xx", 700);
    header->addComboBoxItem(VLAN,"8xx", 800);
    header->addComboBoxItem(VLAN,"9xx", 900);
    header->addComboBoxItem(VLAN,"1xxx", 1000);
    header->addComboBoxItem(VLAN,"2xxx", 2000);
    header->addComboBoxItem(VLAN,"3xxx", 3000);
    header->addComboBoxItem(VLAN,"4xxx", 4000);

    header->hideComboBox(DESC);
    header->setLableText(DESC, "Description");
    header->setLineEditPlaceHolder(DESC, "Search Desc.");

    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));

    fillTV(true, 0);

    ui->vlanTV->verticalHeader()->hide();
    ui->vlanTV->horizontalHeader()->setStretchLastSection(true);
    ui->vlanTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    /////////////////////

    ui->vlanTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->vlanTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionEditVlanRef);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemoveVlanRef);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionExport_PDF);
}

VlanRefDialog::~VlanRefDialog()
{
    delete ui;
}

void VlanRefDialog::fillTV(bool RESET, const int Range)
{
    currentRange = Range;
    QSqlQuery *query = dbMan->selectVlanRef(Range);

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*query);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->vlanTV->hideColumn(ID);



    if(RESET)
    {
        header->resetHeaderWidgets();
    }

    header->setComboBoxCurrentData(VLAN,Range);

}

void VlanRefDialog::refreshSlot(int done)
{
    done++;
    fillTV(false, currentRange);
    ui->vlanTV->selectRow(row);
}

void VlanRefDialog::refreshSlotReset(int done)
{
    done++;
    fillTV(true, currentRange);
    ui->vlanTV->selectRow(row);
}

void VlanRefDialog::headerWidgetsChagedSlot()
{
    int range = header->sections[VLAN]->comboBox->currentData().toInt();
    desc = header->sections[DESC]->lineEdit->text();

    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(range != currentRange)
        fillTV(false,range);


    if(!desc.isEmpty())
    {
        reg.setPattern(desc);
        mapRegExp.insert(DESC, reg);
    }

    proxyModel->setColumnsFilter(mapString, mapRegExp);
    ui->vlanTV->hideColumn(ID);
}

void VlanRefDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->vlanTV->indexAt(ptr);
    row = clickedIndex.row();
    ptr.setY(ptr.y() + 80);

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID);
    id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), VLAN);
    vlan = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESC);
    desc = proxyModel->data(clickedIndex).toString();


    if(id > 0)
        contextMenu.popup(ui->vlanTV->mapToGlobal(ptr));
}

void VlanRefDialog::on_actionEditVlanRef_triggered()
{
    EditVlanRefDialog *editVlanRefDialog = new EditVlanRefDialog(this, dbMan, id, vlan, desc);
    editVlanRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editVlanRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editVlanRefDialog->show();
}

void VlanRefDialog::on_actionRemoveVlanRef_triggered()
{
    RemoveVlanRefDialog *removeVlanRefDialog = new RemoveVlanRefDialog(this, dbMan,id, vlan,desc );
    removeVlanRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeVlanRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlotReset(int)));
    removeVlanRefDialog->show();
}

void VlanRefDialog::on_addBtn_clicked()
{
    AddVlanRefDialog *addVlanRefDialog = new AddVlanRefDialog(this,dbMan);
    addVlanRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addVlanRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlotReset(int)));
    addVlanRefDialog->show();
}

void VlanRefDialog::on_actionExport_PDF_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Vlan Reference As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf")) path += ".pdf";

        QVector<int> columnStretch = QVector<int>() <<0<<20<<50;
        QVector<QString> headers = QVector<QString>() << "ID" << "VLAN"<<"Description";

        Report reportVlanRef(this,proxyModel,headers,columnStretch, path,"VLAN");
        if(reportVlanRef.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportVlanRef.lastError());
        }
    }

}
