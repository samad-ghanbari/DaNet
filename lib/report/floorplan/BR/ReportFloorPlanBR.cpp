#include "ReportFloorPlanBR.h"
#include <QAbstractItemModel>
#include <QDate>
#include <QPainter>
#include <QDebug>

ReportFloorPlanBR::ReportFloorPlanBR(QObject *parent, QAbstractItemModel *Model, const QString Abbr, const QString Exchange, const QString Saloon, const QString OutputPath, const int PaperSize, const int Orient, const QFont Font, const QString CustomInfo) :
    QObject(parent),
    painter(new QPainter()),
    model(Model),
    outputPath(OutputPath),
    abbr(Abbr),
    exchange(Exchange),
    saloon(Saloon)
{

    if(CustomInfo.trimmed().isEmpty())
    {
        CUSTOM_INFO = false;
        customInfo = "";
        headerHeight = 90;
    }
    else
    {
        CUSTOM_INFO = true;
        customInfo = CustomInfo;
        headerHeight = 120;
    }

    bottomHeight = 40;
    leftMargin = 0;
    rightMargin = 100;

    pen = QPen(QColor(0, 0, 80), 1, Qt::SolidLine);
    font = Font;
    currentDate = QDate::currentDate().toString("yyyy/MM/dd");
    error = "No error";
    pageNumber = 1;
    rackHeight = 150;
    rackWidth = 100;
    odfWidth = 50;
    rackGap = 5;
    SRN = getStartRackNo();

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


ReportFloorPlanBR::~ReportFloorPlanBR()
{
    delete painter;
    painter = NULL;
}

void ReportFloorPlanBR::preparePage()
{

    painter->resetTransform();
    painter->save();

    painter->setPen(Qt::darkMagenta);
    painter->setFont(QFont("tahoma", 14, QFont::Bold));
    QRect rec(10,10,painter->viewport().width()-100,30);
    painter->drawText(rec,Qt::AlignLeft, currentDate);
    painter->drawText(rec,Qt::AlignCenter,"DaNet Floor Plan Report");
    rec.setRect(10,40,painter->viewport().width()-100,30);
    painter->drawText(rec, Qt::AlignLeft,abbr+"- Saloon: "+saloon);
    painter->drawText(rec, Qt::AlignCenter,exchange);
    QPixmap im(":/Image/tct.jpg");
    painter->drawPixmap(painter->viewport().width()-50, 15,im);

    if(CUSTOM_INFO)
    {
        rec.setRect(10,70,painter->viewport().width()-20,30);
        painter->setPen(Qt::darkCyan);
        painter->drawText(rec, Qt::AlignLeft,customInfo);
        painter->setPen(Qt::darkMagenta);

        QPen penTemp = QPen(Qt::magenta, 1, Qt::DashDotDotLine);
        painter->setPen(penTemp);
        painter->drawLine(0, 110,painter->viewport().width(), 110);
        painter->setPen(Qt::darkMagenta);
    }
    else
    {
        QPen penTemp = QPen(Qt::magenta, 1, Qt::DashDotDotLine);
        painter->setPen(penTemp);
        painter->drawLine(0, 75,painter->viewport().width(), 75);
        painter->setPen(Qt::darkMagenta);
    }

    //page number
    painter->setFont(QFont("Tahoma", 10));
    painter->translate(10, painter->viewport().height()-10 );
    painter->setPen(QPen(QColor(0, 0, 0), 2));
    painter->drawText(0, 0, QString("Page %1").arg(pageNumber));
    pageNumber += 1;

    painter->restore();

    painter->translate(painter->viewport().width(), painter->viewport().height()-bottomHeight-rackHeight-50); // start print point
}

bool ReportFloorPlanBR::print()
{
    if( model->rowCount() == 0 )
    {
        error = "Saloon is empty";
        return false;
    }

    QPrinter *printer = new QPrinter(QPrinter::ScreenResolution);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOrientation(orient);
    printer->setPaperSize(paperSize);
    printer->setOutputFileName(outputPath);
    printer->setCreator("DaNet Application");
    printer->setDocName("Floor Plan Report");
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
        painter->end();
        return false;
    }
    if(!painter->isActive())
    {
        error = "painter.isActive() == false";
        painter->end();
        return false;
    }
    //--------------------------------------------------------------------------------------

    pen.setColor(Qt::darkBlue);
    painter->setPen(pen);

    preparePage();

    startRackRow = true;
    int row, rack, odfNo, ppp, rowCount = model->rowCount();
    bool ro;
    QString dev;
    QModelIndex index = model->index(0,0);
    currentRackRow = -1000;
    QMap<int, QList<QString>> map;
    modelRow = 0;

    while(modelRow < rowCount)
    {
        //row-rack-shelf-device-RO(rack/Odf)-odfNo, ppp

        index = index.sibling(modelRow, 0);
        row = model->data(index).toInt();

        index = index.sibling(modelRow, 1);
        rack = model->data(index).toInt();

        index = index.sibling(modelRow, 4);
        ro = model->data(index).toBool();

        if(currentRackRow != row)
        {
            currentRackRow = row;
            if(startRackRow)
            {
                startRackRow = false;// set in page preparation
            }
            else
            {
                painter->translate(rightMargin, -rackHeight-50);

                if( painter->transform().dy() < headerHeight)
                {
                    printer->newPage();
                    preparePage();
                }
            }

            drawRackRow(row);
        }

        if(ro)
        {
            //odf
            index = index.sibling(modelRow, 3);
            dev = model->data(index).toString();

            index = index.sibling(modelRow, 5);
            if(model->data(index).isNull())
                odfNo = -1;
            else
                odfNo = model->data(index).toInt();

            index = index.sibling(modelRow, 6);
            ppp = model->data(index).toInt();

            drawOdf(row,rack,dev,odfNo,ppp);
            modelRow++;

        }
        else
        {//rack
            map = getrackInfo();
            if(map.count() > 0)
            {
                QMapIterator<int, QList<QString>> iterator(map);
                iterator.next();
                drawRack(row,rack, iterator.key(), iterator.value());
            }
        }
    }

    painter->end();
    return true;
}

