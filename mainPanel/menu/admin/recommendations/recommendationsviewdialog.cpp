#include "recommendationsviewdialog.h"
#include "ui_recommendationsviewdialog.h"
#include "mainPanel/menu/admin/recommendations/removerecommendationdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>

RecommendationsViewDialog::RecommendationsViewDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::RecommendationsViewDialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    resize(parent->width()-50, parent->height()-50);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);

    ui->recommTV->setModel(model);
    ui->recommTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->recommTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->recommTV->verticalHeader()->hide();

    ui->recommTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->recommTV, SIGNAL(customContextMenuRequested(QPoint)), this,SLOT(contextMenuSlot(QPoint)));
    contextMenu.addAction(ui->actionDelete_Recommendation);

    fillTV();

}

RecommendationsViewDialog::~RecommendationsViewDialog()
{
    delete ui;
}

void RecommendationsViewDialog::fillTV()
{
    QSqlQuery *query = dbMan->selectRecommendations();
    model->setQuery(*query);
    ui->recommTV->hideColumn(0);
}

void RecommendationsViewDialog::on_actionDelete_Recommendation_triggered()
{
    RemoveRecommendationDialog removeRecommendationDialog(this,dbMan,id,lastname,recomm);
    removeRecommendationDialog.exec();
    fillTV();
    ui->recommTV->selectRow(row);
}

void RecommendationsViewDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->recommTV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    id = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // user
    lastname = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3); // recomm
    recomm = model->data(clickedIndex).toString();

    if(id > 0)
        contextMenu.popup(ui->recommTV->mapToGlobal(ptr));
}
