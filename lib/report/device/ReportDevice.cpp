#include "ReportDevice.h"
#include <QAbstractItemModel>
#include <QDate>
#include <QPainter>

ReportDevice::ReportDevice(QObject *parent, QAbstractItemModel *Model, const QString Device, const int SlotCount, const int StartSlot, const int DevexId, const QString DevexName, const QList<QString> exchInfo, const QString Saloon, const QString OutputPath, const int PaperSize, const int Orient, const QFont Font, const QString CustomInfo) :
    QObject(parent),
    painter(new QPainter()),
    model(Model),
    outputPath(OutputPath),
    abbr(exchInfo.at(0)),
    exchange(exchInfo.at(1)),
    address(exchInfo.at(2)),
    saloon(Saloon),
    device(Device),
    devexId(DevexId),
    slotCount(SlotCount),
    startSlot(StartSlot),
    DslamDevice(false)
{
    if(CustomInfo.trimmed().isEmpty())
    {
        CUSTOM_INFO = false;
        customInfo = "";
    }
    else
    {
        CUSTOM_INFO = true;
        customInfo = CustomInfo;
    }

    init(PaperSize, Orient, Font);
    devexName = DevexName;
    devexName.replace(" ","");
}

ReportDevice::ReportDevice(QObject *parent, QAbstractItemModel *AllModel, QString Device,const int SlotCount,const int StartSlot, const QMap<int, int> shelfDevexId, const QList<QString> DslamRackNames, const QList<QString> DslamRackNms, const QList<QString> DslamRackPppoe, const QList<QString> exchInfo, const QString Saloon, const QString OutputPath, const int PaperSize, const int Orient, const QFont Font, const QString CustomInfo) :
    QObject(parent),
    painter(new QPainter()),
    model(AllModel),
    outputPath(OutputPath),
    abbr(exchInfo.at(0)),
    exchange(exchInfo.at(1)),
    address(exchInfo.at(2)),
    saloon(Saloon),
    device(Device),
    devexId(shelfDevexId.value(1)),
    slotCount(SlotCount),
    startSlot(StartSlot),
    DslamDevice(true)
{

    if(CustomInfo.trimmed().isEmpty())
    {
        CUSTOM_INFO = false;
        customInfo = "";
    }
    else
    {
        CUSTOM_INFO = true;
        customInfo = CustomInfo;
    }

    init(PaperSize, Orient, Font);

    dslamRackName = DslamRackNames;
    dslamRackNms = DslamRackNms;
    dslamRackPppoe = DslamRackPppoe;
    dslamDevexId = shelfDevexId;
}


ReportDevice::~ReportDevice()
{
    delete painter;
    painter = NULL;
}

void ReportDevice::init(const int PaperSize, const int Orient, const QFont Font)
{
    if(CUSTOM_INFO)
        headerHeight = 180;
    else
        headerHeight = 150;

    bottomHeight = 40;
    leftMargin = 0;
    rightMargin = 0;

    slotGap = 5;

    pen = QPen(QColor(0, 0, 80), 1, Qt::SolidLine);
    font = Font;
    headerFont = font;
    headerFont.setBold(true);
    headerFont.setPixelSize(font.pixelSize()+2);

    pageNumber = 1;

    currentDate = QDate::currentDate().toString("yyyy/MM/dd");
    error = "No error";

    switch (PaperSize)
    {
    case 0:
        paperSize = QPrinter::A0;
        break;
    case 1:
        paperSize = QPrinter::A1;
        break;
    case 2:
        paperSize = QPrinter::A2;
        break;
    case 3:
        paperSize = QPrinter::A3;
        break;
    case 4:
        paperSize = QPrinter::A4;
        break;
    case 5:
        paperSize = QPrinter::A5;
        break;
    default:
        paperSize = QPrinter::A3;
        break;
    }

    switch (Orient)
    {
    case 0:
        orient = QPrinter::Landscape;
        break;
    case 1:
        orient = QPrinter::Portrait;
        break;
    }
}

int ReportDevice::getSubSlotCount(const int DevexId, const int Slot, const bool checkSubSlot)
{
    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`

    if(checkSubSlot)
    {
        // max subslot:0 count >> 1
        // max subslot: 1 count: >> 2
        // max subslot > 1 >>> count 4
        // it is for ASR 1013
        int dId,slot,cnt = 0, max = 0;
        QModelIndex index = model->index(0,0);
        for(int r = 0; r < model->rowCount(); r++)
        {
            index = index.sibling(r,1);
            dId = model->data(index).toInt();
            index = index.sibling(r,4);
            slot = model->data(index).toInt();
            if((slot == Slot) && (dId == DevexId) )
            {
                index = index.sibling(r,5);
                if(model->data(index).isNull())
                    return 1;
                else
                {
                    max = model->data(index).toInt();
                    cnt++;
                }
            }
        }

        if(cnt == 1)
        {
            if(max == 1)
                cnt = 2;
            else if(max > 1)
                cnt = 4;
        }
        else if(cnt == 2)
        {
            if(max > 1)
                cnt = 4;
        }

        return cnt;
    }
    else
    {
        int dId,slot,cnt = 0;
        QModelIndex index = model->index(0,0);
        for(int r = 0; r < model->rowCount(); r++)
        {
            index = index.sibling(r,1);
            dId = model->data(index).toInt();
            index = index.sibling(r,4);
            slot = model->data(index).toInt();
            if((slot == Slot) && (dId == DevexId) )
            {
                index = index.sibling(r,5);
                if(model->data(index).isNull())
                    return 1;
                else
                    cnt++;
            }
        }

        return cnt;
    }

}

void ReportDevice::preparePage()
{

    painter->resetTransform();

    painter->save();

    painter->setPen(Qt::darkMagenta);
    painter->setFont(QFont("Tahoma", 14, QFont::Bold));
    QRect rec(10,10,painter->viewport().width()-100,30);
    painter->drawText(rec,Qt::AlignLeft, currentDate);
    painter->drawText(rec,Qt::AlignCenter,"DaNet Device Report");
    rec.setRect(10,40,painter->viewport().width()-100,30);
    painter->drawText(rec, Qt::AlignLeft,abbr);
    painter->drawText(rec, Qt::AlignCenter,exchange);
    rec.setRect(10,70,painter->viewport().width()-100,30);
    painter->drawText(rec, Qt::AlignLeft,saloon);

    QPixmap im(":/Image/tct.jpg");
    painter->drawPixmap(painter->viewport().width()-50, 20,im);
    QPen penTemp = QPen(Qt::magenta, 1);
    painter->setPen(penTemp);
    painter->drawLine(0, 100,painter->viewport().width(), 90);
    painter->setPen(Qt::darkBlue);
    rec.setRect(0,110,painter->viewport().width(),30);
    painter->setFont(QFont("Tahoma", 12, QFont::Normal));
    painter->drawText(rec, Qt::AlignRight,address);

    if(CUSTOM_INFO)
    {
        rec.setRect(10,140,painter->viewport().width()-20,30);
        painter->setPen(Qt::darkCyan);
        painter->drawText(rec, Qt::AlignLeft,customInfo);
        painter->setPen(Qt::darkMagenta);
    }

    //page number
    painter->setFont(QFont("Tahoma", 10));
    painter->translate(10, painter->viewport().height()-10 );
    painter->setPen(QPen(QColor(0, 0, 0), 2));
    painter->drawText(0, 0, QString("Page %1").arg(pageNumber));
    pageNumber += 1;

    painter->restore();


    painter->translate(leftMargin, headerHeight); // start print point
}

bool ReportDevice::print()
{
    QPrinter *printer = new QPrinter(QPrinter::ScreenResolution);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOrientation(orient);
    printer->setPaperSize(paperSize);
    printer->setOutputFileName(outputPath);
    printer->setCreator("DaNet Application");
    printer->setDocName("Device Report");
    printer->setPrintProgram("DaNet Application");


    if(!painter->begin(printer))
    {
        error = "painter can't start printer.";
        return false;
    }
    //--------------------------------- error checking -------------------------------------
    if(!printer->isValid())
    {
        error = "printer.isValid() == false";
        return false;
    }
    if(!painter->isActive())
    {
        error = "painter.isActive() == false";
        return false;
    }
    //--------------------------------------------------------------------------------------

    pen.setColor(Qt::darkBlue);
    painter->setPen(pen);


    preparePage();

    if(device.contains("MA5600", Qt::CaseInsensitive))
        paintDslam16SlotDevice();
    else if(device.contains("MXK819", Qt::CaseInsensitive))
        paintDslam16SlotDevice();
    else if(device.contains("MA5300", Qt::CaseInsensitive))
        paintDslam16SlotDevice();
    else if(device.contains("MA5616", Qt::CaseInsensitive))
        paintMA5616Device();
    else if(device.contains("S9303", Qt::CaseInsensitive) || device.contains("S9300", Qt::CaseInsensitive) )
        paintS9303Device();
    else if(device.contains("S9306", Qt::CaseInsensitive))
        paintS9306Device();
    else if(device.contains("S5300", Qt::CaseInsensitive))
        paintS5300Device();
    else if(device.contains("ME60X16", Qt::CaseInsensitive))
        paintNE40X16Device();
    else if(device.contains("ME60X8", Qt::CaseInsensitive))
        paintNE40X8Device();
    else if(device.contains("CX600X16", Qt::CaseInsensitive))
        paintNE40X16Device();
    else if(device.contains("CX600X8", Qt::CaseInsensitive))
        paintNE40X8Device();
    else if(device.contains("NE40X16", Qt::CaseInsensitive))
        paintNE40X16Device();
    else if(device.contains("NE40EX16", Qt::CaseInsensitive))
        paintNE40X16Device();
    else if(device.contains("NE40X8", Qt::CaseInsensitive))
        paintNE40X8Device();
    else if(device.contains("NE40EX8", Qt::CaseInsensitive))
        paintNE40X8Device();
    else if(device.contains("NE5000X16A", Qt::CaseInsensitive))
        paintNE5000X16ADevice();
    else if(device.contains("NE5000X16", Qt::CaseInsensitive))
        paintNE5000X16Device();
    else if(device.contains("NE5000E", Qt::CaseInsensitive))
        paintNE5000X16Device();
    else if(device.contains("7613", Qt::CaseInsensitive))
        paint7613Device();
    else if(device.contains("ASR9010", Qt::CaseInsensitive))
        paintASR9010Device();
    else if(device.contains("ASR1013", Qt::CaseInsensitive))
        paintASR1013Device();
    else if(device.contains("ASR1002-X", Qt::CaseInsensitive))
        paintASR1002XDevice();
    else
        paintUnkownDevice();

    painter->end();
    return true;

}

void ReportDevice::paintDslam16SlotDevice()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
//    QFont f;
//    f.setFamily("Tahoma");
//    f.setBold(true);
//    f.setPointSize(14);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    rackHeight = painter->viewport().height() - 200 -headerHeight -  bottomHeight;
    rackWidth = 3*painter->viewport().width()/4;
    painter->translate((painter->viewport().width() - rackWidth)/2, 100);

    shelfHeaderHeight = 50;
    shelfBottomHeight = 50;
    shelfHeight = rackHeight / 3 - shelfHeaderHeight - shelfBottomHeight;
    slotWidth = (rackWidth - 17*slotGap) / 16;
    slotHeight = shelfHeight - 2*slotGap;


    paintDslam16SlotShelf(1);
    if(dslamDevexId.value(2) > -2) // if it is not outdoor
        paintDslam16SlotShelf(2);
    if(dslamDevexId.value(3) > -2)
        paintDslam16SlotShelf(3);

}

void ReportDevice::paintDslam16SlotShelf(const int shelfNo)
{
    painter->setFont(font);
    painter->save();
    QRect rect(0,0,rackWidth,shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    QString dn = dslamRackName.at(shelfNo - 1);
    dn = dn.replace(" ", "");
    painter->drawText(rect, Qt::AlignCenter, dn);
    painter->setFont(font);
    painter->translate(0, shelfHeaderHeight);
    //.. shelf slots
    rect.setRect(0,0,rackWidth, shelfHeight); //shelf frame
    painter->drawRect(rect);
    //slots
    painter->translate(slotGap, slotGap);
    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`

    int devexid = dslamDevexId.value(shelfNo);
    int id, slot,subslotCount,subHeight,deviceSlot;
    QString card, purchase;
    deviceSlot = startSlot;
    QModelIndex index = model->index(0,0);
    // fill slot
    int r = 0;
    while(r < model->rowCount())
    {
        index = index.sibling(r,1);
        id = model->data(index).toInt();
        if(id == devexid)
        {
            index = index.sibling(r, 4);
            slot = model->data(index).toInt();
            index = index.sibling(r,6);
            card = model->data(index).toString();
            index = index.sibling(r,15);
            purchase = model->data(index).toString();

//            painter->save();
//            painter->setFont(smallFont);
            while(deviceSlot < slot)
            {
                //empty slots
                rect.setRect(0,0,slotWidth, slotHeight);
                painter->fillRect(rect,QColor(240,240, 250));
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "0/"+QString::number(deviceSlot));
                painter->setPen(pen);
                painter->translate(slotWidth+slotGap, 0);

                deviceSlot++;
            }
//            painter->restore();

            if(deviceSlot == slot)
            {
                subslotCount = getSubSlotCount(devexid, slot);
                subHeight = (slotHeight - (subslotCount-1)*slotGap)/subslotCount;
                if(subslotCount > 0)
                {
                    for(int j=0; j < subslotCount; j++)
                    {
                        index = index.sibling(r, 4);
                        slot = model->data(index).toInt();
                        index = index.sibling(r,6);
                        card = model->data(index).toString();
                        index = index.sibling(r,15);
                        purchase = model->data(index).toString();

                        if(deviceSlot == slot)
                        {
                            r++;
                            rect.setRect(0,j*(subHeight+slotGap),slotWidth, subHeight);
                            if( (slot == 7) || (slot == 8) )
                                painter->fillRect(rect,QColor(200,190, 190));
                            else
                                painter->fillRect(rect,QColor(240,240, 250));
                            painter->drawRect(rect);
//                            painter->save();
//                            painter->setFont(smallFont);
                            painter->setPen(Qt::blue);
                            painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "0/"+QString::number(deviceSlot));
                            painter->setPen(pen);
//                            painter->restore();
                            if(!card.isEmpty())
                            {
                                if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                                    card = "";
                                else
                                    card = card + "#"+ purchase;

                                card = stringToVerticalString(card);
                                painter->drawText(rect, Qt::AlignCenter, card);
                            }

                        }
                    }
                    deviceSlot++;
                    painter->translate(slotWidth+slotGap, 0);
                }
                else
                {
                    r++;
                }
            }else r++;
        }
        else
            r++;
    }
