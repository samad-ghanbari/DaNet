#include "recommendationdialog.h"
#include "ui_recommendationdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>


RecommendationDialog::RecommendationDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::RecommendationDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    lastname = dbMan->getLoggedinLastname();
    ui->userLbl->setText("From Dear "+lastname+" To Developer");
}

RecommendationDialog::~RecommendationDialog()
{
    delete ui;
}

void RecommendationDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RecommendationDialog::on_recomTE_textChanged()
{
    if(ui->recomTE->toPlainText().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void RecommendationDialog::on_okBtn_clicked()
{
    QString recom = ui->recomTE->toPlainText();

    if(dbMan->insertRecommendation(lastname, recom))
        this->close();
    else
        QMessageBox::warning(this, "ERROR", "Error in sendinf message.\n"+dbMan->lastError());
}
