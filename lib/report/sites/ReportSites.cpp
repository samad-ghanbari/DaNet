#include "ReportSites.h"
#include <QAbstractItemModel>
#include <QDate>
#include <QPainter>

ReportSites::ReportSites(QObject *parent, QAbstractItemModel *Model, const QString OutputPath, const QString MotherAbbr, const QString Mother, const int PaperSize, const int Compressfactor, const int Orient, const QFont SiteNameFont, const QString CustomInfo) :
    QObject(parent),
    painter(new QPainter()),
    RemoveSiteFlag(false),
    model(Model),
    outputPath(OutputPath),
    mother(Mother),
    motherAbbr(MotherAbbr)

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
    leftMargin = 60;
    rightMargin = 0;

    siteRectWidth = 100;

    pen = QPen(QColor(0, 0, 80), 1, Qt::SolidLine);
    siteNameFont = SiteNameFont;

    pageNumber = 1;

    currentDate = QDate::currentDate().toString("yyyy/MM/dd");
    currentCascade = 1;
    error = "No error";

    compressFactor = Compressfactor/100.0;

    //fill lineposMap to where the line should draw - 1 subsite > (x,y) ...
    linePosMap.clear();
    linePosMap.insert(0, QList<int>() <<0<<0);
    linePosMap.insert(1, QList<int>() <<(50*compressFactor)<<0);
    linePosMap.insert(2, QList<int>() <<(50*compressFactor)<<(-300*compressFactor) << (50*compressFactor) << (300*compressFactor));
    linePosMap.insert(3, QList<int>() <<(50*compressFactor)<<(-300*compressFactor) << (50*compressFactor) << 0 << (50*compressFactor) << (300*compressFactor));
    linePosMap.insert(4, QList<int>() <<(50*compressFactor)<<(-300*compressFactor) << (50*compressFactor) <<(-150*compressFactor)<<(50*compressFactor)<<(150*compressFactor)<<(50*compressFactor)<<(300*compressFactor));
    linePosMap.insert(5, QList<int>() <<(50*compressFactor)<<(-300*compressFactor) << (50*compressFactor) <<(-150*compressFactor)<<(50*compressFactor)<<0<<(50*compressFactor)<<(150*compressFactor)<<(50*compressFactor)<<(300*compressFactor));


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

ReportSites::ReportSites(QObject *parent, QAbstractItemModel *Model, const QString OutputPath, const QString MotherAbbr, const QString Mother, const int PaperSize, const int Compressfactor, const int Orient, const QFont SiteNameFont, const QList<int> RemoveSiteCN, const QString CustomInfo):
    QObject(parent),
    painter(new QPainter()),
    RemoveSiteFlag(true),
    model(Model),
    outputPath(OutputPath),
    mother(Mother),
    motherAbbr(MotherAbbr)
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

    removeCascade = RemoveSiteCN[0];
    removeNode = RemoveSiteCN[1];

    bottomHeight = 40;
    leftMargin = 60;
    rightMargin = 0;

    siteRectWidth = 100;

    pen = QPen(QColor(0, 0, 80), 1, Qt::SolidLine);
    siteNameFont = SiteNameFont;

    pageNumber = 1;

    currentDate = QDate::currentDate().toString("yyyy/MM/dd");
    currentCascade = 1;
    error = "No error";

    compressFactor = Compressfactor/100.0;

    //fill lineposMap to where the line should draw - 1 subsite > (x,y) ...
    linePosMap.clear();
    linePosMap.insert(0, QList<int>() <<0<<0);
    linePosMap.insert(1, QList<int>() <<(50*compressFactor)<<0);
    linePosMap.insert(2, QList<int>() <<(50*compressFactor)<<(-300*compressFactor) << (50*compressFactor) << (300*compressFactor));
    linePosMap.insert(3, QList<int>() <<(50*compressFactor)<<(-300*compressFactor) << (50*compressFactor) << 0 << (50*compressFactor) << (300*compressFactor));
    linePosMap.insert(4, QList<int>() <<(50*compressFactor)<<(-300*compressFactor) << (50*compressFactor) <<(-150*compressFactor)<<(50*compressFactor)<<(150*compressFactor)<<(50*compressFactor)<<(300*compressFactor));
    linePosMap.insert(5, QList<int>() <<(50*compressFactor)<<(-300*compressFactor) << (50*compressFactor) <<(-150*compressFactor)<<(50*compressFactor)<<0<<(50*compressFactor)<<(150*compressFactor)<<(50*compressFactor)<<(300*compressFactor));


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


ReportSites::~ReportSites()
{
    delete painter;
    painter = NULL;
}