//    painter->save();
//    painter->setFont(smallFont);
    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots
        rect.setRect(0,0,slotWidth, slotHeight);
        if( (deviceSlot == 7) || (deviceSlot == 8) )
            painter->fillRect(rect,QColor(200,190, 190));
        else
            painter->fillRect(rect,QColor(240,240, 250));
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "0/"+QString::number(deviceSlot));
        painter->setPen(pen);
        painter->translate(slotWidth+slotGap, 0);

        deviceSlot++;
    }
//    painter->restore();
    // end fill slot
    painter->restore();
    //..
    painter->translate(0,shelfHeight+shelfHeaderHeight);
    rect.setRect(0,0,rackWidth, shelfBottomHeight);
    painter->fillRect(rect, QColor(230, 230, 240));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    if(!dslamRackNms.at(shelfNo - 1).isEmpty())
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, "IP: "+dslamRackNms.at(shelfNo - 1));
    if(!dslamRackPppoe.at(shelfNo - 1).isEmpty())
        painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "PPPOE: "+dslamRackPppoe.at(shelfNo - 1));
    painter->setFont(font);
    painter->translate(0,shelfBottomHeight);
}

void ReportDevice::paintMA5616Device()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
//    QFont f;
//    f.setFamily("Tahoma");
//    f.setBold(true);
//    f.setPointSize(14);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    double deviceWidth = 3*painter->viewport().width()/5;
    double deviceHeight  = deviceWidth / 3;

    shelfHeaderHeight = 50;
    shelfBottomHeight = 50;

    if( (deviceHeight) > (painter->viewport().height() - bottomHeight -headerHeight - 200) )
    {
        deviceHeight = painter->viewport().height() - headerHeight - bottomHeight - 200;
        painter->translate((painter->viewport().width()-deviceWidth)/2, 50); //start point
    }
    else
        painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    double leftSectionWidth = deviceWidth /3 - 2*slotGap;
    double rightSectionWidth = deviceWidth + - leftSectionWidth - 3* slotGap;
    double leftSectionHeight = (deviceHeight - 3*slotGap) / 2;
    slotHeight = (deviceHeight - 5*slotGap) / 4 ;


    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, dslamRackName.at(0));
    painter->setFont(font);
    painter->translate(0, shelfHeaderHeight);

    //frame
    rect.setRect(0, 0, deviceWidth, deviceHeight);
    painter->drawRect(rect);

    rect.setRect(0,deviceHeight,deviceWidth, shelfBottomHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    QString ip = dslamRackNms.at(0);
    QString p = dslamRackPppoe.at(0);
    if(!ip.isEmpty())
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, "IP: "+ ip);
    if(!p.isEmpty())
        painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "PPPOE: "+p);
    painter->setFont(font);


    painter->translate(slotGap, slotGap);

    rect.setRect(0,0,leftSectionWidth, leftSectionHeight);
    painter->fillRect(rect, QColor(200,190, 190));
    painter->drawRect(rect);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, "CCUB/E");


    rect.setRect(0,leftSectionHeight + slotGap,leftSectionWidth, leftSectionHeight);
    painter->fillRect(rect, QColor(220,200, 200));
    painter->drawRect(rect);

    if(device.contains("5616V", Qt::CaseInsensitive))
        painter->drawText(rect, Qt::AlignCenter, "PAIC");
    else
        painter->drawText(rect, Qt::AlignCenter, "PDIA/B");

    painter->setFont(font);

    painter->translate(leftSectionWidth + slotGap, 0);
    //slots
    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    int r = 0,devexid,slot,subslotCount,subWidth, deviceSlot = startSlot;
    QString card, purchase;
    QModelIndex index = model->index(0,4);
    while(r < model->rowCount())
    {
        index = index.sibling(r, 1);
        devexid = model->data(index).toInt();

        if(devexid == devexId)
        {
            index = index.sibling(r, 4);
            slot = model->data(index).toInt();
            index = index.sibling(r, 6);
            card = model->data(index).toString();
            index = index.sibling(r, 15);
            purchase = model->data(index).toString();

//            painter->save();
//            painter->setFont(smallFont);
            while (deviceSlot < slot)
            {
                //empty slots
                rect.setRect(0, (deviceSlot-startSlot)*(slotHeight + slotGap), rightSectionWidth, slotHeight);
                painter->fillRect(rect,QColor(230, 230, 245) );
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , "0/"+QString::number(deviceSlot));
                painter->setPen(pen);
                deviceSlot++;
            }
//            painter->restore();

            if(deviceSlot == slot)
            {
                subslotCount = getSubSlotCount(devexId, slot);
                subWidth = (rightSectionWidth - (subslotCount-1)*slotGap)/subslotCount;
                if(subslotCount > 0)
                {

                    for(int j=0; j < subslotCount; j++)
                    {
                        index = index.sibling(r, 4);
                        slot = model->data(index).toInt();
                        index = index.sibling(r, 6);
                        card = model->data(index).toString();
                        index = index.sibling(r, 15);
                        purchase = model->data(index).toString();

                        if(deviceSlot == slot)
                        {
                            r++;

                            rect.setRect(j*(subWidth+slotGap), (slot-startSlot)*(slotHeight + slotGap), subWidth, slotHeight);
                            painter->fillRect(rect,QColor(230, 230, 245) );
                            painter->drawRect(rect);
//                            painter->save();
//                            painter->setFont(smallFont);
                            painter->setPen(Qt::blue);
                            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , "0/"+QString::number(deviceSlot));
                            painter->setPen(pen);
//                            painter->restore();
                            if(!card.isEmpty())
                            {
                                if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                                    card = "";
                                else
                                    card = card + "#"+ purchase;

                                painter->drawText(rect, Qt::AlignCenter, card);
                            }
                        }
                    }
                    deviceSlot++;
                }
                else r++;
            }else r++;
        } else r++;
    }

