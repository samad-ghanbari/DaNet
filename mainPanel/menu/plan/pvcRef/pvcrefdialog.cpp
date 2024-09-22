#include "pvcrefdialog.h"
#include "ui_pvcrefdialog.h"
#include "lib/headerview.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "mainPanel/menu/plan/pvcRef/addpvcrefdialog.h"
#include "mainPanel/menu/plan/pvcRef/editpvcrefdialog.h"
#include "mainPanel/menu/plan/pvcRef/removepvcrefdialog.h"
#include "lib/report/general/Report.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>

// table index
#define ID 0
#define VPI 1
#define VCI 2
#define DESC 3


PvcRefDialog::PvcRefDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::PvcRefDialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(4,VPI, ui->pvcTv);
    ui->pvcTv->setHorizontalHeader(header);
    ui->pvcTv->setModel(proxyModel);
    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));


    header->hideWidgets(ID);


    header->hideComboBox(VPI);
    header->setLableText(VPI, "VPI");
    header->setLineEditPlaceHolder(VPI,"Search VPI");

    header->hideComboBox(VCI);
    header->setLableText(VCI, "VCI");
    header->setLineEditPlaceHolder(VCI, "Search VCI.");

    header->hideComboBox(DESC);
    header->setLableText(DESC, "Description");
    header->setLineEditPlaceHolder(DESC, "Search Desc");

    ui->pvcTv->verticalHeader()->hide();
    ui->pvcTv->horizontalHeader()->setStretchLastSection(true);
    ui->pvcTv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    /////////////////////
    fillTV(true);

    ui->pvcTv->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->pvcTv, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionEditPvcRef);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemovePvcRef);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionExport_PDF);
}

PvcRefDialog::~PvcRefDialog()
{
    delete ui;
}

void PvcRefDialog::fillTV(bool RESET)
{
    QSqlQuery *query = dbMan->selectPvcRef();

    //header->blockSignals(true);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*query);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->pvcTv->hideColumn(ID);

    if(RESET)
    {
        header->resetHeaderWidgets();
    }

    //header->blockSignals(false);
}

void PvcRefDialog::refreshSlotReset(int done)
{
    done++;
    fillTV(true);
    ui->pvcTv->selectRow(row);
}

void PvcRefDialog::refreshSlot(int done)
{
    done++;
    fillTV(false);
    ui->pvcTv->selectRow(row);
}

void PvcRefDialog::headerWidgetsChagedSlot()
{
    vpi = header->sections[VPI]->lineEdit->text().toInt();
    vci = header->sections[VCI]->lineEdit->text().toInt();
    desc = header->sections[DESC]->lineEdit->text();

    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(vpi != 0)
        mapString.insert(VPI, QString::number(vpi));
    if(vci != 0)
        mapString.insert(VCI, QString::number(vci));

    if(!desc.isEmpty())
    {
        reg.setPattern(desc);
        mapRegExp.insert(DESC, reg);
    }

    proxyModel->setColumnsFilter(mapString, mapRegExp);
    ui->pvcTv->hideColumn(ID);
}

void PvcRefDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->pvcTv->indexAt(ptr);
    row = clickedIndex.row();
    ptr.setY(ptr.y() + 80);

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID);
    id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), VPI);
    vpi = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), VCI);
    vci = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESC);
    desc = proxyModel->data(clickedIndex).toString();

    if(id > 0)
        contextMenu.popup(ui->pvcTv->mapToGlobal(ptr));
}

void PvcRefDialog::on_actionEditPvcRef_triggered()
{
    EditPvcRefDialog *editPvcRefDialog = new EditPvcRefDialog(this, dbMan, id, vpi, vci, desc);
    editPvcRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editPvcRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editPvcRefDialog->show();
}

void PvcRefDialog::on_actionRemovePvcRef_triggered()
{
    RemovePvcRefDialog *removePvcRefDialog = new RemovePvcRefDialog(this, dbMan,id, vpi,vci,desc );
    removePvcRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removePvcRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlotReset(int)));
    removePvcRefDialog->show();
}

void PvcRefDialog::on_addBtn_clicked()
{
    AddPvcRefDialog *addPvcRefDialog = new AddPvcRefDialog(this,dbMan);
    addPvcRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addPvcRefDialog, SIGNAL(finished(int)), this, SLOT(refreshSlotReset(int)));
    addPvcRefDialog->show();
}

void PvcRefDialog::on_actionExport_PDF_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet PVC Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf")) path += ".pdf";

        QVector<int> columnStretch = QVector<int>() <<0<<20<<20<<50;
        QVector<QString> headers = QVector<QString>() << "ID" << "VPI"<<"VCI"<<"Description";

        Report reportPvc(this,proxyModel,headers, columnStretch, path, "PVC");
        if(reportPvc.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportPvc.lastError());
        }
    }
}
