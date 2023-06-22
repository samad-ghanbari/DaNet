#include "s9306widget.h"
#include "ui_s9306widget.h"
#include <QSqlQueryModel>

#define UnusedSlotColor "#c9bd43"

S9306Widget::S9306Widget(QWidget *parent, QSqlQueryModel *model) :
    QWidget(parent),
    ui(new Ui::S9306Widget)
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

S9306Widget::~S9306Widget()
{
    delete ui;
}

void S9306Widget::hideSlots()
{

    ui->s1s1->setVisible(false);
    ui->s1s2->setVisible(false);
    ui->s1s3->setVisible(false);

    ui->s2s1->setVisible(false);
    ui->s2s2->setVisible(false);
    ui->s2s3->setVisible(false);

    ui->s3s1->setVisible(false);
    ui->s3s2->setVisible(false);
    ui->s3s3->setVisible(false);

    ui->s4s1->setVisible(false);
    ui->s4s2->setVisible(false);
    ui->s4s3->setVisible(false);

    ui->s5s1->setVisible(false);
    ui->s5s2->setVisible(false);
    ui->s5s3->setVisible(false);

    ui->s6s1->setVisible(false);
    ui->s6s2->setVisible(false);
    ui->s6s3->setVisible(false);
}

void S9306Widget::setText(const int slot, int subSlot, QString card)
{
    if(slot == 1)
    {
        if(subSlot == 0)
        {
            ui->s1s0->setText(card);
            if(card.startsWith("_"))
                ui->s1s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s1s1->setVisible(true);
            ui->s1s1->setText(card);
            if(card.startsWith("_"))
                ui->s1s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s1s2->setVisible(true);
            ui->s1s2->setText(card);
            if(card.startsWith("_"))
                ui->s1s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s1s3->setVisible(true);
            ui->s1s3->setText(card);
            if(card.startsWith("_"))
                ui->s1s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 2)
    {
        if(subSlot == 0)
        {
            ui->s2s0->setText(card);
            if(card.startsWith("_"))
                ui->s2s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s2s1->setVisible(true);
            ui->s2s1->setText(card);
            if(card.startsWith("_"))
                ui->s2s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s2s2->setVisible(true);
            ui->s2s2->setText(card);
            if(card.startsWith("_"))
                ui->s2s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s2s3->setVisible(true);
            ui->s2s3->setText(card);
            if(card.startsWith("_"))
                ui->s2s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 3)
    {
        if(subSlot == 0)
        {
            ui->s3s0->setText(card);
            if(card.startsWith("_"))
                ui->s3s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s3s1->setVisible(true);
            ui->s3s1->setText(card);
            if(card.startsWith("_"))
                ui->s3s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s3s2->setVisible(true);
            ui->s3s2->setText(card);
            if(card.startsWith("_"))
                ui->s3s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s3s3->setVisible(true);
            ui->s3s3->setText(card);
            if(card.startsWith("_"))
                ui->s3s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 4)
    {
        if(subSlot == 0)
        {
            ui->s4s0->setText(card);
            if(card.startsWith("_"))
                ui->s4s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s4s1->setVisible(true);
            ui->s4s1->setText(card);
            if(card.startsWith("_"))
                ui->s4s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s4s2->setVisible(true);
            ui->s4s2->setText(card);
            if(card.startsWith("_"))
                ui->s4s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s4s3->setVisible(true);
            ui->s4s3->setText(card);
            if(card.startsWith("_"))
                ui->s4s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 5)
    {
        if(subSlot == 0)
        {
            ui->s5s0->setText(card);
            if(card.startsWith("_"))
                ui->s5s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s5s1->setVisible(true);
            ui->s5s1->setText(card);
            if(card.startsWith("_"))
                ui->s5s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s5s2->setVisible(true);
            ui->s5s2->setText(card);
            if(card.startsWith("_"))
                ui->s5s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s5s3->setVisible(true);
            ui->s5s3->setText(card);
            if(card.startsWith("_"))
                ui->s5s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 6)
    {
        if(subSlot == 0)
        {
            ui->s6s0->setText(card);
            if(card.startsWith("_"))
                ui->s6s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s6s1->setVisible(true);
            ui->s6s1->setText(card);
            if(card.startsWith("_"))
                ui->s6s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s6s2->setVisible(true);
            ui->s6s2->setText(card);
            if(card.startsWith("_"))
                ui->s6s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s6s3->setVisible(true);
            ui->s6s3->setText(card);
            if(card.startsWith("_"))
                ui->s6s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }

}