//    painter->save();
//    painter->setFont(smallFont);
    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots
        rect.setRect(0, (deviceSlot-startSlot)*(slotHeight + slotGap), rightSectionWidth, slotHeight);
        painter->fillRect(rect,QColor(230, 230, 245) );
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , "0/"+QString::number(deviceSlot));
        painter->setPen(pen);
        deviceSlot++;
    }
//    painter->restore();

}

void ReportDevice::paintS9303Device()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    double deviceWidth = 3*painter->viewport().width()/5;
    double deviceHeight  = deviceWidth / 2;

    shelfHeaderHeight = 50;

    if( (deviceHeight) > (painter->viewport().height() - headerHeight - 200) )
    {
        deviceHeight = painter->viewport().height() - headerHeight  - 200;
        painter->translate((painter->viewport().width()-deviceWidth)/2, 50); //start point
    }
    else
        painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    double fanWidth = 50;
    double fanHeight = deviceHeight - 2*slotGap;
    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);

    slotWidth = deviceWidth - fanWidth - 3*slotGap;
    slotHeight = (deviceHeight - 6*slotGap) / 5;
    double mcuWidth = (slotWidth - slotGap)/2;
    double pwrWidth = (slotWidth - 2*slotGap) / 3;

    //fan
    rect.setRect(deviceWidth -fanWidth - slotGap, slotGap, fanWidth, fanHeight);
    painter->fillRect(rect, QColor(200, 220, 240));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "F\nA\nN");

    //mcu
    rect.setRect(slotGap, 4*slotGap+3*slotHeight, mcuWidth, slotHeight);
    painter->fillRect(rect, QColor(200, 200, 230));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "MCU");

    rect.setRect(2*slotGap+mcuWidth, 4*slotGap+3*slotHeight, mcuWidth, slotHeight);
    painter->fillRect(rect, QColor(200, 200, 230));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "MCU");

    //pwr
    rect.setRect(slotGap, 5*slotGap+4*slotHeight, pwrWidth, slotHeight);
    painter->fillRect(rect, QColor(210, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "PWR1");

    rect.setRect(2*slotGap+pwrWidth, 5*slotGap+4*slotHeight, pwrWidth, slotHeight);
    painter->fillRect(rect, QColor(210, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "PWR2");

    rect.setRect(3*slotGap+2*pwrWidth, 5*slotGap+4*slotHeight, pwrWidth, slotHeight);
    painter->fillRect(rect, QColor(210, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "POE");

    //slots
    QString card, purchase;
    int devexid,slot,deviceSlot = startSlot, subCount;;
    double subSlotWidth;
    painter->translate(slotGap, slotGap);
    QMap<int, int> slotY;
    slotY.insert(1,2*slotHeight + 2*slotGap);
    slotY.insert(2,slotHeight+slotGap);
    slotY.insert(3,0);

    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0;
    while(i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();

        if(devexid == devexId)
        {

            index = index.sibling(i, 4);
            slot = model->data(index).toInt();

//            painter->save();
//            painter->setFont(smallFont);
            if(deviceSlot < slot)
            {
                //empty slots

                rect.setRect(0, slotY.value(deviceSlot), slotWidth, slotHeight);
                painter->fillRect(rect, QColor(230, 230, 230));
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , QString::number(deviceSlot)+"/0");
                painter->setPen(pen);
                deviceSlot++;
            }
//            painter->restore();

            if(deviceSlot == slot)
            {
                subCount = getSubSlotCount(devexid,slot);
                subSlotWidth = (slotWidth - (subCount - 1)*slotGap)/subCount;

                if(subCount > 0)
                {
                    for(int j = 0; j < subCount; j++)
                    {
                        index = index.sibling(i, 4);
                        slot = model->data(index).toInt();

                        index = index.sibling(i, 6);
                        card = model->data(index).toString();

                        index = index.sibling(i, 15);
                        purchase = model->data(index).toString();

                        if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                            purchase = "";
                        else
                            purchase = card + "#"+ purchase;



                        rect.setRect(j*subSlotWidth+j*slotGap, slotY.value(slot), subSlotWidth, slotHeight);
                        painter->fillRect(rect, QColor(230, 230, 230));
                        painter->drawRect(rect);
//                        painter->save();
//                        painter->setFont(smallFont);
                        painter->setPen(Qt::blue);
                        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , QString::number(deviceSlot)+"/"+QString::number(j));
                        painter->setPen(pen);
//                        painter->restore();

                        if(!card.isEmpty())
                            painter->drawText(rect, Qt::AlignCenter, purchase);

                        i++;
                    }
                    deviceSlot++;
                }
                else
                {
                    i++;
                }

            }
        }
        else i++;
    }

//    painter->save();
//    painter->setFont(smallFont);
    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots

        rect.setRect(0, slotY.value(deviceSlot), slotWidth, slotHeight);
        painter->fillRect(rect, QColor(230, 230, 230));
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , QString::number(deviceSlot)+"/0");
        painter->setPen(pen);
        deviceSlot++;
    }
//    painter->restore();
}

void ReportDevice::paintS9306Device()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    double deviceWidth = 3*painter->viewport().width()/5;
    double deviceHeight  = painter->viewport().height() - headerHeight - bottomHeight - 200;

    if(orient == QPrinter::Portrait)
    {
        deviceHeight = painter->viewport().height()/2;
    }
    shelfHeaderHeight = 50;

    painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    double fanWidth = 50;
    double fanHeight = (deviceHeight - 3*slotGap) / 2;
    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);

    slotWidth = deviceWidth - fanWidth - 3*slotGap;
    slotHeight = (deviceHeight - 10*slotGap) / 10; //pwr>2*
    double pmWidth = (slotWidth - 9*slotGap)/10; // power-mcu-poe
    double pmHeight = 2*slotHeight ;

    //fan
    rect.setRect(deviceWidth -fanWidth - slotGap, 2*slotGap+fanHeight, fanWidth, fanHeight);
    painter->fillRect(rect, QColor(200, 220, 240));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "F\nA\nN");

    rect.setRect(deviceWidth -fanWidth - slotGap, slotGap, fanWidth, fanHeight);
    painter->fillRect(rect, QColor(200, 220, 240));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "F\nA\nN");

    //power-mcu-poe
    QString lbl;
    for(int s = 0 ; s < 10; s++)
    {
        rect.setRect((s+1)*slotGap+s*pmWidth, 9*slotGap+8*slotHeight, pmWidth, pmHeight);
        painter->fillRect(rect, QColor(230, 200, 200));
        painter->drawRect(rect);
        switch (s)
        {
        case 0:
            lbl = "P\nW\nR\n1";
            break;
        case 1:
            lbl = "P\nW\nR\n2";
            break;
        case 2:
            lbl = "P\nW\nR\n3";
            break;
        case 3:
            lbl = "P\nW\nR\n4";
            break;
        case 4:
            lbl = "M\nC\nU";
            break;
        case 5:
            lbl = "M\nC\nU";
            break;
        case 6:
            lbl = "P\nO\nE";
            break;
        case 7:
            lbl = "P\nO\nE";
            break;
        case 8:
            lbl = "P\nO\nE";
            break;
        case 9:
            lbl = "P\nO\nE";
            break;
        }
        painter->drawText(rect, Qt::AlignCenter, lbl);
    }
    //slots
    QString card, purchase;
    int devexid, slot,deviceSlot = startSlot, subCount;
    double subSlotWidth;

    painter->translate(slotGap, slotGap);
    QMap<int,int> slotY;
    slotY.insert(6,0);
    slotY.insert(5,slotHeight+slotGap);
    slotY.insert(4,2*slotHeight+2*slotGap);
    slotY.insert(8,3*slotHeight+3*slotGap);
    slotY.insert(7,4*slotHeight+4*slotGap);
    slotY.insert(3,5*slotHeight+5*slotGap);
    slotY.insert(2,6*slotHeight+6*slotGap);
    slotY.insert(1,7*slotHeight+7*slotGap);

    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0;
    while( i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();
        if(devexid == devexId)
        {
            index = index.sibling(i, 4);
            slot = model->data(index).toInt();

//            painter->save();
//            painter->setFont(smallFont);
            while(deviceSlot < slot)
            {
                //empty slots

                rect.setRect(0, slotY.value(deviceSlot), slotWidth, slotHeight);
                painter->fillRect(rect, QColor(230, 230, 230));
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , QString::number(deviceSlot)+"/0");
                painter->setPen(pen);
                deviceSlot++;
            }
//            painter->restore();

            subCount = getSubSlotCount(devexid,slot);
            subSlotWidth = (slotWidth - (subCount - 1)*slotGap)/subCount;

            if(subCount > 0)
            {
                for(int j = 0; j < subCount; j++)
                {
                    index = index.sibling(i, 4);
                    slot = model->data(index).toInt();

                    index = index.sibling(i, 6);
                    card = model->data(index).toString();

                    index = index.sibling(i, 15);
                    purchase = model->data(index).toString();

                    if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                        purchase = "";
                    else
                        purchase = card + "#"+ purchase;

                    rect.setRect(j*subSlotWidth+j*slotGap, slotY.value(slot), subSlotWidth, slotHeight);
                    painter->fillRect(rect, QColor(230, 230, 230));
                    painter->drawRect(rect);
//                    painter->save();
//                    painter->setFont(smallFont);
                    painter->setPen(Qt::blue);
                    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(slot)+"/"+QString::number(j));
                    painter->setPen(pen);
//                    painter->restore();
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);

                    i++;
                }
                deviceSlot++;
            }
            else  i++;
        }
        else i++;
    }

//    painter->save();
//    painter->setFont(smallFont);
    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots

        rect.setRect(0, slotY.value(deviceSlot), slotWidth, slotHeight);
        painter->fillRect(rect, QColor(230, 230, 230));
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , QString::number(deviceSlot)+"/0");
        painter->setPen(pen);
        deviceSlot++;
    }
