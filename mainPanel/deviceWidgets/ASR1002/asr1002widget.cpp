#include "asr1002widget.h"
#include "ui_asr1002widget.h"
#include <QSqlQueryModel>

#define UnusedSlotColor "#c9bd43"

ASR1002Widget::ASR1002Widget(QWidget *parent, QSqlQueryModel *model) :
    QWidget(parent),
    ui(new Ui::ASR1002Widget)
{
    ui->setupUi(this);

    hideSlots();

    QModelIndex index = model->index(0,0);
    int currentShelf,shelf, slot, sub;
    QString card;
    currentShelf = model->data(index).toInt();

    //`shelf`, `slot`, `sub_slot`, `card`
    for(int i = 0; i < model->rowCount(); i++)
    {
        index = index.sibling(i, 0);
        shelf = model->data(index).toInt();
        if(shelf != currentShelf) break;
        index = index.sibling(i, 1);
        slot = model->data(index).toInt();
        index = index.sibling(i,2);
        if(model->data(index).isNull())
            sub = -1;
        else
            sub = model->data(index).toInt();
        index = index.sibling(i,3);
        card = model->data(index).toString();
        if(card.startsWith("EMPTY"))
            card = "";

        if(card.startsWith("#"))
            card = "";
        setText(slot,sub,card);
    }

}

ASR1002Widget::~ASR1002Widget()
{
    delete ui;
}

void ASR1002Widget::hideSlots()
{

//    ui->s0s0->setVisible(false);
//    ui->s0s1->setVisible(false);
//    ui->s0s2->setVisible(false);
//    ui->s0s3->setVisible(false);
}

void ASR1002Widget::setText(const int slot, int subSlot, QString card)
{
    if(slot == 0)
    {
        if(subSlot == 0)
        {
            ui->s0s0->setText(card);
            if(card.startsWith("_"))
                ui->s0s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s0s1->setVisible(true);
            ui->s0s1->setText(card);
            if(card.startsWith("_"))
                ui->s0s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s0s2->setVisible(true);
            ui->s0s2->setText(card);
            if(card.startsWith("_"))
                ui->s0s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s0s3->setVisible(true);
            ui->s0s3->setText(card);
            if(card.startsWith("_"))
                ui->s0s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
}
