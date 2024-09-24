#include "RemoveDSLAM.h"
#include <QAbstractItemModel>
#include <QDate>
#include <QStringList>
#include <QPainter>
#include <QPrinter>

RemoveDSLAM::RemoveDSLAM(QObject *Parent, const QString OutputPaththis, const QString Area, const QString Mother, const QString Exch, const QString Dslam, const QString IPm, const QString Vlan, const QStringList Agg1, const QStringList Agg2, const QStringList Cx, const QStringList Bras, const QString Desc) :
    QObject(Parent),
    painter(new QPainter()),
    outputPath(OutputPaththis),
    dslamName(Dslam),
    area(Area),
    mother(Mother),
    exch(Exch),
    ipm(IPm),
    vlan(Vlan),
    agg1(Agg1),
    agg2(Agg2),
    cx(Cx),
    bras(Bras)
{
    headerHeight = 60;

    if(Desc.isEmpty())
        customInfo = "DSLAM جمع آوری می گردد";
    else
        customInfo = Desc;


    bottomHeight = 20;

    pen = QPen(QColor(0, 0, 80), 1, Qt::SolidLine);

    headersFont.setFamily("Family");
    headersFont.setPixelSize(20);
    headersFont.setBold(true);

    contentFont.setFamily("Family");
    contentFont.setBold(true);
    contentFont.setPixelSize(16);

    topicFont.setFamily("Family");
    topicFont.setBold(true);
    topicFont.setPixelSize(16);


    pageNumber = 1;
    current_date = QDate::currentDate().toString("yyyy/MM/dd");
    error = "No error";
}

RemoveDSLAM::~RemoveDSLAM()
{
    delete painter;
    painter = NULL;
}

void RemoveDSLAM::preparePage()
{ // print a border on each page
    painter->resetTransform();
    painter->save();
    painter->setPen(Qt::darkMagenta);
    painter->setFont(QFont("tahoma", 16, QFont::Bold));
    QRect rec(0,0,painter->viewport().width()-50,50);
    painter->drawText(rec,Qt::AlignLeft | Qt::AlignVCenter, current_date);
    painter->drawText(rec,Qt::AlignCenter,"DaNet DSLAM Remove Report");
    QPixmap im(":/Image/tct.jpg");
    painter->drawPixmap(painter->viewport().width()-50, 0,im);
    painter->setPen(Qt::darkMagenta);

    painter->setFont(QFont("tahoma", 10));
    painter->translate(10, painter->viewport().height() );
    painter->setPen(QPen(QColor(0, 0, 0), 2));
    painter->drawText(0, 0, QString("Page %1").arg(pageNumber));
    pageNumber += 1;
    painter->restore();
    painter->translate(10,headerHeight);

}

bool RemoveDSLAM::print()
{
    /*
     *
    dslamName(Dslam),
    area(Area),
    mother(Mother),
    exch(Exch),
    ipg(IPg),
    ipm(IPm),
    vlan(Vlan),
    agg1(Agg1),
    agg2(Agg2),
    cx(Cx),
    bras(Bras)
    */

    //check inputs
    bool d = dslamName.isEmpty();
    bool a = area.isEmpty();
    bool m = mother.isEmpty();
    bool e = exch.isEmpty();
    bool im = ipm.isEmpty();
    bool a1 = (agg1.count() == 10)? false : true;
    bool a2 = (agg2.count() == 6)? false : true;
    bool c =  (cx.count() == 6)? false : true;
    bool b = (bras.count() == 3) ? false : true;

    if(d || a  || m || e || im || a1 || a2 || c || b)
    {
        error = "DSLAM Parameters is not available.";
        return false;
    }
    else
    {
        QPrinter *pr = new QPrinter(QPrinter::ScreenResolution);
        pr->setOutputFormat(QPrinter::PdfFormat);
        pr->setOrientation(QPrinter::Landscape);
        pr->setPaperSize(QPrinter::A3);
        pr->setOutputFileName(outputPath);
        pr->setCreator("DaNet Application");
        pr->setDocName("DSLAM Remove");
        pr->setPrintProgram("DaNet Application");

        if(printTable(pr))
            return true;
        else
            return false;
    }
}