//    painter->restore();

    //sru
    rect.setRect(0,slotY.value(8), slotWidth, slotHeight);
    painter->fillRect(rect, QColor(200, 180, 150));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, "8");
    painter->drawText(rect, Qt::AlignCenter, "SRU");

    rect.setRect(0,slotY.value(7), slotWidth, slotHeight);
    painter->fillRect(rect, QColor(200, 180, 150));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, "7");
    painter->drawText(rect, Qt::AlignCenter, "SRU");

}

void ReportDevice::paintS5300Device()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    double deviceWidth = painter->viewport().width()/2;
    double deviceHeight  = 100;

    shelfHeaderHeight = 50;

    painter->translate(deviceWidth/2, 100); //start point

    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->fillRect(rect, QColor(240, 240, 240));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "24 * 1G");
}

void ReportDevice::paintNE40X16Device()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    double deviceWidth = 3*painter->viewport().width()/5;
    double deviceHeight  = painter->viewport().height() - headerHeight - bottomHeight - 200;

    shelfHeaderHeight = 50;

    painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);

    slotWidth = (deviceWidth - 10*slotGap) / 9;
    double sfuWidth = deviceWidth - 2*slotGap;
    slotHeight = (deviceHeight - 7*slotGap - 4*slotWidth)/2;

    QMap<int, int> slotX, slotY;
    slotX.insert(1,0);
    slotX.insert(2,slotWidth+slotGap);
    slotX.insert(3,2*slotWidth+2*slotGap);
    slotX.insert(17,3*slotWidth+3*slotGap);
    slotX.insert(18,4*slotWidth+4*slotGap);
    slotX.insert(4,5*slotWidth+5*slotGap);
    slotX.insert(5,6*slotWidth+6*slotGap);
    slotX.insert(6,7*slotWidth+7*slotGap);
    slotX.insert(7,8*slotWidth+8*slotGap);

    slotX.insert(8,0);
    slotX.insert(9,slotWidth+slotGap);
    slotX.insert(10,2*slotWidth+2*slotGap);
    slotX.insert(11,3*slotWidth+3*slotGap);
    slotX.insert(12,4*slotWidth+4*slotGap);
    slotX.insert(13,5*slotWidth+5*slotGap);
    slotX.insert(14,6*slotWidth+6*slotGap);
    slotX.insert(15,7*slotWidth+7*slotGap);
    slotX.insert(16,8*slotWidth+8*slotGap);

    slotX.insert(19,0);
    slotX.insert(20,0);
    slotX.insert(21,0);
    slotX.insert(22,0);

    slotY.insert(1,0);
    slotY.insert(2,0);
    slotY.insert(3,0);
    slotY.insert(17,0);
    slotY.insert(18,0);
    slotY.insert(4,0);
    slotY.insert(5,0);
    slotY.insert(6,0);
    slotY.insert(7,0);

    //down shelf
    slotY.insert(8,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(9,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(10,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(11,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(12,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(13,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(14,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(15,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(16,slotHeight+4*slotWidth+5*slotGap);

    slotY.insert(19,slotHeight+slotGap);
    slotY.insert(20,slotHeight+slotWidth+2*slotGap);
    slotY.insert(21,slotHeight+2*slotWidth+3*slotGap);
    slotY.insert(22,slotHeight+3*slotWidth+4*slotGap);


    painter->translate(slotGap, slotGap);
    //mpu
    rect.setRect(slotX.value(17), slotY.value(17), slotWidth, slotHeight);
    painter->fillRect(rect,QColor(200, 200, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "M\nP\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "17 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(18), slotY.value(18), slotWidth, slotHeight);
    painter->fillRect(rect,QColor(200, 200, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "M\nP\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "18 ");
    painter->setPen(pen);

    //sfu
    rect.setRect(slotX.value(19), slotY.value(19), sfuWidth, slotWidth);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "SFU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "19 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(20), slotY.value(20), sfuWidth, slotWidth);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "SFU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "20 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(21), slotY.value(21), sfuWidth, slotWidth);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "SFU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "21 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(22), slotY.value(22), sfuWidth, slotWidth);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "SFU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "22 ");
    painter->setPen(pen);

    //slots
    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0,devexid,slot,deviceSlot = startSlot, subCount;
    double subSlotHeight;
    QString card, purchase;
    while( i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();

        if(devexid == devexId)
        {
            index = index.sibling(i, 4);
            slot = model->data(index).toInt();
            subCount = getSubSlotCount(devexid,slot);
            subSlotHeight = (slotHeight - (subCount - 1)*slotGap)/subCount;


            while(deviceSlot < slot)
            {
                //empty slots

                rect.setRect(slotX.value(deviceSlot), slotY.value(deviceSlot), slotWidth, slotHeight);
                painter->fillRect(rect, QColor(230, 230, 230));
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter , QString::number(deviceSlot)+"/0");
                painter->setPen(pen);
                deviceSlot++;
            }

            if(subCount > 0)
            {
                for(int j = 0; j < subCount; j++)
                {
                    index = index.sibling(i, 4);
                    slot = model->data(index).toInt();
                    index = index.sibling(i, 6);
                    card = model->data(index).toString();
                    index = index.sibling(i, 15);
                    purchase = model->data(index).toString();

                    if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                        purchase = "";
                    else
                        purchase = card + "#"+ purchase;

                    purchase = stringToVerticalString(purchase);

                    if((slot > 7) && (slot <17))
                        //down slots - reverse subslot
                        rect.setRect(slotX.value(slot),slotY.value(slot) + slotHeight-(j+1)*subSlotHeight - j*slotGap, slotWidth, subSlotHeight);
                    else
                        rect.setRect(slotX.value(slot),slotY.value(slot) + j*subSlotHeight+j*slotGap, slotWidth, subSlotHeight);

                    painter->fillRect(rect, QColor(230, 230, 230));
                    painter->drawRect(rect);
                    painter->setPen(Qt::blue);
                    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, QString::number(slot)+"/"+QString::number(j));
                    painter->setPen(pen);

                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);



                    i++;
                }
                deviceSlot++;
            }
            else
                i++;

        }else i++;
    }

    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots

        rect.setRect(slotX.value(deviceSlot), slotY.value(deviceSlot), slotWidth, slotHeight);
        painter->fillRect(rect, QColor(230, 230, 230));
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter , QString::number(deviceSlot)+"/0");
        painter->setPen(pen);
        deviceSlot++;
    }
}

void ReportDevice::paintNE40X8Device()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    double deviceWidth = 3*painter->viewport().width()/5;
    double deviceHeight  = deviceWidth ;

    shelfHeaderHeight = 50;

    if( (deviceHeight) > (painter->viewport().height() - bottomHeight -headerHeight - 200) )
    {
        deviceHeight = painter->viewport().height() - headerHeight - bottomHeight - 200;
    }


    painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);

    slotWidth = (deviceWidth - 12*slotGap) / 11;
    double fanWidth = deviceWidth - 2*slotGap;
    double pwrWidth = (deviceWidth - 3*slotGap)/2;

    slotHeight = (deviceHeight - 4*slotGap - 2*slotWidth);

    QMap<int, int> slotX;
    slotX.insert(1,0);
    slotX.insert(2,slotWidth+slotGap);
    slotX.insert(3,2*slotWidth+2*slotGap);
    slotX.insert(4,3*slotWidth+3*slotGap);
    slotX.insert(9,4*slotWidth+4*slotGap);
    slotX.insert(11,5*slotWidth+5*slotGap);
    slotX.insert(10,6*slotWidth+6*slotGap);
    slotX.insert(5,7*slotWidth+7*slotGap);
    slotX.insert(6,8*slotWidth+8*slotGap);
    slotX.insert(7,9*slotWidth+9*slotGap);
    slotX.insert(8,10*slotWidth+10*slotGap);


    //fan
    rect.setRect(slotGap, slotGap, fanWidth, slotWidth);
    painter->fillRect(rect, QColor(240, 250, 240));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "FAN");

    //power
    rect.setRect(slotGap, slotHeight+slotWidth+3*slotGap, pwrWidth, slotWidth);
    painter->fillRect(rect, QColor(220, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "POWER 1");

    rect.setRect(pwrWidth + 2*slotGap, slotHeight+slotWidth+3*slotGap, pwrWidth, slotWidth);
    painter->fillRect(rect, QColor(220, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "POWER 2");

    painter->translate(slotGap, slotWidth + 2*slotGap);// top left slot
    //mpu sru
    rect.setRect(slotX.value(9), 0, slotWidth, slotHeight);
    painter->fillRect(rect,QColor(200, 200, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "M\nP\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "9 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(10), 0, slotWidth, slotHeight);
    painter->fillRect(rect,QColor(200, 200, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "M\nP\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "10 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(11), 0, slotWidth, slotHeight);
    painter->fillRect(rect,QColor(200, 200, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "S\nR\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "11 ");
    painter->setPen(pen);



    //slots
    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0,slot,devexid, subCount,deviceSlot=startSlot;
    double subSlotHeight;
    QString card, purchase;
    while( i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();

        if(devexid == devexId)
        {
            index = index.sibling(i, 4);
            slot = model->data(index).toInt();
            subCount = getSubSlotCount(devexid, slot);
            subSlotHeight = (slotHeight - (subCount - 1)*slotGap)/subCount;
//            painter->save();
//            painter->setFont(smallFont);
            while(deviceSlot < slot)
            {
                //empty slots

                rect.setRect(slotX.value(deviceSlot), 0, slotWidth, slotHeight);// y:(deviceSlot-startSlot)*(slotHeight+slotGap)
                painter->fillRect(rect, QColor(230, 230, 230));
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter , QString::number(deviceSlot)+"/0");
                painter->setPen(pen);
                deviceSlot++;
            }
//            painter->restore();


            if(subCount > 0)
            {
                for(int j = 0; j < subCount; j++)
                {
                    index = index.sibling(i, 4);
                    slot = model->data(index).toInt();

                    index = index.sibling(i, 6);
                    card = model->data(index).toString();

                    index = index.sibling(i, 15);
                    purchase = model->data(index).toString();

                    if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                        purchase = "";
                    else
                        purchase = card + "#"+ purchase;

                    purchase = stringToVerticalString(purchase);

                    rect.setRect(slotX.value(slot), j*subSlotHeight+j*slotGap, slotWidth, subSlotHeight);
                    painter->fillRect(rect, QColor(230, 230, 230));
                    painter->drawRect(rect);
//                    painter->save();
//                    painter->setFont(smallFont);
                    painter->setPen(Qt::blue);
                    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, QString::number(slot)+"/"+QString::number(j));
                    painter->setPen(pen);
//                    painter->restore();
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);

                    i++;
                }
                deviceSlot++;
            }
            else
                i++;
        }else i++;
    }

    //end
//    painter->save();
//    painter->setFont(smallFont);
    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots

        rect.setRect(slotX.value(deviceSlot), 0, slotWidth, slotHeight);
        painter->fillRect(rect, QColor(230, 230, 230));
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter , QString::number(deviceSlot)+"/0");
        painter->setPen(pen);
        deviceSlot++;
    }
//    painter->restore();
}