void ReportSites::preparePage(QPainter *painter)
{
    sitePosMap.clear();
    subSiteMap.clear();


    painter->resetTransform();

    painter->save();

    painter->setPen(Qt::darkMagenta);
    painter->setFont(QFont("tahoma", 14, QFont::Bold));
    QRect rec(10,10,painter->viewport().width()-100,30);
    painter->drawText(rec,Qt::AlignLeft, currentDate);
    painter->drawText(rec,Qt::AlignCenter,"DaNet Exchange Sites Report");
    rec.setRect(10,40,painter->viewport().width()-100,30);
    painter->drawText(rec, Qt::AlignLeft,motherAbbr+"- Cascade: "+QString::number(currentCascade));
    painter->drawText(rec, Qt::AlignCenter,mother);
    QPixmap im(":/Image/tct.jpg");
    painter->drawPixmap(painter->viewport().width()-50, 15,im);

    if(CUSTOM_INFO)
    {
        rec.setRect(10,70,painter->viewport().width()-20,30);
        painter->setPen(Qt::darkCyan);
        painter->drawText(rec, Qt::AlignLeft,customInfo);
        painter->setPen(Qt::darkMagenta);

        QPen penTemp = QPen(Qt::magenta, 1, Qt::SolidLine);
        painter->setPen(penTemp);
        painter->drawLine(0, 110,painter->viewport().width(), 110);
        painter->setPen(Qt::darkMagenta);
    }
    else
    {
        QPen penTemp = QPen(Qt::magenta, 1, Qt::SolidLine);
        painter->setPen(penTemp);
        painter->drawLine(0, 75,painter->viewport().width(), 75);
        painter->setPen(Qt::darkMagenta);
    }

    rec.setRect(5,headerHeight,55,exchHeight);
    painter->drawRect(rec);
    painter->fillRect(rec, QColor(250, 240, 230));
    painter->setFont(QFont("tahoma", 24, QFont::Bold));

    QString mth = stringToVerticalString(motherAbbr);
    painter->drawText(rec, Qt::AlignCenter, mth);



//    painter->save();
//    painter->translate(40,exchHeight/2+70);
//    painter->rotate(-90);
//    painter->drawText(0,0, motherAbbr);
//    painter->restore();

    //page number
    painter->setFont(QFont("tahoma", 10));
    painter->translate(10, painter->viewport().height()-10 );
    painter->setPen(QPen(QColor(0, 0, 0), 2));
    painter->drawText(0, 0, QString("Page %1").arg(pageNumber));
    pageNumber += 1;

    painter->restore();


    painter->translate(leftMargin, exchHeight/2 + headerHeight); // start print point

    fillSubSiteMap(currentCascade);
}

bool ReportSites::print()
{
    if(model->rowCount() == 0)
    {
        error = "No cascade is available.";
        return false;
    }

    QPrinter *printer = new QPrinter(QPrinter::ScreenResolution);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOrientation(orient);
    printer->setPaperSize(paperSize);
    printer->setOutputFileName(outputPath);
    printer->setCreator("DaNet Application");
    printer->setDocName("Site Report");
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

    exchHeight = painter->viewport().height() - bottomHeight - headerHeight; //exchange box height

    pen.setColor(Qt::darkBlue);
    painter->setPen(pen);



    QModelIndex index = model->index(0,9);
    currentCascade = model->data(index).toInt();

    preparePage(painter);


    //0`id`, 1`area`, 2`exchange`, 3`abbr`, 4`type`, 5`mother_exchange_id`, 6`mother_exchange_abbr`,
    //7`uplink_exchange_id`, 8`uplink_exchange_abbr`, 9`site_cascade`, 10`site_node`, 11`address`

    int siteId, cascade,node, motherId, uplinkId, subSiteCount,x0, x,y0, y;
    QString abbr, siteName, Dev, Ip,Pppoe;
    QList<int> intTemp;
    for(int row = 0; row < model->rowCount(); row++)
    {
        index = index.sibling(row,0);
        siteId = model->data(index).toInt();

        index = index.sibling(row,2);
        siteName = model->data(index).toString();

        index = index.sibling(row,3);
        abbr = model->data(index).toString();

        index = index.sibling(row,5);
        motherId = model->data(index).toInt();

        index = index.sibling(row,7);
        uplinkId = model->data(index).toInt();

        index = index.sibling(row,9);
        cascade = model->data(index).toInt();

        index = index.sibling(row,10);
        node = model->data(index).toInt();

        index = index.sibling(row,12);
        Dev = model->data(index).toString();

        index = index.sibling(row,13);
        Ip = model->data(index).toString();

        index = index.sibling(row,14);
        Pppoe = model->data(index).toString();

        if(currentCascade != cascade)
        {
            currentCascade = cascade;
            printer->newPage();
            preparePage(painter);
        }


        intTemp.clear();
        intTemp << painter->transform().dx() << painter->transform().dy();
        sitePosMap.insert(siteId,intTemp);

        if(uplinkId == motherId) //head of cascade
        {
            painter->drawLine(0,0,100,0);
            painter->translate(100, 0);
        }
        else
        {
            subSiteCount = subSiteMap.value(uplinkId);
            int s = getSubSiteCount(uplinkId);
            intTemp = linePosMap.value(s);
            subSiteMap.remove(uplinkId);
            subSiteMap.insert(uplinkId, subSiteCount-1);

            if(intTemp.count() < 2)
            {
                error = "error in line positioning.";
                return false;
            }

            x = intTemp.at(subSiteCount*2-2);
            y = intTemp.at(subSiteCount*2-1);

            intTemp = sitePosMap.value(uplinkId);
            if(intTemp.count() < 2)
            {
                error = "error in site positioning.";
                return false;
            }
            x0 = intTemp.at(0);
            y0 = intTemp.at(1);

            painter->resetTransform();
            painter->translate(x0, y0);
            painter->drawLine(0,0,x,y);
            painter->translate(x, y);
        }

        if(RemoveSiteFlag)
        {
            if((removeCascade == cascade) && (removeNode == node) )
                drawSite(painter, abbr, siteName, Dev, Ip, Pppoe, true);
            else
                drawSite(painter, abbr, siteName, Dev, Ip, Pppoe);
        }
        else
            drawSite(painter, abbr, siteName, Dev, Ip, Pppoe);
        painter->translate(100,0);

        intTemp.clear();
        intTemp << painter->transform().dx() << painter->transform().dy();
        sitePosMap.insert(siteId,intTemp);

    }


    painter->end();
    return true;

}