int ReportFloorPlanBR::getStartRackNo()
{
    int srn = 1;
    int rn;
    QModelIndex index = model->index(0,1);
    for(int i = 0; i < model->rowCount() ; i++)
    {
        index = index.sibling(i, 1);
        rn = model->data(index).toInt();
        if(srn > rn) srn = rn;
    }
    return srn;
}

QMap<int, QList<QString> > ReportFloorPlanBR::getrackInfo()
{
    QMap<int, QList<QString>> rackInfo;
    QList<QString> deviceList;

    int row,rack,currentRack, shelfCount;
    QString device;
    QModelIndex index = model->index(modelRow, 1);
    currentRack = model->data(index).toInt();
    shelfCount = 0;

    while(modelRow < (model->rowCount()))
    {
        index = index.sibling(modelRow, 0);
        row = model->data(index).toInt();

        index = index.sibling(modelRow, 1);
        rack = model->data(index).toInt();

        index = index.sibling(modelRow, 3);
        device = model->data(index).toString();

        if( (currentRackRow == row) && (rack == currentRack) )
        {
            shelfCount++;
            deviceList << device;
            modelRow++;
        }
        else
            break;
    }

    rackInfo.insert(shelfCount, deviceList);

    return rackInfo;
}

void ReportFloorPlanBR::drawRack(const int rackRow, const int rackNo, const int shelfCount, const QList<QString> devices)
{
    int shelfHeight = rackHeight;
    if(shelfCount > 0)
        shelfHeight = rackHeight / shelfCount;

    QRect rect(-rackWidth,0, rackWidth, shelfHeight);

    painter->save();
    int odfCOUNT =odfCount(rackRow,rackNo);
    int startX = -(rackNo - SRN - odfCOUNT)*(rackWidth+rackGap) - (odfCOUNT*(odfWidth+rackGap));

    painter->translate(startX, 0);
    for(int i=0; i < shelfCount ; i++)
    {
        painter->fillRect(rect, QColor(230, 250, 255));
        painter->drawRect(rect);
        painter->save();
        painter->setFont(font);
        painter->drawText(rect, Qt::AlignCenter, devices.at(i));
        painter->restore();
        painter->translate(0, shelfHeight);
    }
    rect.setRect(-rackWidth,0,rackWidth,25);
    painter->drawText(rect, Qt::AlignCenter, QString::number(rackNo));
    painter->restore();
}

int ReportFloorPlanBR::odfCount(const int rackRow, const int rackNo)
{
    //row-rack-shelf-device-RO(rack/Odf)-ppp
    QModelIndex ind;
    bool RO;
    int C, R, cnt = 0, rowCount;
    rowCount = model->rowCount();
    for(int r = 0; r < rowCount; r++)
    {
        ind = model->index(r,4);
        RO = model->data(ind).toBool();
        if(RO)
        {
            ind = model->index(r, 0);
            R = model->data(ind).toInt();
            ind  = model->index(r,1);
            C = model->data(ind).toInt();
            if( (R == rackRow) && (C < rackNo) )
                cnt++;
        }
    }

    return cnt;
}

void ReportFloorPlanBR::drawOdf(const int rackRow, const int rackNo, const QString device, const int odfNo, const int ppp)
{
    QRect rect(-odfWidth,0, odfWidth, rackHeight);

    QString dev;
    int len = device.length();
    for(int j=0, i = 0; i < len;i++)
    {
        dev.insert(j,device.at(i));
        j++;
        dev.insert(j,'\n');
        j++;
    }

    painter->save();
    int odfCOUNT = odfCount(rackRow,rackNo);
    int startX = -(rackNo - SRN - odfCOUNT)*(rackWidth+rackGap) - (odfCOUNT*(odfWidth+rackGap));

    painter->translate(startX, 0);

    painter->fillRect(rect, QColor(240, 255, 230));
    painter->drawRect(rect);
    painter->save();
    painter->setFont(font);

    if(odfNo > -1)
        painter->drawText(rect, Qt::AlignCenter,dev+QString::number(odfNo)+"\n("+QString::number(ppp)+")" );
    else
        painter->drawText(rect, Qt::AlignCenter,dev);

    painter->restore();
    painter->translate(0, rackHeight);
    rect.setRect(-odfWidth,0,odfWidth,25);
    painter->drawText(rect, Qt::AlignCenter, QString::number(rackNo));
    painter->restore();
}

void ReportFloorPlanBR::drawRackRow(const int rackRow)
{
    QRect rect(-rackWidth,0, rightMargin, rackHeight);
    painter->save();
    QPen pen(Qt::darkGray);
    painter->setPen(pen);
    painter->setFont(QFont("Tahoma", 16));
    painter->drawText(rect, Qt::AlignCenter, "Row "+QString::number(rackRow));
    painter->restore();
    painter->translate(-rightMargin, 0);
}

QString ReportFloorPlanBR::lastError()
{
    return error;
}