void ReportDevice::paintNE5000X16Device()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    double deviceWidth = 3*painter->viewport().width()/5;
    double deviceHeight  = painter->viewport().height() - headerHeight - bottomHeight - 200;

    shelfHeaderHeight = 50;

    painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);

    slotWidth = (deviceWidth - 12*slotGap) / 11;
    slotHeight = (deviceHeight - 5*slotGap - 2*slotWidth)/2;

    painter->translate(slotGap, slotGap);

    //fan
    double pfWidth = (deviceWidth - 3*slotGap)/2;

    rect.setRect(0,0,pfWidth, slotWidth);
    painter->fillRect(rect, QColor(220, 240, 220));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "FAN");


    rect.setRect(pfWidth+slotGap,0,pfWidth, slotWidth);
    painter->fillRect(rect, QColor(220, 240, 220));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "FAN");

    //power
    rect.setRect(0,2*slotHeight+slotWidth+3*slotGap,pfWidth, slotWidth);
    painter->fillRect(rect, QColor(220, 240, 220));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "FAN");

    rect.setRect(slotGap+pfWidth,2*slotHeight+slotWidth+3*slotGap,pfWidth, slotWidth);
    painter->fillRect(rect, QColor(220, 240, 220));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "FAN");

    //slots positions
    QMap<int, int> slotX, slotY;
    slotX.insert(1,0);
    slotX.insert(2,slotWidth+slotGap);
    slotX.insert(3,2*slotWidth+2*slotGap);
    slotX.insert(4,3*slotWidth+3*slotGap);
    slotX.insert(17,4*slotWidth+4*slotGap);
    slotX.insert(18,5*slotWidth+5*slotGap);
    slotX.insert(5,6*slotWidth+6*slotGap);
    slotX.insert(6,7*slotWidth+7*slotGap);
    slotX.insert(7,8*slotWidth+8*slotGap);
    slotX.insert(8,9*slotWidth+9*slotGap);
    slotX.insert(9,10*slotWidth+10*slotGap);

    slotX.insert(10,0);
    slotX.insert(11,slotWidth+slotGap);
    slotX.insert(12,2*slotWidth+2*slotGap);
    slotX.insert(13,3*slotWidth+3*slotGap);
    slotX.insert(19,4*slotWidth+4*slotGap);
    slotX.insert(20,5*slotWidth+5*slotGap);
    slotX.insert(21,6*slotWidth+6*slotGap);
    slotX.insert(22,7*slotWidth+7*slotGap);
    slotX.insert(14,8*slotWidth+8*slotGap);
    slotX.insert(15,9*slotWidth+9*slotGap);
    slotX.insert(16,10*slotWidth+10*slotGap);


    slotY.insert(1,0);
    slotY.insert(2,0);
    slotY.insert(3,0);
    slotY.insert(4,0);
    slotY.insert(17,0);
    slotY.insert(18,0);
    slotY.insert(5,0);
    slotY.insert(6,0);
    slotY.insert(7,0);
    slotY.insert(8,0);
    slotY.insert(9,0);

    slotY.insert(10,slotHeight+slotGap);
    slotY.insert(11,slotHeight+slotGap);
    slotY.insert(12,slotHeight+slotGap);
    slotY.insert(13,slotHeight+slotGap);
    slotY.insert(19,slotHeight+slotGap);
    slotY.insert(20,slotHeight+slotGap);
    slotY.insert(21,slotHeight+slotGap);
    slotY.insert(22,slotHeight+slotGap);
    slotY.insert(14,slotHeight+slotGap);
    slotY.insert(15,slotHeight+slotGap);
    slotY.insert(16,slotHeight+slotGap);

    painter->translate(0, slotGap+slotWidth);

    //mpu
    rect.setRect(slotX.value(17), slotY.value(17), slotWidth, slotHeight);
    painter->fillRect(rect,QColor(200, 200, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "M\nP\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "17 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(18), slotY.value(18), slotWidth, slotHeight);
    painter->fillRect(rect,QColor(200, 200, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "M\nP\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "18 ");
    painter->setPen(pen);

    //sfu
    rect.setRect(slotX.value(19), slotY.value(19), slotWidth, slotHeight);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "S\nF\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "19 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(20), slotY.value(20), slotWidth, slotHeight);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "S\nF\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "20 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(21), slotY.value(21), slotWidth, slotHeight);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "S\nF\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "21 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(22), slotY.value(22), slotWidth, slotHeight);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "S\nF\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "22 ");
    painter->setPen(pen);

    //slots
    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0,slot,devexid, subCount,deviceSlot = startSlot;
    double subSlotHeight;
    QString card, purchase;
    while( i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();

        if(devexid == devexId)
        {
            index = index.sibling(i, 4);
            slot = model->data(index).toInt();
            subCount = getSubSlotCount(devexid,slot);
            subSlotHeight = (slotHeight - (subCount - 1)*slotGap)/subCount;

//            painter->save();
//            painter->setFont(smallFont);
            while(deviceSlot < slot)
            {
                //empty slots
                rect.setRect(slotX.value(deviceSlot), slotY.value(deviceSlot), slotWidth, slotHeight);
                painter->fillRect(rect, QColor(230, 230, 230));
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter , QString::number(deviceSlot)+"/0");
                painter->setPen(pen);
                deviceSlot++;
            }
//            painter->restore();

            if(subCount > 0)
            {
                for(int j = 0; j < subCount; j++)
                {
                    index = index.sibling(i, 4);
                    slot = model->data(index).toInt();

                    index = index.sibling(i, 6);
                    card = model->data(index).toString();

                    index = index.sibling(i, 15);
                    purchase = model->data(index).toString();

                    if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                        purchase = "";
                    else
                        purchase = card + "#"+ purchase;

                    purchase = stringToVerticalString(purchase);

//                    if((slot > 9) && (slot <17))
//                        //down slots - reverse subslot
//                        rect.setRect(slotX.value(slot),slotY.value(slot) + slotHeight-(j+1)*subSlotHeight - j*slotGap, slotWidth, subSlotHeight);
//                    else
                        rect.setRect(slotX.value(slot),slotY.value(slot) + j*subSlotHeight+j*slotGap, slotWidth, subSlotHeight);

                    painter->fillRect(rect, QColor(230, 230, 230));
                    painter->drawRect(rect);
//                    painter->save();
//                    painter->setFont(smallFont);
                    painter->setPen(Qt::blue);
                    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, QString::number(slot)+"/"+QString::number(j));
                    painter->setPen(pen);
//                    painter->restore();
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);

                    i++;
                }
                deviceSlot++;
            }
            else
                i++;
        }else i++;
    }

//    painter->save();
//    painter->setFont(smallFont);
    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots

        rect.setRect(slotX.value(deviceSlot), slotY.value(deviceSlot), slotWidth, slotHeight);
        painter->fillRect(rect, QColor(230, 230, 230));
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter , QString::number(deviceSlot)+"/0");
        painter->setPen(pen);
        deviceSlot++;
    }
//    painter->restore();
}