bool RemoveDSLAM::printTable(QPrinter *printer)
{
    if(!painter->begin(printer))
    {
        error = "painter->begin error.";
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
    //----------------------------------------------------------------------------

    painter->setPen(pen);

    preparePage();
    QRect rect(0,0, painter->viewport().width()-20, 40);
    painter->fillRect(rect, QColor(250, 170, 170));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignCenter, "Remove DSLAM "+dslamName);
    painter->translate(0, 40);
    painter->setFont(contentFont);

    ///

    int tableWidth = painter->viewport().width()-20;
    int tableHeight = painter->viewport().height()-bottomHeight-headerHeight-100;
    QList<int> headerWidth;
    int totalWidth;
    headerWidth << 30 << 10 << 20 << 20; //4 columns
    totalWidth = 0;
    foreach (int i, headerWidth)
        totalWidth += i;

    for(int i = 0; i < headerWidth.count(); i++)
    {
        headerWidth[i] = tableWidth * headerWidth[i] / totalWidth;
    }

    double rowHeight;
    int totalRowCount = 21;
    //in total 18
    // bras:2   agg1:2  agg2:2  cx:2

    //bras: bras1<< bras2<<e1,e2
    //agg1: agg << eth << i1 << i2 << i3 << i4 << o1 << o2 << o3 << o4;
    //agg2: agg << eth << i1 << i2 << o1 << o2
    //cx :  cx << eth << i1 << i2 << o1 << o2

    //bras : is 2 bras:
    if(QString(bras[0]).isEmpty())
        totalRowCount--;
    if(QString(bras[1]).isEmpty())
        totalRowCount--;

    if(QString(agg1[2]).isEmpty())
        totalRowCount--;
    if(QString(agg1[3]).isEmpty())
        totalRowCount--;
    if(QString(agg1[4]).isEmpty())
        totalRowCount--;
    if(QString(agg1[5]).isEmpty())
        totalRowCount--;

    if(QString(agg2[2]).isEmpty())
        totalRowCount--;
    if(QString(agg2[3]).isEmpty())
        totalRowCount--;

    if(QString(cx[2]).isEmpty())
        totalRowCount--;
    if(QString(cx[3]).isEmpty())
        totalRowCount--;

    rowHeight = tableHeight / totalRowCount;

    //########################START TABLE

    //Header
    rect.setRect(0,0,tableWidth, 2*rowHeight);
    painter->fillRect(rect, QColor(255, 230, 240));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignCenter, "DSLAM Remove Information");
    painter->translate(0,2*rowHeight);
    painter->setFont(topicFont);
    //AREA
    rect.setRect(0, 0, headerWidth[0], rowHeight);
    painter->fillRect(rect, QColor(200, 220, 255));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Area");

    rect.setRect(headerWidth[0], 0, headerWidth[1]+headerWidth[2]+headerWidth[3], rowHeight);
    painter->fillRect(rect, QColor(220, 255, 220));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+area);

    painter->translate(0, rowHeight);
    //mother
    rect.setRect(0, 0, headerWidth[0], rowHeight);
    painter->fillRect(rect, QColor(200, 220, 255));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Mother Exchange");

    rect.setRect(headerWidth[0], 0, headerWidth[1]+headerWidth[2]+headerWidth[3], rowHeight);
    painter->fillRect(rect, QColor(220, 255, 220));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+mother);

    painter->translate(0, rowHeight);
    //exch/site
    rect.setRect(0, 0, headerWidth[0], rowHeight);
    painter->fillRect(rect, QColor(200, 220, 255));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Exch/Site");

    rect.setRect(headerWidth[0], 0, headerWidth[1]+headerWidth[2]+headerWidth[3], rowHeight);
    painter->fillRect(rect, QColor(220, 255, 220));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+exch);

    painter->translate(0, rowHeight);
    //DSLAM name
    rect.setRect(0, 0, headerWidth[0], rowHeight);
    painter->fillRect(rect, QColor(200, 220, 255));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " DSLAM Name");

    rect.setRect(headerWidth[0], 0, headerWidth[1]+headerWidth[2]+headerWidth[3], rowHeight);
    painter->fillRect(rect, QColor(220, 255, 220));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+dslamName);

    painter->translate(0, rowHeight);
    //DSLAM IP
    rect.setRect(0, 0, headerWidth[0], rowHeight);
    painter->fillRect(rect, QColor(200, 220, 255));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " DSLAM IP");

    rect.setRect(headerWidth[0], 0, headerWidth[1]+headerWidth[2]+headerWidth[3], rowHeight);
    painter->fillRect(rect, QColor(220, 255, 220));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+ipm);

    painter->translate(0, rowHeight);
    //VLANs
    rect.setRect(0, 0, headerWidth[0], 2*rowHeight);
    painter->fillRect(rect, QColor(200, 220, 255));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " VLANs");

    rect.setRect(headerWidth[0], 0, headerWidth[1]+headerWidth[2]+headerWidth[3], 2*rowHeight);
    painter->fillRect(rect, QColor(220, 255, 220));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+vlan);

    painter->translate(0, 2*rowHeight);
    //BRAS
    if(!QString(bras[0]).isEmpty() || !QString(bras[1]).isEmpty())
    {
        if(QString(bras[0]).isEmpty() || QString(bras[1]).isEmpty())
            rect.setRect(0, 0, headerWidth[0], rowHeight);
        else
            rect.setRect(0, 0, headerWidth[0], 2*rowHeight);

        painter->fillRect(rect, QColor(200, 220, 255));
        painter->drawRect(rect);
        painter->setFont(headersFont);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " BRAS");

        rect.setRect(headerWidth[0], 0, headerWidth[1]+headerWidth[2]+headerWidth[3], rowHeight);
        painter->fillRect(rect, QColor(220, 255, 220));
        painter->drawRect(rect);
        painter->setFont(headersFont);
        if(QString(bras[0]).isEmpty())
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+bras[1]);
        else
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+bras[0]);

        if(!QString(bras[0]).isEmpty() && !QString(bras[1]).isEmpty())
        {
            rect.setRect(headerWidth[0], rowHeight, headerWidth[1]+headerWidth[2]+headerWidth[3], rowHeight);
            painter->fillRect(rect, QColor(220, 255, 220));
            painter->drawRect(rect);
            painter->setFont(headersFont);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+bras[1]);

            painter->translate(0, 2*rowHeight);
        }
        else
            painter->translate(0, rowHeight);

        //BRAS eth
        rect.setRect(0, 0, headerWidth[0], rowHeight);
        painter->fillRect(rect, QColor(200, 220, 255));
        painter->drawRect(rect);
        painter->setFont(headersFont);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " BRAS Ether-Trunks To Agg");

        rect.setRect(headerWidth[0], 0, headerWidth[1]+headerWidth[2]+headerWidth[3], rowHeight);
        painter->fillRect(rect, QColor(220, 255, 220));
        painter->drawRect(rect);
        painter->setFont(headersFont);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+bras[2]);

        painter->translate(0, rowHeight);

    }

    //AGG1   //agg1: agg << eth << i1 << i2 << i3 << i4 << o1 << o2 << o3 << o4
    if(!QString(agg1[0]).isEmpty())
    {
        if(!QString(agg1[2]).isEmpty() || !QString(agg1[3]).isEmpty() || !QString(agg1[4]).isEmpty() || !QString(agg1[5]).isEmpty())
        {
            int aggRowHeight = rowHeight;

            if(!QString(agg1[2]).isEmpty() && !QString(agg1[3]).isEmpty() && !QString(agg1[4]).isEmpty() && !QString(agg1[5]).isEmpty())
                aggRowHeight = 4*rowHeight;
            else if(QString(agg1[4]).isEmpty() && QString(agg1[5]).isEmpty())
                aggRowHeight = 2*rowHeight;
            else if(QString(agg1[3]).isEmpty() && QString(agg1[4]).isEmpty() && QString(agg1[5]).isEmpty())
                aggRowHeight = rowHeight;

            rect.setRect(0, 0, headerWidth[0], aggRowHeight);

            painter->fillRect(rect, QColor(200, 220, 255));
            painter->drawRect(rect);
            painter->setFont(headersFont);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+agg1[0]+" To DSLAM");
            //eth
            rect.setRect(headerWidth[0], 0, headerWidth[1], aggRowHeight);

            painter->fillRect(rect, QColor(220, 255, 220));
            painter->drawRect(rect);
            painter->setFont(headersFont);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Eth: "+agg1[1]);
            //int1

            //0agg << 1eth << 2i1 << 3i2 << 4i3 << 5i4 << 6o1 << 7o2 << 8o3 << 9o4
            if(!QString(agg1[2]).isEmpty()) // int1
            {//i1
                rect.setRect(headerWidth[0]+headerWidth[1], 0, headerWidth[2], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " int: "+agg1[2]);

                rect.setRect(headerWidth[0]+headerWidth[1]+headerWidth[2], 0, headerWidth[3], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " ODF: "+agg1[6]);
            }



            if(!QString(agg1[3]).isEmpty()) // int2
            {
                //int2
                rect.setRect(headerWidth[0]+headerWidth[1], rowHeight, headerWidth[2], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " int: "+agg1[3]);

                rect.setRect(headerWidth[0]+headerWidth[1]+headerWidth[2], rowHeight, headerWidth[3], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " ODF: "+agg1[7]);
            }

            if(!QString(agg1[4]).isEmpty()) // int 3
            {
                //int3
                rect.setRect(headerWidth[0]+headerWidth[1], 2*rowHeight, headerWidth[2], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " int: "+agg1[4]);

                rect.setRect(headerWidth[0]+headerWidth[1]+headerWidth[2], 2*rowHeight, headerWidth[3], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " ODF: "+agg1[8]);
            }

            if(!QString(agg1[5]).isEmpty()) // int 4
            {
                //int3
                rect.setRect(headerWidth[0]+headerWidth[1], 3*rowHeight, headerWidth[2], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " int: "+agg1[5]);

                rect.setRect(headerWidth[0]+headerWidth[1]+headerWidth[2], 3*rowHeight, headerWidth[3], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " ODF: "+agg1[9]);
            }

            painter->translate(0, aggRowHeight);
        }

    }

    //AGG2
    if(!QString(agg2[0]).isEmpty())
    {
        if(!(QString(agg2[2]).isEmpty() && QString(agg2[3]).isEmpty()))
        {
            if(QString(agg2[2]).isEmpty() || QString(agg2[3]).isEmpty())
                rect.setRect(0, 0, headerWidth[0], rowHeight);
            else
                rect.setRect(0, 0, headerWidth[0], 2*rowHeight);

            painter->fillRect(rect, QColor(200, 220, 255));
            painter->drawRect(rect);
            painter->setFont(headersFont);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+agg2[0]+" To DSLAM");
            //eth
            if(QString(agg2[2]).isEmpty() || QString(agg2[3]).isEmpty())
                rect.setRect(headerWidth[0], 0, headerWidth[1], rowHeight);
            else
                rect.setRect(headerWidth[0], 0, headerWidth[1], 2*rowHeight);
            painter->fillRect(rect, QColor(220, 255, 220));
            painter->drawRect(rect);
            painter->setFont(headersFont);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Eth: "+agg2[1]);
            //int1

            if(!QString(agg2[2]).isEmpty())
            {//i1
                rect.setRect(headerWidth[0]+headerWidth[1], 0, headerWidth[2], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " int: "+agg2[2]);

                rect.setRect(headerWidth[0]+headerWidth[1]+headerWidth[2], 0, headerWidth[3], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " ODF: "+agg2[4]);
            }
            else
            {
                //int2
                rect.setRect(headerWidth[0]+headerWidth[1], 0, headerWidth[2], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " int: "+agg2[3]);

                rect.setRect(headerWidth[0]+headerWidth[1]+headerWidth[2], 0, headerWidth[3], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " ODF: "+agg2[5]);
            }

            if(!QString(agg2[2]).isEmpty() && !QString(agg2[3]).isEmpty())
            {
                //int2
                rect.setRect(headerWidth[0]+headerWidth[1], rowHeight, headerWidth[2], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " int: "+agg2[3]);

                rect.setRect(headerWidth[0]+headerWidth[1]+headerWidth[2], rowHeight, headerWidth[3], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " ODF: "+agg2[5]);

                painter->translate(0, 2*rowHeight);

            }
            else
                painter->translate(0, rowHeight);
        }
    }

    //CX
    if(!QString(cx[0]).isEmpty())
    {
        if(!QString(cx[2]).isEmpty() || !QString(cx[3]).isEmpty())
        {
            if(QString(cx[2]).isEmpty() || QString(cx[3]).isEmpty())
                rect.setRect(0, 0, headerWidth[0], rowHeight);
            else
                rect.setRect(0, 0, headerWidth[0], 2*rowHeight);

            painter->fillRect(rect, QColor(200, 220, 255));
            painter->drawRect(rect);
            painter->setFont(headersFont);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+cx[0]+" To DSLAM");
            //eth
            if(QString(cx[2]).isEmpty() || QString(cx[3]).isEmpty())
                rect.setRect(headerWidth[0], 0, headerWidth[1], rowHeight);
            else
                rect.setRect(headerWidth[0], 0, headerWidth[1], 2*rowHeight);
            painter->fillRect(rect, QColor(220, 255, 220));
            painter->drawRect(rect);
            painter->setFont(headersFont);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Eth: "+cx[1]);
            //int1

            if(!QString(cx[2]).isEmpty())
            {//i1
                rect.setRect(headerWidth[0]+headerWidth[1], 0, headerWidth[2], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " int: "+cx[2]);

                rect.setRect(headerWidth[0]+headerWidth[1]+headerWidth[2], 0, headerWidth[3], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " ODF: "+cx[4]);
            }
            else
            {
                //int2
                rect.setRect(headerWidth[0]+headerWidth[1], 0, headerWidth[2], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " int: "+cx[3]);

                rect.setRect(headerWidth[0]+headerWidth[1]+headerWidth[2], 0, headerWidth[3], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " ODF: "+cx[5]);
            }

            if(!QString(cx[2]).isEmpty() && !QString(cx[3]).isEmpty())
            {
                //int2
                rect.setRect(headerWidth[0]+headerWidth[1], rowHeight, headerWidth[2], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " int: "+cx[3]);

                rect.setRect(headerWidth[0]+headerWidth[1]+headerWidth[2], rowHeight, headerWidth[3], rowHeight);
                painter->fillRect(rect, QColor(220, 255, 220));
                painter->drawRect(rect);
                painter->setFont(headersFont);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " ODF: "+cx[5]);

                painter->translate(0, 2*rowHeight);

            }
            else
                painter->translate(0, rowHeight);
        }
    }

    //Desc 3
    rect.setRect(0, 0, headerWidth[0], 3*rowHeight);
    painter->fillRect(rect, QColor(200, 220, 255));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Description");

    rect.setRect(headerWidth[0], 0, headerWidth[1]+headerWidth[2]+headerWidth[3], 3*rowHeight);
    painter->fillRect(rect, QColor(220, 255, 220));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+customInfo);

    //
    painter->end();

    return true;
}

QString RemoveDSLAM::lastError()
{
    return error;
}
