#include "rdrefdialog.h"
#include "ui_rdrefdialog.h"
#include "lib/headerview.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "mainPanel/menu/plan/rdRef/addrdrefdialog.h"
#include "mainPanel/menu/plan/rdRef/editrdrefdialog.h"
#include "mainPanel/menu/plan/rdRef/removerdrefdialog.h"
#include "lib/report/general/Report.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>

// table index
#define ID 0
#define RD 1
#define DESC 2

RdRefDialog::RdRefDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::RdRefDialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(3,RD, ui->rdTV);
    ui->rdTV->setHorizontalHeader(header);
    ui->rdTV->setModel(proxyModel);
    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));


    header->hideWidgets(ID);

    header->hideComboBox(RD);
    header->setLableText(RD, "RD");
    header->setLineEditPlaceHolder(RD,"Search RD");

    header->hideComboBox(DESC);
    header->setLableText(DESC, "Description");
    header->setLineEditPlaceHolder(DESC, "Search Desc.");

    fillTV(true);

    ui->rdTV->verticalHeader()->hide();
    ui->rdTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->rdTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    /////////////////////

    ui->rdTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->rdTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionEditRdRef);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemoveRdRef);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionExport_PDF);
}

RdRefDialog::~RdRefDialog()
{
    delete ui;
}

void RdRefDialog::fillTV(bool RESET)
{
    QSqlQuery *query = dbMan->selectRdRef();

    //header->blockSignals(true);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*query);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->rdTV->hideColumn(ID);

    if(RESET)
    {
        header->resetHeaderWidgets();
    }

    //header->blockSignals(false);
}


void RdRefDialog::refreshSlot(int done)
{
    done++;
    fillTV(false);
    ui->rdTV->selectRow(row);
}

void RdRefDialog::refreshSlotReset(int done)
{
    done++;
    fillTV(true);
    ui->rdTV->selectRow(row);
}

void RdRefDialog::headerWidgetsChagedSlot()
{
    rd = header->sections[RD]->lineEdit->text();
    desc = header->sections[DESC]->lineEdit->text();

    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(!rd.isEmpty())
    {
        reg.setPattern(rd);
        mapRegExp.insert(RD, reg);
    }

    if(!desc.isEmpty())
    {
        reg.setPattern(desc);
        mapRegExp.insert(DESC, reg);
    }

    proxyModel->setColumnsFilterRegExp(mapRegExp);
    ui->rdTV->hideColumn(ID);

}

void RdRefDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->rdTV->indexAt(ptr);
    row = clickedIndex.row();
    ptr.setY(ptr.y() + 80);

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID);
    id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), RD);
    rd = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESC);
    desc = proxyModel->data(clickedIndex).toString();

    if(id > 0)
        contextMenu.popup(ui->rdTV->mapToGlobal(ptr));
}

void RdRefDialog::on_actionEditRdRef_triggered()
{
    EditRdRefDialog *editRdRefDialog = new EditRdRefDialog(this, dbMan, id, rd, desc);
    editRdRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editRdRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editRdRefDialog->show();
}

void RdRefDialog::on_actionRemoveRdRef_triggered()
{
    RemoveRdRefDialog *removeRdRefDialog = new RemoveRdRefDialog(this, dbMan,id, rd,desc );
    removeRdRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeRdRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlotReset(int)));
    removeRdRefDialog->show();
}

void RdRefDialog::on_addBtn_clicked()
{
    AddRdRefDialog *addRdRefDialog = new AddRdRefDialog(this,dbMan);
    addRdRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addRdRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlotReset(int)));
    addRdRefDialog->show();
}

void RdRefDialog::on_actionExport_PDF_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet RD Reference Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf")) path += ".pdf";
        QVector<int> columnStretch = QVector<int>() <<0<<20<<50;
        QVector<QString> headers = QVector<QString>() << "ID" << "RD"<<"Description";

        Report reportRdRef(this,proxyModel, headers, columnStretch, path,"Route Distinguisher");
        if(reportRdRef.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportRdRef.lastError());
        }
    }

}