void ReportDevice::paintNE5000X16ADevice()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    double deviceWidth = 3*painter->viewport().width()/5;
    double deviceHeight  = painter->viewport().height() - headerHeight - bottomHeight - 200;

    shelfHeaderHeight = 50;

    painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);

    slotWidth = (deviceWidth - 10*slotGap) / 9;
    double sfuWidth = deviceWidth - 2*slotGap;
    slotHeight = (deviceHeight - 7*slotGap - 4*slotWidth)/2;

    QMap<int, int> slotX, slotY;
    slotX.insert(1,0);
    slotX.insert(2,slotWidth+slotGap);
    slotX.insert(3,2*slotWidth+2*slotGap);
    slotX.insert(17,3*slotWidth+3*slotGap);
    slotX.insert(18,4*slotWidth+4*slotGap);
    slotX.insert(4,5*slotWidth+5*slotGap);
    slotX.insert(5,6*slotWidth+6*slotGap);
    slotX.insert(6,7*slotWidth+7*slotGap);
    slotX.insert(7,8*slotWidth+8*slotGap);

    slotX.insert(8,0);
    slotX.insert(9,slotWidth+slotGap);
    slotX.insert(10,2*slotWidth+2*slotGap);
    slotX.insert(11,3*slotWidth+3*slotGap);
    slotX.insert(12,4*slotWidth+4*slotGap);
    slotX.insert(13,5*slotWidth+5*slotGap);
    slotX.insert(14,6*slotWidth+6*slotGap);
    slotX.insert(15,7*slotWidth+7*slotGap);
    slotX.insert(16,8*slotWidth+8*slotGap);

    slotX.insert(19,0);
    slotX.insert(20,0);
    slotX.insert(21,0);
    slotX.insert(22,0);

    slotY.insert(1,0);
    slotY.insert(2,0);
    slotY.insert(3,0);
    slotY.insert(17,0);
    slotY.insert(18,0);
    slotY.insert(4,0);
    slotY.insert(5,0);
    slotY.insert(6,0);
    slotY.insert(7,0);

    //down shelf
    slotY.insert(8,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(9,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(10,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(11,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(12,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(13,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(14,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(15,slotHeight+4*slotWidth+5*slotGap);
    slotY.insert(16,slotHeight+4*slotWidth+5*slotGap);

    slotY.insert(19,slotHeight+slotGap);
    slotY.insert(20,slotHeight+slotWidth+2*slotGap);
    slotY.insert(21,slotHeight+2*slotWidth+3*slotGap);
    slotY.insert(22,slotHeight+3*slotWidth+4*slotGap);


    painter->translate(slotGap, slotGap);
    //mpu
    rect.setRect(slotX.value(17), slotY.value(17), slotWidth, slotHeight);
    painter->fillRect(rect,QColor(200, 200, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "M\nP\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "17 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(18), slotY.value(18), slotWidth, slotHeight);
    painter->fillRect(rect,QColor(200, 200, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "M\nP\nU");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "18 ");
    painter->setPen(pen);

    //sfu
    rect.setRect(slotX.value(19), slotY.value(19), sfuWidth, slotWidth);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "SFUI");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "19 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(20), slotY.value(20), sfuWidth, slotWidth);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "SFUI");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "20 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(21), slotY.value(21), sfuWidth, slotWidth);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "SFUI");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "21 ");
    painter->setPen(pen);

    rect.setRect(slotX.value(22), slotY.value(22), sfuWidth, slotWidth);
    painter->fillRect(rect,QColor(220, 220, 180));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "SFUI");
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, "22 ");
    painter->setPen(pen);

    //slots
    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0,devexid,slot,deviceSlot = startSlot, subCount;
    double subSlotHeight;
    QString card, purchase;
    while( i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();

        if(devexid == devexId)
        {
            index = index.sibling(i, 4);
            slot = model->data(index).toInt();
            subCount = getSubSlotCount(devexid,slot);
            subSlotHeight = (slotHeight - (subCount - 1)*slotGap)/subCount;


            while(deviceSlot < slot)
            {
                //empty slots

                rect.setRect(slotX.value(deviceSlot), slotY.value(deviceSlot), slotWidth, slotHeight);
                painter->fillRect(rect, QColor(230, 230, 230));
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter , QString::number(deviceSlot)+"/0");
                painter->setPen(pen);
                deviceSlot++;
            }

            if(subCount > 0)
            {
                for(int j = 0; j < subCount; j++)
                {
                    index = index.sibling(i, 4);
                    slot = model->data(index).toInt();
                    index = index.sibling(i, 6);
                    card = model->data(index).toString();
                    index = index.sibling(i, 15);
                    purchase = model->data(index).toString();

                    if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                        purchase = "";
                    else
                        purchase = card + "#"+ purchase;

                    purchase = stringToVerticalString(purchase);

                    if((slot > 7) && (slot <17))
                        //down slots - reverse subslot
                        rect.setRect(slotX.value(slot),slotY.value(slot) + slotHeight-(j+1)*subSlotHeight - j*slotGap, slotWidth, subSlotHeight);
                    else
                        rect.setRect(slotX.value(slot),slotY.value(slot) + j*subSlotHeight+j*slotGap, slotWidth, subSlotHeight);

                    painter->fillRect(rect, QColor(230, 230, 230));
                    painter->drawRect(rect);
                    painter->setPen(Qt::blue);
                    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, QString::number(slot)+"/"+QString::number(j));
                    painter->setPen(pen);

                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);



                    i++;
                }
                deviceSlot++;
            }
            else
                i++;

        }else i++;
    }

    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots

        rect.setRect(slotX.value(deviceSlot), slotY.value(deviceSlot), slotWidth, slotHeight);
        painter->fillRect(rect, QColor(230, 230, 230));
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter , QString::number(deviceSlot)+"/0");
        painter->setPen(pen);
        deviceSlot++;
    }
}

void ReportDevice::paint7613Device()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);
    shelfHeaderHeight = 50;

    double deviceWidth = 3*painter->viewport().width()/5;
    double deviceHeight  = painter->viewport().height() - headerHeight- 200 - bottomHeight;

    if(orient == QPrinter::Portrait)
    {
        deviceHeight = painter->viewport().height()/2;
    }


    painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    double powerHeight = 50;
    double powerWidth = (deviceWidth - 3*slotGap) / 2;
    double fanWidth = 50;
    double fanHeight = deviceHeight - 3*slotGap - powerHeight;
    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);

    slotWidth = deviceWidth - fanWidth - 3*slotGap;
    slotHeight = (fanHeight - 12*slotGap) / 13;

    //fan
    rect.setRect(slotGap, slotGap, fanWidth, fanHeight);
    painter->fillRect(rect, QColor(200, 220, 240));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "F\nA\nN");

    //power
    rect.setRect(slotGap, fanHeight + 2*slotGap, powerWidth, powerHeight);
    painter->fillRect(rect, QColor(230, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "Power 1");

    rect.setRect(powerWidth + 2*slotGap, fanHeight + 2*slotGap, powerWidth, powerHeight);
    painter->fillRect(rect, QColor(230, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "Power 2");

    //slots
    QString card, purchase;
    int devexid, slot,deviceSlot = startSlot, subCount;
    double subSlotWidth;

    painter->translate(fanWidth+2*slotGap, slotGap);
    QMap<int,int> slotY;
    slotY.insert(1,0);
    slotY.insert(2,slotHeight+slotGap);
    slotY.insert(3,2*slotHeight+2*slotGap);
    slotY.insert(4,3*slotHeight+3*slotGap);
    slotY.insert(5,4*slotHeight+4*slotGap);
    slotY.insert(6,5*slotHeight+5*slotGap);
    slotY.insert(7,6*slotHeight+6*slotGap);
    slotY.insert(8,7*slotHeight+7*slotGap);
    slotY.insert(9,8*slotHeight+8*slotGap);
    slotY.insert(10,9*slotHeight+9*slotGap);
    slotY.insert(11,10*slotHeight+10*slotGap);
    slotY.insert(12,11*slotHeight+11*slotGap);
    slotY.insert(13,12*slotHeight+12*slotGap);

//    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
//    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0;
    while( i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();
        if(devexid == devexId)
        {
            index = index.sibling(i, 4);
            slot = model->data(index).toInt();

//            painter->save();
//            painter->setFont(smallFont);
            while(deviceSlot < slot)
            {
                //empty slots

                rect.setRect(0, slotY.value(deviceSlot), slotWidth, slotHeight);
                painter->fillRect(rect, QColor(230, 230, 230));
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , QString::number(deviceSlot)+"/0");
                painter->setPen(pen);
                deviceSlot++;
            }
//            painter->restore();

            subCount = getSubSlotCount(devexid,slot);
            subSlotWidth = (slotWidth - (subCount - 1)*slotGap)/subCount;

            if(subCount > 0)
            {
                for(int j = 0; j < subCount; j++)
                {
                    index = index.sibling(i, 4);
                    slot = model->data(index).toInt();

                    index = index.sibling(i, 6);
                    card = model->data(index).toString();

                    index = index.sibling(i, 15);
                    purchase = model->data(index).toString();

                    if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                        purchase = "";
                    else
                        purchase = card + "#"+ purchase;

                    rect.setRect(j*subSlotWidth+j*slotGap, slotY.value(slot), subSlotWidth, slotHeight);
                    painter->fillRect(rect, QColor(230, 230, 230));
                    painter->drawRect(rect);
//                    painter->save();
//                    painter->setFont(smallFont);
                    painter->setPen(Qt::blue);
                    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(slot)+"/"+QString::number(j));
                    painter->setPen(pen);
//                    painter->restore();
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);

                    i++;
                }
                deviceSlot++;
            }
            else  i++;
        }
        else i++;
    }

//    painter->save();
//    painter->setFont(smallFont);
    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots

        rect.setRect(0, slotY.value(deviceSlot), slotWidth, slotHeight);
        painter->fillRect(rect, QColor(230, 230, 230));
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , QString::number(deviceSlot)+"/0");
        painter->setPen(pen);
        deviceSlot++;
    }
//    painter->restore();
}

