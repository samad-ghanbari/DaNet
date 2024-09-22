#include "ne5000X16Awidget.h"
#include "ui_ne5000X16Awidget.h"
#include <QSqlQueryModel>

#define UnusedSlotColor "#c9bd43"

NE5000X16AWidget::NE5000X16AWidget(QWidget *parent,const bool NE40FLAG, QSqlQueryModel *model) :
    QWidget(parent),
    ui(new Ui::NE5000X16AWidget)
{
    ui->setupUi(this);

    if(!NE40FLAG)
        ui->deviceLabel->setText("Huawei CX600X16");

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
        card = stringToVerticalString(card);
        if(card.startsWith("EMPTY"))
            card = "";

        if(card.startsWith("#"))
            card = "";
        setText(slot,sub,card);
    }

}

NE5000X16AWidget::~NE5000X16AWidget()
{
    delete ui;
}

void NE5000X16AWidget::hideSlots()
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

    ui->s7s1->setVisible(false);
    ui->s7s2->setVisible(false);
    ui->s7s3->setVisible(false);

    ui->s8s1->setVisible(false);
    ui->s8s2->setVisible(false);
    ui->s8s3->setVisible(false);

    ui->s9s1->setVisible(false);
    ui->s9s2->setVisible(false);
    ui->s9s3->setVisible(false);

    ui->s10s1->setVisible(false);
    ui->s10s2->setVisible(false);
    ui->s10s3->setVisible(false);

    ui->s11s1->setVisible(false);
    ui->s11s2->setVisible(false);
    ui->s11s3->setVisible(false);

    ui->s12s1->setVisible(false);
    ui->s12s2->setVisible(false);
    ui->s12s3->setVisible(false);

    ui->s13s1->setVisible(false);
    ui->s13s2->setVisible(false);
    ui->s13s3->setVisible(false);

    ui->s14s1->setVisible(false);
    ui->s14s2->setVisible(false);
    ui->s14s3->setVisible(false);

    ui->s15s1->setVisible(false);
    ui->s15s2->setVisible(false);
    ui->s15s3->setVisible(false);

    ui->s16s1->setVisible(false);
    ui->s16s2->setVisible(false);
    ui->s16s3->setVisible(false);
}

QString NE5000X16AWidget::stringToVerticalString(QString string)
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

void NE5000X16AWidget::setText(const int slot, int subSlot, QString card)
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
    else if(slot == 10)
    {
        if(subSlot == 0)
        {
            ui->s10s0->setText(card);
            if(card.startsWith("_"))
                ui->s10s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s10s1->setVisible(true);
            ui->s10s1->setText(card);
            if(card.startsWith("_"))
                ui->s10s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s10s2->setVisible(true);
            ui->s10s2->setText(card);
            if(card.startsWith("_"))
                ui->s10s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s10s3->setVisible(true);
            ui->s10s3->setText(card);
            if(card.startsWith("_"))
                ui->s10s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 11)
    {
        if(subSlot == 0)
        {
            ui->s11s0->setText(card);
            if(card.startsWith("_"))
                ui->s11s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s11s1->setVisible(true);
            ui->s11s1->setText(card);
            if(card.startsWith("_"))
                ui->s11s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s11s2->setVisible(true);
            ui->s11s2->setText(card);
            if(card.startsWith("_"))
                ui->s11s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s11s3->setVisible(true);
            ui->s11s3->setText(card);
            if(card.startsWith("_"))
                ui->s11s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 12)
    {
        if(subSlot == 0)
        {
            ui->s12s0->setText(card);
            if(card.startsWith("_"))
                ui->s12s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s12s1->setVisible(true);
            ui->s12s1->setText(card);
            if(card.startsWith("_"))
                ui->s12s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s12s2->setVisible(true);
            ui->s12s2->setText(card);
            if(card.startsWith("_"))
                ui->s12s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s12s3->setVisible(true);
            ui->s12s3->setText(card);
            if(card.startsWith("_"))
                ui->s12s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 13)
    {
        if(subSlot == 0)
        {
            ui->s13s0->setText(card);
            if(card.startsWith("_"))
                ui->s13s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s13s1->setVisible(true);
            ui->s13s1->setText(card);
            if(card.startsWith("_"))
                ui->s13s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s13s2->setVisible(true);
            ui->s13s2->setText(card);
            if(card.startsWith("_"))
                ui->s13s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s13s3->setVisible(true);
            ui->s13s3->setText(card);
            if(card.startsWith("_"))
                ui->s13s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 14)
    {
        if(subSlot == 0)
        {
            ui->s14s0->setText(card);
            if(card.startsWith("_"))
                ui->s14s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s14s1->setVisible(true);
            ui->s14s1->setText(card);
            if(card.startsWith("_"))
                ui->s14s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s14s2->setVisible(true);
            ui->s14s2->setText(card);
            if(card.startsWith("_"))
                ui->s14s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s14s3->setVisible(true);
            ui->s14s3->setText(card);
            if(card.startsWith("_"))
                ui->s14s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 15)
    {
        if(subSlot == 0)
        {
            ui->s15s0->setText(card);
            if(card.startsWith("_"))
                ui->s15s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s15s1->setVisible(true);
            ui->s15s1->setText(card);
            if(card.startsWith("_"))
                ui->s15s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s15s2->setVisible(true);
            ui->s15s2->setText(card);
            if(card.startsWith("_"))
                ui->s15s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s15s3->setVisible(true);
            ui->s15s3->setText(card);
            if(card.startsWith("_"))
                ui->s15s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }
    else if(slot == 16)
    {
        if(subSlot == 0)
        {
            ui->s16s0->setText(card);
            if(card.startsWith("_"))
                ui->s16s0->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 1)
        {
            ui->s16s1->setVisible(true);
            ui->s16s1->setText(card);
            if(card.startsWith("_"))
                ui->s16s1->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        if(subSlot == 2)
        {
            ui->s16s2->setVisible(true);
            ui->s16s2->setText(card);
            if(card.startsWith("_"))
                ui->s16s2->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
        else if(subSlot == 3)
        {
            ui->s16s3->setVisible(true);
            ui->s16s3->setText(card);
            if(card.startsWith("_"))
                ui->s16s3->setStyleSheet("QLabel { background-color :"+QString(UnusedSlotColor)+"; }");
        }
    }

}