void ReportSites::drawSite(QPainter *painter, const QString abbr, const QString siteName, const QString dev, const QString ip, const QString pppoe, const bool removeFlag)
{
    QRect rect(0,-siteRectWidth/2,siteRectWidth,siteRectWidth);
    QRect devRect(0,-siteRectWidth,siteRectWidth, siteRectWidth/2);
    QRect ipPppoerect(0,siteRectWidth/2, siteRectWidth, siteRectWidth);
    painter->fillRect(rect, QColor(255, 250, 255));
    painter->drawRect(rect);
    QString str;
    if(abbr.compare(siteName, Qt::CaseInsensitive) == 0)
        str = abbr;
    else
        str = abbr+"\n"+siteName;
    painter->save();
    painter->setFont(siteNameFont);
    painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap , str);
    painter->setPen(QColor(0, 70, 60));
    painter->drawText(devRect, Qt::AlignBottom | Qt::AlignHCenter | Qt::TextWordWrap , dev);
    QFont f;
    f.setPointSize(8);
    painter->setFont(f);
    painter->setPen(QColor(100, 10, 10));
    if(!pppoe.isEmpty())
        painter->drawText(ipPppoerect,Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap , ip+"\nPPPOE: "+pppoe);

    if(removeFlag)
    {
        //draw cross
        painter->setPen(QColor(200, 10, 10));
        painter->drawLine(-10,-10-siteRectWidth/2,siteRectWidth+10,siteRectWidth/2+10);
        painter->drawLine(-10,10+siteRectWidth/2,siteRectWidth+10,-siteRectWidth/2-10);
    }

    painter->restore();
}

int ReportSites::getSubSiteCount(int uplinkId)
{
    QModelIndex ix;
    int u,cnt = 0;
    for(int i = 0; i < model->rowCount(); i++)
    {
        ix = model->index(i,7);
        u = model->data(ix).toInt();
        if(u == uplinkId) cnt++;
    }

    return cnt;

}

void ReportSites::fillSubSiteMap(const int cascade)
{
    subSiteMap.clear();
    int id, cnt, cas;
    QModelIndex index;
    for(int i = 0; i < model->rowCount(); i++)
    {
        index = model->index(i,9);
        cas = model->data(index).toInt();
        if(cas == cascade)
        {
            index = model->index(i,0);
            id = model->data(index).toInt();
            cnt = getSubSiteCount(id);

            subSiteMap.insert(id, cnt);
        }

    }
}

int ReportSites::getNode(const int uplinkId)
{
    int n = -1, u;
    QModelIndex index;
    for(int i = 0; i < model->rowCount(); i++)
    {
        index = model->index(i, 7);
        u = model->data(index).toInt();
        if(u == uplinkId)
        {
            index = index.sibling(index.row(), 10);
            n = model->data(index).toInt();
        }
    }
    return n;
}

QString ReportSites::lastError()
{
    return error;
}

QString ReportSites::stringToVerticalString(QString string)
{
    QString res = "";
    QChar ch;
    int len = string.length();
    for(int i = 0; i < len; i++)
    {
        ch = string.at(i);
        if( (i != 0) || (i != len-1) )
            res +='\n'+ch;
        else
            res += ch;
    }

    return res;
}