void ReportDevice::paintASR9010Device()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    double deviceWidth = 3*painter->viewport().width()/5;
    double deviceHeight  = painter->viewport().height() - headerHeight - bottomHeight - 200;

    if(orient == QPrinter::Portrait)
        deviceHeight -=200;

    shelfHeaderHeight = 50;

    painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);

    slotWidth = (deviceWidth - 11*slotGap) / 10;
    double fanAirWidth = deviceWidth - 2*slotGap;
    double fanAirHeight = 50, pwrHeight = 50;
    double pwrWidth = (deviceWidth - 4*slotGap)/3;

    slotHeight = (deviceHeight - 3*fanAirHeight - 2*pwrHeight - 7*slotGap);


    //fan
    rect.setRect(slotGap, slotHeight +2*slotGap, fanAirWidth, fanAirHeight);
    painter->fillRect(rect, QColor(200, 220, 240));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "Fan Tray 0");
    rect.setRect(slotGap, slotHeight +3*slotGap +fanAirHeight, fanAirWidth, fanAirHeight);
    painter->fillRect(rect, QColor(200, 220, 240));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "Fan Tray 1");
    //air intake
    rect.setRect(slotGap, slotHeight +4*slotGap +2*fanAirHeight, fanAirWidth, fanAirHeight);
    painter->fillRect(rect, QColor(230, 240, 240));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "Air Intake");

    //power
    rect.setRect(slotGap, slotHeight +5*slotGap +3*fanAirHeight, pwrWidth, pwrHeight);
    painter->fillRect(rect, QColor(220, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "POWER 0");

    rect.setRect(pwrWidth+2*slotGap, slotHeight +5*slotGap +3*fanAirHeight, pwrWidth, pwrHeight);
    painter->fillRect(rect, QColor(220, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "POWER 1");

    rect.setRect(2*pwrWidth +3*slotGap, slotHeight +5*slotGap +3*fanAirHeight, pwrWidth, pwrHeight);
    painter->fillRect(rect, QColor(220, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "POWER 2");

    rect.setRect(slotGap, slotHeight +6*slotGap +3*fanAirHeight+pwrHeight, pwrWidth, pwrHeight);
    painter->fillRect(rect, QColor(220, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "POWER 0");

    rect.setRect(pwrWidth+2*slotGap,  slotHeight +6*slotGap +3*fanAirHeight+pwrHeight, pwrWidth, pwrHeight);
    painter->fillRect(rect, QColor(220, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "POWER 1");

    rect.setRect(2*pwrWidth +3*slotGap,  slotHeight +6*slotGap +3*fanAirHeight+pwrHeight, pwrWidth, pwrHeight);
    painter->fillRect(rect, QColor(220, 200, 200));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, "POWER 2");

    painter->translate(slotGap, slotGap);

    QMap<int, int> slotX;
    slotX.insert(0,0);
    slotX.insert(1,slotWidth+slotGap);
    slotX.insert(2,2*slotWidth+2*slotGap);
    slotX.insert(3,3*slotWidth+3*slotGap);
    slotX.insert(4,4*slotWidth+4*slotGap);
    slotX.insert(5,5*slotWidth+5*slotGap);
    slotX.insert(6,6*slotWidth+6*slotGap);
    slotX.insert(7,7*slotWidth+7*slotGap);
    slotX.insert(8,8*slotWidth+8*slotGap);
    slotX.insert(9,9*slotWidth+9*slotGap);

    //slots
    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0,slot,devexid, subCount,deviceSlot=startSlot;
    double subSlotHeight;
    QString card, purchase;
    while( i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();

        if(devexid == devexId)
        {
            index = index.sibling(i, 4);
            slot = model->data(index).toInt();
            subCount = getSubSlotCount(devexid, slot);
            subSlotHeight = (slotHeight - (subCount - 1)*slotGap)/subCount;

//            painter->save();
//            painter->setFont(smallFont);
            while(deviceSlot < slot)
            {
                //empty slots

                rect.setRect(slotX.value(deviceSlot),0, slotWidth, slotHeight);
                painter->fillRect(rect, QColor(230, 230, 230));
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter , "0/"+QString::number(deviceSlot)+"/0");
                painter->setPen(pen);
                deviceSlot++;
            }
//            painter->restore();

            if(subCount > 0)
            {
                for(int j = 0; j < subCount; j++)
                {
                    index = index.sibling(i, 4);
                    slot = model->data(index).toInt();

                    index = index.sibling(i, 6);
                    card = model->data(index).toString();

                    index = index.sibling(i, 15);
                    purchase = model->data(index).toString();

                    if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                        card = "";
                    else
                        card = card + "#"+ purchase;

                    card = stringToVerticalString(card);

                    rect.setRect(slotX.value(slot), j*(subSlotHeight+slotGap), slotWidth, subSlotHeight);
                    if((slot == 4) || (slot == 5) )
                        painter->fillRect(rect, QColor(200, 200, 210));
                    else
                        painter->fillRect(rect, QColor(230, 230, 230));
                    painter->drawRect(rect);
//                    painter->save();
//                    painter->setFont(smallFont);
                    painter->setPen(Qt::blue);
                    painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, "0/"+QString::number(slot)+"/"+QString::number(j));
                    painter->setPen(pen);
//                    painter->restore();
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, card);

                    i++;
                }
                deviceSlot++;
            }
            else
                i++;
        }else i++;
    }

//    painter->save();
//    painter->setFont(smallFont);
    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots

        rect.setRect(slotX.value(deviceSlot), 0, slotWidth, slotHeight);
        painter->fillRect(rect, QColor(230, 230, 230));
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter , "0/"+QString::number(deviceSlot)+"/0");
        painter->setPen(pen);
        deviceSlot++;
    }
//    painter->restore();
}

void ReportDevice::paintASR1013Device()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    //overall 10 slot
    //4 slot : ESP ESP RP RP
    //6 slot line
    // find sip:4sub/mip:2sub/elc:1/empty:0   back cards can be one of these types
    QMap<int, int> subCountMap;
    subCountMap.insert(0, getSubSlotCount(devexId,0, true));
    subCountMap.insert(1, getSubSlotCount(devexId,1, true));
    subCountMap.insert(2, getSubSlotCount(devexId,2, true));
    subCountMap.insert(3, getSubSlotCount(devexId,3, true));
    subCountMap.insert(4, getSubSlotCount(devexId,4, true));
    subCountMap.insert(5, getSubSlotCount(devexId,5, true));
    int sipCount = 0;
    QList<int> keys = subCountMap.keys();
    foreach(int i,  keys)
    {
        if(subCountMap.value(i) > 2)
            sipCount++;
    }
    double deviceHeight  = sipCount*80+(6-sipCount)*50 + 11*slotGap + 200;
    //slot height ::  SIP(5xsub) = 80  / MIP(2xsub): 50 / ELC(1 sub-full slot)=50
    double deviceWidth = 3*painter->viewport().width()/5;
    double slotWidth = deviceWidth - 2*slotGap;
    shelfHeaderHeight = 50;

    painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);
    painter->translate(slotGap, slotGap);

    QMap<int,int> slotY;
    int h = 0;
    slotY.insert(5,0);
    if(subCountMap.value(5) > 2)
        h = 80;
    else
        h = 50;
    slotY.insert(4,h+slotGap);
    if(subCountMap.value(4) > 2)
        h += 80;
    else
        h += 50;
    slotY.insert(3,h+2*slotGap);
    if(subCountMap.value(3) > 2)
        h += 80;
    else
        h += 50;
    slotY.insert(2,h+200+7*slotGap);
    if(subCountMap.value(2) > 2)
        h += 80;
    else
        h += 50;
    slotY.insert(1,h+200+8*slotGap);
    if(subCountMap.value(1) > 2)
        h += 80;
    else
        h += 50;
    slotY.insert(0,h+200+9*slotGap);


    QMap<int,int> slotHeightMap;
    h = (subCountMap.value(0) > 2)?80:50;
    slotHeightMap.insert(0, h);
    h = (subCountMap.value(1) > 2)?80:50;
    slotHeightMap.insert(1, h);
    h = (subCountMap.value(2) > 2)?80:50;
    slotHeightMap.insert(2, h);
    h = (subCountMap.value(3) > 2)?80:50;
    slotHeightMap.insert(3, h);
    h = (subCountMap.value(4) > 2)?80:50;
    slotHeightMap.insert(4, h);
    h = (subCountMap.value(5) > 2)?80:50;
    slotHeightMap.insert(5, h);

    //draw chassiss
    //slot 5
    rect.setRect(0, slotY.value(5), slotWidth, slotHeightMap.value(5));
    painter->fillRect(rect, QColor(230, 230, 230));
    painter->drawRect(rect);
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " 5");
    painter->setPen(pen);
    //slot 4
    rect.setRect(0, slotY.value(4), slotWidth, slotHeightMap.value(4));
    painter->fillRect(rect, QColor(230, 230, 230));
    painter->drawRect(rect);
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " 4");
    painter->setPen(pen);
    //slot 3
    rect.setRect(0, slotY.value(3), slotWidth, slotHeightMap.value(3));
    painter->fillRect(rect, QColor(230, 230, 230));
    painter->drawRect(rect);
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " 3");
    painter->setPen(pen);
    //RP2
    h = slotY.value(3)+slotHeightMap.value(3);
    rect.setRect(0, h+slotGap , slotWidth, 50);
    painter->fillRect(rect, QColor(215, 235, 235));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter, "RP2-R1");
    painter->setPen(pen);
    //ESP-F1
    h = slotY.value(3)+slotHeightMap.value(3)+50;
    rect.setRect(0, h+2*slotGap , slotWidth, 50);
    painter->fillRect(rect, QColor(215, 235, 235));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter, "ESP-F1");
    painter->setPen(pen);
    //ESP-F0
    h = slotY.value(3)+slotHeightMap.value(3)+100;
    rect.setRect(0, h+3*slotGap , slotWidth, 50);
    painter->fillRect(rect, QColor(215, 235, 235));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter, "ESP-F0");
    painter->setPen(pen);
    //RP2
    h = slotY.value(3)+slotHeightMap.value(3)+150;
    rect.setRect(0, h+4*slotGap , slotWidth, 50);
    painter->fillRect(rect, QColor(215, 235, 235));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter, "RP2-R0");
    painter->setPen(pen);
    //slot 2
    rect.setRect(0, slotY.value(2), slotWidth, slotHeightMap.value(2));
    painter->fillRect(rect, QColor(230, 230, 230));
    painter->drawRect(rect);
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " 2");
    painter->setPen(pen);
    //slot 1
    rect.setRect(0, slotY.value(1), slotWidth, slotHeightMap.value(1));
    painter->fillRect(rect, QColor(230, 230, 230));
    painter->drawRect(rect);
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " 1");
    painter->setPen(pen);
    //slot 0
    rect.setRect(0, slotY.value(0), slotWidth, slotHeightMap.value(0));
    painter->fillRect(rect, QColor(230, 230, 230));
    painter->drawRect(rect);
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " 0");
    painter->setPen(pen);


    //slots
    QString card, purchase;
    int devexid, slot, subslot;
    double subSlotWidth, subslotHeight;

    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0;
    while( i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();
        if(devexid == devexId)
        {
            index = index.sibling(i, 4);
            slot = model->data(index).toInt();
            index = index.sibling(i, 5);
            subslot = model->data(index).toInt();
            index = index.sibling(i, 6);
            card = model->data(index).toString();
            index = index.sibling(i, 15);
            purchase = model->data(index).toString();
            if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                purchase = "";
            else
                purchase = card + "#"+ purchase;

            if(subCountMap.value(slot) == 1)
            {
                subSlotWidth = slotWidth - 10 - 2*slotGap;
                subslotHeight = 50;

                if(subslot == 0)
                {
                    rect.setRect(slotGap+10, slotY.value(slot), subSlotWidth, subslotHeight);
                    painter->fillRect(rect, QColor(220, 210, 210));
                    painter->drawRect(rect);
                    painter->setPen(Qt::darkMagenta);
                    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(subslot));
                    painter->setPen(pen);
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);
                }

            }
            else if(subCountMap.value(slot) == 2)
            {
                subSlotWidth = (slotWidth - 10 - 3*slotGap)/2;
                subslotHeight = 50;

                if(subslot == 0)
                {
                    rect.setRect(slotGap+10, slotY.value(slot), subSlotWidth, subslotHeight);
                    painter->fillRect(rect, QColor(220, 210, 210));
                    painter->drawRect(rect);
                    painter->setPen(Qt::darkMagenta);
                    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(subslot));
                    painter->setPen(pen);
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);
                }
                else if(subslot == 1)
                {
                    rect.setRect(2*slotGap+10+subSlotWidth, slotY.value(slot), subSlotWidth, subslotHeight);
                    painter->fillRect(rect, QColor(220, 210, 210));
                    painter->drawRect(rect);
                    painter->setPen(Qt::darkMagenta);
                    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(subslot));
                    painter->setPen(pen);
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);
                }

            }
            else if(subCountMap.value(slot) > 2)
            {
                subSlotWidth = (slotWidth - 10 - 3*slotGap)/2;
                subslotHeight = (slotHeightMap.value(slot)-3*slotGap)/2;

                if(subslot == 0)
                {
                    rect.setRect(slotGap+10, slotY.value(slot)+slotGap, subSlotWidth, subslotHeight);
                    painter->fillRect(rect, QColor(220, 210, 210));
                    painter->drawRect(rect);
                    painter->setPen(Qt::darkMagenta);
                    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(subslot));
                    painter->setPen(pen);
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);
                }
                else if(subslot == 1)
                {
                    rect.setRect(2*slotGap+10+subSlotWidth, slotY.value(slot)+slotGap, subSlotWidth, subslotHeight);
                    painter->fillRect(rect, QColor(220, 210, 210));
                    painter->drawRect(rect);
                    painter->setPen(Qt::darkMagenta);
                    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(subslot));
                    painter->setPen(pen);
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);
                }
                else if(subslot == 2)
                {
                    rect.setRect(slotGap+10, slotY.value(slot)+2*slotGap+subslotHeight, subSlotWidth, subslotHeight);
                    painter->fillRect(rect, QColor(220, 210, 210));
                    painter->drawRect(rect);
                    painter->setPen(Qt::darkMagenta);
                    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(subslot));
                    painter->setPen(pen);
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);
                }
                else if(subslot == 3)
                {
                    rect.setRect(2*slotGap+10+subSlotWidth, slotY.value(slot)+2*slotGap+subslotHeight, subSlotWidth, subslotHeight);
                    painter->fillRect(rect, QColor(220, 210, 210));
                    painter->drawRect(rect);
                    painter->setPen(Qt::darkMagenta);
                    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(subslot));
                    painter->setPen(pen);
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);
                }
            }
            else
            {
                i++;
                continue;
            }

        }

        i++;
    }

}

