#include "asr9010widget.h"
#include "ui_asr9010widget.h"
#include <QSqlQueryModel>

#define UnusedSlotColor "#c9bd43"

ASR9010Widget::ASR9010Widget(QWidget *parent, QSqlQueryModel *model) :
    QWidget(parent),
    ui(new Ui::ASR9010Widget)
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
        card = stringToVerticalString(card);
        setText(slot,sub,card);
    }
}

ASR9010Widget::~ASR9010Widget()
{
    delete ui;
}

void ASR9010Widget::hideSlots()
{
    ui->s0s1->setVisible(false);
    ui->s0s2->setVisible(false);
    ui->s0s3->setVisible(false);

    ui->s1s1->setVisible(false);
    ui->s1s2->setVisible(false);
    ui->s1s3->setVisible(false);

    ui->s2s1->setVisible(false);
    ui->s2s2->setVisible(false);
    ui->s2s3->setVisible(false);

    ui->s3s1->setVisible(false);
    ui->s3s2->setVisible(false);
    ui->s3s3->setVisible(false);

    ui->s6s1->setVisible(false);
    ui->s6s2->setVisible(false);
    ui->s6s3->setVisible(false);

    ui->s7s1->setVisible(false);
    ui->s7s2->setVisible(false);
    ui->s7s3->setVisible(false);

    ui->s8s1->setVisible(false);
    ui->s8s2->setVisible(false);
    ui->s8s3->setVisible(false);

    ui->s9s1->setVisible(false);
    ui->s9s2->setVisible(false);
    ui->s9s3->setVisible(false);
}

QString ASR9010Widget::stringToVerticalString(QString string)
{
    if(string.contains("*") || string.contains("-"))
    {

        string = string.replace("*", "\n*\n");
        string = string.replace("-", "\n-\n");
        string = string.replace("1G", "\n1G\n");
        string = string.replace("10G", "10\nG\n");
        string = string.replace("40G", "40\nG\n");
        string = string.replace("100G", "100\nG\n");
        string = string.replace("#", "\n#\n");
        string = string.replace("\n\n", "\n");

        return string;
    }
    else
    {
        QString res = "";
        QChar ch;
        int len = string.length();
        for(int i = 0; i < len; i++)
        {
            ch = string.at(i);
            if(i > 0)
                res +='\n'+ch;
            else
                res += ch;
        }

        return res;
    }
}

void ASR9010Widget::setText(const int slot, int subSlot, QString card)
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
    else if(slot == 1)
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
    else if(slot == 7)
    {
        if(subSlot == 0)
        {
            ui->s7s0->setText(card);
            if(card.startsWith("_"))
                ui->s7s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s7s1->setVisible(true);
            ui->s7s1->setText(card);
            if(card.startsWith("_"))
                ui->s7s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s7s2->setVisible(true);
            ui->s7s2->setText(card);
            if(card.startsWith("_"))
                ui->s7s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s7s3->setVisible(true);
            ui->s7s3->setText(card);
            if(card.startsWith("_"))
                ui->s7s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 8)
    {
        if(subSlot == 0)
        {
            ui->s8s0->setText(card);
            if(card.startsWith("_"))
                ui->s8s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s8s1->setVisible(true);
            ui->s8s1->setText(card);
            if(card.startsWith("_"))
                ui->s8s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s8s2->setVisible(true);
            ui->s8s2->setText(card);
            if(card.startsWith("_"))
                ui->s8s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s8s3->setVisible(true);
            ui->s8s3->setText(card);
            if(card.startsWith("_"))
                ui->s8s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 9)
    {
        if(subSlot == 0)
        {
            ui->s9s0->setText(card);
            if(card.startsWith("_"))
                ui->s9s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s9s1->setVisible(true);
            ui->s9s1->setText(card);
            if(card.startsWith("_"))
                ui->s9s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s9s2->setVisible(true);
            ui->s9s2->setText(card);
            if(card.startsWith("_"))
                ui->s9s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s9s3->setVisible(true);
            ui->s9s3->setText(card);
            if(card.startsWith("_"))
                ui->s9s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }

}
