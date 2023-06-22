#include "vsirefdialog.h"
#include "ui_vsirefdialog.h"
#include "lib/headerview.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "mainPanel/menu/plan/vsiRef/addvsirefdialog.h"
#include "mainPanel/menu/plan/vsiRef/editvsirefdialog.h"
#include "mainPanel/menu/plan/vsiRef/removevsirefdialog.h"
#include "lib/report/general/Report.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>

// table index
#define ID 0
#define VSI 1
#define DESC 2

VsiRefDialog::VsiRefDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::VsiRefDialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(3,VSI, ui->vsiTV);
    ui->vsiTV->setHorizontalHeader(header);
    ui->vsiTV->setModel(proxyModel);
    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));


    header->hideWidgets(ID);


    header->hideComboBox(VSI);
    header->setLableText(VSI, "VSI");
    header->setLineEditPlaceHolder(VSI, "Search VSI.");


    header->hideComboBox(DESC);
    header->setLableText(DESC, "Description");
    header->setLineEditPlaceHolder(DESC, "Search Desc");

    fillTV(true);

    ui->vsiTV->verticalHeader()->hide();
    ui->vsiTV->horizontalHeader()->setStretchLastSection(true);
    ui->vsiTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    /////////////////////

    ui->vsiTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->vsiTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionEditVsiRef);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemoveVsiRef);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionExport_PDF);
}

VsiRefDialog::~VsiRefDialog()
{
    delete ui;
}

void VsiRefDialog::fillTV(bool RESET)
{
    QSqlQuery *query = dbMan->selectVsiRef();

    //header->blockSignals(true);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*query);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->vsiTV->hideColumn(ID);


    if(RESET)
    {
        header->resetHeaderWidgets();
    }

    //header->blockSignals(false);
}

void VsiRefDialog::refreshSlot(int done)
{
    done++;
    fillTV(false);
    ui->vsiTV->selectRow(row);
}

void VsiRefDialog::refreshSlotReset(int done)
{
    done++;
    fillTV(true);
    ui->vsiTV->selectRow(row);
}

void VsiRefDialog::headerWidgetsChagedSlot()
{
    vsi = header->sections[VSI]->lineEdit->text().toInt();
    desc = header->sections[DESC]->lineEdit->text();

    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(vsi != 0)
        mapString.insert(VSI, QString::number(vsi));

    if(!desc.isEmpty())
    {
        reg.setPattern(desc);
        mapRegExp.insert(DESC, reg);
    }

    proxyModel->setColumnsFilter(mapString, mapRegExp);
    ui->vsiTV->hideColumn(ID);

}

void VsiRefDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->vsiTV->indexAt(ptr);
    row = clickedIndex.row();
    ptr.setY(ptr.y() + 80);

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID); // id
    id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), VSI); // area
    vsi = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESC); // exchange
    desc = proxyModel->data(clickedIndex).toString();

    if(id > 0)
        contextMenu.popup(ui->vsiTV->mapToGlobal(ptr));
}

void VsiRefDialog::on_actionEditVsiRef_triggered()
{
    EditVsiRefDialog *editVsiRefDialog = new EditVsiRefDialog(this, dbMan, id, vsi,desc);
    editVsiRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editVsiRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editVsiRefDialog->show();
}

void VsiRefDialog::on_actionRemoveVsiRef_triggered()
{
    RemoveVsiRefDialog *removeVsiRefDialog = new RemoveVsiRefDialog(this, dbMan,id, vsi,desc );
    removeVsiRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeVsiRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlotReset(int)));
    removeVsiRefDialog->show();
}

void VsiRefDialog::on_addBtn_clicked()
{
    AddVsiRefDialog *addVsiRefDialog = new AddVsiRefDialog(this,dbMan);
    addVsiRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addVsiRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addVsiRefDialog->show();
}

void VsiRefDialog::on_actionExport_PDF_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet VSI Reference As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf")) path += ".pdf";

        QVector<int> columnStretch = QVector<int>() <<0<<20<<50;
        QVector<QString> headers = QVector<QString>() << "ID" << "VSI"<<"Description";

        Report reportVsiRef(this,proxyModel,headers,columnStretch, path, "VSI Reference");
        if(reportVsiRef.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportVsiRef.lastError());
        }
    }

}