void ReportDevice::paintASR1002XDevice()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    double deviceWidth = 3*painter->viewport().width()/5;
    double deviceHeight  = 150 +4*slotGap;
    shelfHeaderHeight = 50;

    painter->translate((painter->viewport().width()-deviceWidth)/2, 100); //start point

    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);
    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);

    slotWidth = (deviceWidth - 3*slotGap)/2;
    slotHeight = 50;

    rect.setRect(slotGap, slotGap, deviceWidth-2*slotGap, slotHeight);
    painter->fillRect(rect, QColor(200, 220, 240));
    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "ESP-F0");
    painter->translate(slotGap, slotHeight+2*slotGap);

    //slots
    QString card, purchase;
    int devexid, subslot;

    QMap<int,int> subslotY;
    subslotY.insert(0,0);
    subslotY.insert(1,0);
    subslotY.insert(2,slotHeight+slotGap);
    subslotY.insert(3,slotHeight+slotGap);

    QMap<int,int> subslotX;
    subslotX.insert(0,0);
    subslotX.insert(1,slotWidth+slotGap);
    subslotX.insert(2,0);
    subslotX.insert(3,slotWidth+slotGap);

    //draw chassiss
    //sub 0
    rect.setRect(subslotX.value(0), subslotY.value(0), slotWidth, slotHeight);
    painter->fillRect(rect, QColor(230, 230, 230));
    painter->drawRect(rect);
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, "0/0");
    painter->setPen(pen);
    //sub 1
    rect.setRect(subslotX.value(1), subslotY.value(1), slotWidth, slotHeight);
    painter->fillRect(rect, QColor(230, 230, 230));
    painter->drawRect(rect);
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, "0/1");
    painter->setPen(pen);
    //sub 2
    rect.setRect(subslotX.value(2), subslotY.value(2), slotWidth, slotHeight);
    painter->fillRect(rect, QColor(230, 230, 230));
    painter->drawRect(rect);
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, "0/2");
    painter->setPen(pen);
    //sub 3
    rect.setRect(subslotX.value(3), subslotY.value(3), slotWidth, slotHeight);
    painter->fillRect(rect, QColor(230, 230, 230));
    painter->drawRect(rect);
    painter->setPen(Qt::blue);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, "0/3");
    painter->setPen(pen);

    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0;
    while( i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();
        if(devexid == devexId)
        {
            index = index.sibling(i, 5);
            subslot = model->data(index).toInt();
            index = index.sibling(i, 6);
            card = model->data(index).toString();

            index = index.sibling(i, 15);
            purchase = model->data(index).toString();

            if(card.compare("EMPTY", Qt::CaseInsensitive) == 0)
                purchase = "";
            else
                purchase = card + "#"+ purchase;

            rect.setRect(subslotX.value(subslot), subslotY.value(subslot), slotWidth, slotHeight);
            painter->setPen(pen);
            if(!card.isEmpty())
                painter->drawText(rect, Qt::AlignCenter, purchase);
        }

        i++;
    }
}

void ReportDevice::paintUnkownDevice()
{
    QRect rect(0,0, painter->viewport().width(), 50);
    painter->fillRect(rect,QColor(240, 240, 245));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, device);

    painter->setFont(font);

    double deviceWidth = painter->viewport().width()/2;
    double deviceHeight  = painter->viewport().height() - headerHeight - bottomHeight - 200;

    shelfHeaderHeight = 50;

    painter->translate(deviceWidth/2, 100); //start point

    //header
    rect.setRect(0,0,deviceWidth, shelfHeaderHeight);
    painter->fillRect(rect, QColor(200, 230, 200));
    painter->drawRect(rect);
    painter->setFont(headerFont);
    painter->drawText(rect, Qt::AlignCenter, devexName);
    painter->translate(0,shelfHeaderHeight);
    painter->setFont(font);

    slotWidth = (deviceWidth - 2*slotGap);

    slotHeight = (deviceHeight - (slotCount+1)*slotGap)/slotCount;
    if(slotHeight > 50)
    {
        slotHeight = 50;
        deviceHeight = slotCount*(slotHeight + slotGap) + slotGap;
    }


    //frame
    rect.setRect(0,0,deviceWidth, deviceHeight);
    painter->drawRect(rect);

    painter->translate(slotGap, slotGap);

    //slots
    //0`slots_id`, 1`devex_id`, 2`card_id`, 3`shelf`, 4`slot`, 5`sub_slot`, 6`card`, 7`adsl`, 8`vdsl`, 9`shdsl`,
    //10`e`, 11`g`, 12`g10`, 13`g40`, 14`g100`, 15`purchase`, 16`description`
    QModelIndex index = model->index(0,4);
    int i = 0,slot,devexid, subCount,deviceSlot = startSlot;
    double subSlotWidth;
    QString card, purchase;
    while( i < model->rowCount())
    {
        index = index.sibling(i, 1);
        devexid = model->data(index).toInt();

        if(devexid == devexId)
        {
            index = index.sibling(i, 4);
            slot = model->data(index).toInt();

            subCount = getSubSlotCount(devexid, slot);

            subSlotWidth = (slotWidth - (subCount - 1)*slotGap)/subCount;

            while(deviceSlot < slot)
            {
                //empty slots
                rect.setRect(0, (deviceSlot-startSlot)*(slotHeight+slotGap), slotWidth, slotHeight);
                painter->fillRect(rect, QColor(230, 230, 230));
                painter->drawRect(rect);
                painter->setPen(Qt::blue);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , QString::number(deviceSlot)+"/0");
                painter->setPen(pen);
                deviceSlot++;
            }

            if(subCount > 0)
            {
                for(int j = 0; j < subCount; j++)
                {
                    index = index.sibling(i, 4);
                    slot = model->data(index).toInt();

                    index = index.sibling(i, 6);
                    card = model->data(index).toString();

                    index = index.sibling(i, 15);
                    purchase = model->data(index).toString();

                    purchase = card + "#"+ purchase;

                    if(startSlot == 0)
                        rect.setRect(j*subSlotWidth+j*slotGap,(slot)*(slotHeight+slotGap), subSlotWidth, slotHeight);
                    else
                        rect.setRect(j*subSlotWidth+j*slotGap,(slot-1)*(slotHeight+slotGap), subSlotWidth, slotHeight);
                    painter->fillRect(rect, QColor(230, 230, 230));
                    painter->drawRect(rect);
                    painter->setPen(Qt::blue);
                    painter->drawText(rect, Qt::AlignRight | Qt::AlignVCenter, QString::number(slot)+"/"+QString::number(j));
                    painter->setPen(pen);
                    if(!card.isEmpty())
                        painter->drawText(rect, Qt::AlignCenter, purchase);

                    i++;
                }
                deviceSlot++;
            }
            else
                i++;

        }else i++;
    }

    while(deviceSlot < (slotCount+startSlot))
    {
        //empty slots
        rect.setRect(0, (deviceSlot-startSlot)*(slotHeight+slotGap), slotWidth, slotHeight);
        painter->fillRect(rect, QColor(230, 230, 230));
        painter->drawRect(rect);
        painter->setPen(Qt::blue);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter , QString::number(deviceSlot)+"/0");
        painter->setPen(pen);
        deviceSlot++;
    }
}

QString ReportDevice::lastError()
{
    return error;
}

QString ReportDevice::stringToVerticalString(QString string)
{

    if(string.isEmpty())
        return "";

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

//    if(string.isEmpty())
//        return "";

//    QString res = "";
//    QChar ch;
//    int len = string.length();
//    for(int i = 0; i < len; i++)
//    {
//        ch = string.at(i);
//        if( (i != 0) || (i != len-1) )
//            res +='\n'+ch;
//        else
//            res += ch;
//    }

//    return res;
}





