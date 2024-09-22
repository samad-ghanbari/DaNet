#include "ReportOdf.h"
#include <QAbstractItemModel>
#include <QDate>
#include <QPainter>
#include <QPrinter>
#include <QFontDatabase>

ReportOdf::ReportOdf(QObject *parent, const QAbstractItemModel* tableModel, const QString OutputPath, const QString CustomInfo) :
    QObject(parent),
    painter(new QPainter()),
    model(tableModel),
    outputPath(OutputPath)
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

    init();
    highlightFlag = false;

    QFontDatabase::addApplicationFont(":/font/tahoma.ttf");
    QFontDatabase::addApplicationFont(":/font/tahomabd.ttf");
}

ReportOdf::ReportOdf(QObject *parent, const QAbstractItemModel *tableModel, const QList<int> HighlightPinIds, const QString OutputPath, const QString CustomInfo) :
    QObject(parent),
    painter(new QPainter()),
    model(tableModel),
    outputPath(OutputPath)

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

    init();
    highlightPinIds = HighlightPinIds;
    highlightFlag = true;
}

ReportOdf::~ReportOdf()
{
    delete painter;
    painter = NULL;
}

void ReportOdf::init()
{
    topMargin = 5;
    bottomMargin = 5;
    leftMargin = 5;
    rightMargin = 5;
    if(CUSTOM_INFO)
        headerHeight = 130;
    else
        headerHeight = 100;
    bottomHeight = 20;
    leftBlank = 0;
    rightBlank = 0;
    maxRowHeight = 50;
    tableHeaderHight = 40;
    pen = QPen(QColor(0, 0, 80), 1, Qt::SolidLine); //pen for borders
//    pen.setWidth(2);
    QFont font1; // font for headers
    font1.setPixelSize(14);
    font1.setBold(true);
    font1.setFamily("tahoma");
    headersFont = font1;
    QFont font2; // font for content
    font2.setPixelSize(14);
    font2.setFamily("tahoma");
    contentFont = font2;
    contentFont.setFamily("tahoma");
    headerColor = QColor(Qt::darkRed);
    contentColor = QColor(Qt::darkBlue);

    //0`odf_id`, 1`pos_id`, 2`pin_id`, 3`exchange_id`, 4`saloon_no`,
    //5`area`, 6`exchange`, 7`abbr`,8`saloon_name`, 9`pin_per_pos`,
    //10`odf_no`, 11`pos_no`, 12`local_device`, 13`local_label`,14`pin_no`
    //15`remote`, 16`remote_label`


    columnStretch = QVector<int>() <<0<<0<<0<<0<<0
                                   <<0<<0<<0<<0<<0
                                   <<0<<0<<15<<30<<10
                                   <<15<<30;

    headers = QVector<QString>() <<"odfId"<<"posId"<<"pinId"<<"exchangeId"<<"saloonNo"
                                 <<"area"<<"exchange"<<"abbr"<<"saloon"<<"ppp"
                                 <<"odf"<<"pos"<<"Local Device"<<"Local Label"<<"PIN No"
                                 <<"Remote"<<"Remote Label";

    pageNumber = 1;
    currentPosId = model->data(model->index(0,1)).toInt();
    int area = model->data(model->index(0,5)).toInt();
    exchange = model->data(model->index(0,6)).toString();
    QString abbr = model->data(model->index(0,7)).toString();
    saloon = model->data(model->index(0,8)).toString();
    ppp = model->data(model->index(0,9)).toInt();
    odfNo = model->data(model->index(0,10)).toInt();
    posNo = model->data(model->index(0,11)).toInt();

    headerAbbr = QString::number(area)+"-"+abbr;
    currentDate = QDate::currentDate().toString("yyyy/MM/dd");

    error = "No error";
}

void ReportOdf::preparePage(QPainter *painter)
{ // print a border on each page

    painter->setPen(Qt::darkMagenta);
    painter->setFont(QFont("tahoma", 14, QFont::Bold));
    QRect rec(10,10,painter->viewport().width()-100,30);
    painter->drawText(rec,Qt::AlignLeft, currentDate);
    painter->drawText(rec,Qt::AlignCenter,"DaNet ODF Report");
    rec.setRect(10,40,painter->viewport().width()-100,30);
    painter->drawText(rec, Qt::AlignLeft,headerAbbr+"- Saloon: "+saloon);
    painter->drawText(rec, Qt::AlignRight,exchange);
    rec.setRect(10,70,painter->viewport().width()-100,30);
//    painter->setFont(QFont("tahoma", 14, QFont::Bold));
//    painter->setPen(QPen(QColor(0, 0, 80), 2));
    painter->setPen(Qt::blue);
    painter->drawText(rec, Qt::AlignLeft,"ODF NO: "+QString::number(odfNo)+" - "+"POS NO: "+QString::number(posNo));
    painter->setPen(Qt::darkMagenta);
    painter->drawText(rec, Qt::AlignRight,"ODF PIN-per-POS: "+QString::number(ppp));
    QPixmap im(":/Image/tct.jpg");
    painter->drawPixmap(painter->viewport().width()-50, 25,im);

    if(CUSTOM_INFO)
    {
        rec.setRect(10,100,painter->viewport().width()-20,30);
        painter->setPen(Qt::darkCyan);
        painter->drawText(rec, Qt::AlignLeft,customInfo);
        painter->setPen(Qt::darkMagenta);
    }

    painter->setFont(QFont("tahoma", 10));
    painter->translate(10, painter->viewport().height()-10 );
    painter->setPen(QPen(QColor(0, 0, 0), 2));
    painter->drawText(0, 0, QString("Page %1").arg(pageNumber));
    pageNumber += 1;
}

bool ReportOdf::print()
{
    QPrinter *pr = new QPrinter(QPrinter::ScreenResolution);
    pr->setOutputFormat(QPrinter::PdfFormat);
    pr->setOrientation(QPrinter::Portrait);
    pr->setPaperSize(QPrinter::A3);
    pr->setOutputFileName(outputPath);
    pr->setCreator("DaNet Application");
    pr->setDocName("ODF Report");
    pr->setPrintProgram("DaNet Application");

    if(printTable(pr))
        return true;
    else
    {
        error = "can not print tables";
        return false;
    }
}

bool ReportOdf::printTable(QPrinter *p)
{
    QPrinter *printer = p;

    if(!painter->begin(printer))
    {
        error = "painter can't start printer.";
        return false;
    }
    painter->translate(0, headerHeight); // start print point


    //--------------------------------- error checking -------------------------------------

    int columnCount = model->columnCount();
    int count = columnStretch.count();
    if(count != columnCount)
    {
        error = "Different columns count in model and in columnStretch";
        return false;
    }
    count = headers.count();
    if(count != columnCount && count != 0)
    {
        error = "Different columns count in model and in headers";
        return false;
    }
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
    double tableWidth = painter->viewport().width() - leftBlank - rightBlank;
    if(tableWidth <= 0)
    {
        error = "wrong table width";
        return false;
    }
    int totalStretch = 0;
    for (int i = 0; i < columnStretch.count(); i++)
    {
        if(columnStretch[i] < 0)
        {
            error = QString("wrong column stretch, columnt: %1 stretch: %2").arg(i).arg(columnStretch[i]);
            return false;
        }
        totalStretch += columnStretch[i];
    }
    if(totalStretch <= 0)
    {
        error = QString("wrong stretch");
        return false;
    }
    QVector<double> columnWidth;
    for (int i = 0; i < columnStretch.count(); i++)
    {
        columnWidth.append(tableWidth / totalStretch * columnStretch[i]);
    }


    headers.isEmpty() ? newPageFlag = false : newPageFlag = true;

   //----------------------------------------------------------------------------

    painter->save(); // before table print

    // to know row height before printing
    // at first print to test image
    QPainter testSize;
    QImage* image = new QImage(10, 10, QImage::Format_RGB32);
    image->setDotsPerMeterX(printer->logicalDpiX() * 100 / 2.54); // 2.54 cm = 1 inch   dot/inch = dot/2.54cm = dot*100/2.54m=100/2.54 dot/m
    image->setDotsPerMeterY(printer->logicalDpiY() * 100 / 2.54);
    testSize.begin(image);

    painter->save(); //push setting
    painter->translate(-painter->transform().dx(), -painter->transform().dy());
    preparePage(painter);
    painter->restore(); // pop setting

    painter->setPen(pen);
    painter->setFont(contentFont);
    testSize.setFont(contentFont);
    painter->translate(-painter->transform().dx() + leftBlank, 0);
    painter->save();

    painter->drawLine(0, 0, tableWidth, 0); // first horizontal line

    int j =0;
    int posId, pinId;
    while(j < model->rowCount())
    { // for each row


        pinId = -1;
        doHighlight = false;
        posId = model->data(model->index(j,1)).toInt();

        if(highlightFlag )
        {
            pinId = model->data(model->index(j,2)).toInt();
            if(highlightPinIds.contains(pinId))
            {
                doHighlight = true;
            }
            else
            {
                doHighlight = false;
            }
        }



        if(currentPosId != posId)
        {
            currentPosId = posId;
            int area = model->data(model->index(j,5)).toInt();
            exchange = model->data(model->index(j,6)).toString();
            QString abbr = model->data(model->index(j,7)).toString();
            saloon = model->data(model->index(j,8)).toString();
            ppp = model->data(model->index(j,9)).toInt();
            odfNo = model->data(model->index(j,10)).toInt();
            posNo = model->data(model->index(j,11)).toInt();

            headerAbbr = QString::number(area)+"-"+abbr;

            int y = painter->transform().dy();
            painter->restore();
            painter->save();
            for(int i = 0; i < columnCount; i++)
            { // vertical lines
                painter->drawLine(0, 0, 0, - painter->transform().dy() + y);
                painter->translate(columnWidth[i], 0);
            }
            painter->drawLine(0, 0, 0, - painter->transform().dy() + y); // last vertical line
            painter->restore();

            printer->newPage();
            newPageFlag = true;

            painter->save();
            painter->translate(-painter->transform().dx(), -painter->transform().dy());
            preparePage(painter);
            painter->restore();

            painter->translate(-painter->transform().dx() + leftBlank, -painter->transform().dy() + headerHeight);
            painter->save();
            painter->drawLine(0, 0, tableWidth, 0); // first horizontal line

        }


        if(newPageFlag)
        {
            painter->setFont(headersFont);
            testSize.setFont(headersFont);
        }
        else
        {
            painter->setFont(contentFont);
            testSize.setFont(contentFont);
        }


        // --------------------------- row height counting ----------------------------

        int maxHeight = 0; // max row Height
        for(int i = 0; i < columnCount; i++)
        { // for each column
            QString str;
            if(newPageFlag)
            {
                str = headers.at(i);
                maxHeight = tableHeaderHight;
            } else
            {
                str = model->data(model->index(j,i), Qt::DisplayRole).toString();

                QRect rect(0, 0, columnWidth[i] - rightMargin - leftMargin, maxRowHeight);
                QRect realRect;
                testSize.drawText(rect, Qt::AlignLeft | Qt::TextWordWrap, str, &realRect);

                if (realRect.height() > maxHeight && columnStretch[i] != 0)
                {
                    realRect.height() > maxRowHeight ? maxHeight = maxRowHeight : maxHeight = realRect.height();
                }

            }
        }

        if( (painter->transform().dy() + maxHeight + topMargin + bottomMargin) > (painter->viewport().height() - bottomHeight) )
        { // begin from new page

            currentPosId = model->data(model->index(j,1)).toInt();
            int area = model->data(model->index(j,5)).toInt();
            exchange = model->data(model->index(j,6)).toString();
            QString abbr = model->data(model->index(j,7)).toString();
            saloon = model->data(model->index(j,8)).toString();
            ppp = model->data(model->index(j,9)).toInt();
            odfNo = model->data(model->index(j,10)).toInt();
            posNo = model->data(model->index(j,11)).toInt();

            headerAbbr = QString::number(area)+"-"+abbr;

            int y = painter->transform().dy();
            painter->restore();
            painter->save();
            for(int i = 0; i < columnCount; i++)
            { // vertical lines
                painter->drawLine(0, 0, 0, - painter->transform().dy() + y);
                painter->translate(columnWidth[i], 0);
            }
            painter->drawLine(0, 0, 0, - painter->transform().dy() + y); // last vertical line
            painter->restore();
            printer->newPage();
            newPageFlag = true;

            painter->save();
            painter->translate(-painter->transform().dx(), -painter->transform().dy());
            preparePage(painter);
            painter->restore();

            painter->translate(-painter->transform().dx() + leftBlank, -painter->transform().dy() + headerHeight);
            painter->save();
            painter->drawLine(0, 0, tableWidth, 0); // first horizontal line
        }

        //------------------------------ content printing -------------------------------------------

        painter->save();
        if(newPageFlag)
        {
            painter->setFont(headersFont);
            testSize.setFont(headersFont);
            painter->setPen(QPen(headerColor));
        }
        else
        {
            painter->setFont(contentFont);
            testSize.setFont(contentFont);
            painter->setPen(QPen(contentColor));
        }
        for(int i = 0; i < columnCount; i++)
        { // for each column
            QString str;
            if(newPageFlag)
            {
                str = headers.at(i);
//                maxHeight = tableHeaderHight;
                QRect rec(leftMargin, topMargin, columnWidth[i] - rightMargin - leftMargin, tableHeaderHight);
                QRect fillRec(0, 0, columnWidth[i] , tableHeaderHight+topMargin+bottomMargin);
                painter->fillRect(fillRec, QColor(250, 240, 230));
                painter->drawText(rec, Qt::AlignCenter | Qt::TextWordWrap, str);
            } else
            {
                str = model->data(model->index(j,i), Qt::DisplayRole).toString();
                QRect rec(leftMargin, topMargin, columnWidth[i] - rightMargin - leftMargin, maxHeight);
                //highlight rec
                if(doHighlight && (highlightPinIds.length() < 50) )
                {
                    QRect tempRect(0,0,columnWidth[i], maxHeight+topMargin+bottomMargin);
                    painter->fillRect(tempRect, Qt::yellow);
                }
                if(i == 14) // pin no
                    {
                        painter->save();
                        painter->setPen(Qt::black);
                        painter->setFont(QFont("tahoma", 12, QFont::Bold));
                        painter->drawText(rec, Qt::AlignCenter | Qt::AlignVCenter | Qt::TextWordWrap, str);
                        painter->restore();

                    }
                else
                {
                    //change color in case of highlighting
                    if(doHighlight && (highlightPinIds.length() < 50) )
                    {
                        painter->save();
                        painter->setPen(Qt::black);
                        painter->drawText(rec, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, str);
                        painter->restore();
                    }
                    else
                        painter->drawText(rec, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, str);
                }
            }

            painter->translate(columnWidth[i], 0);
        }
        painter->restore();

        if(newPageFlag)
        {
            painter->drawLine(0, tableHeaderHight + topMargin + bottomMargin, tableWidth, tableHeaderHight + topMargin + bottomMargin); // last horizontal line
            painter->translate(0, tableHeaderHight + topMargin + bottomMargin);
        }else
        {
            painter->drawLine(0, maxHeight + topMargin + bottomMargin, tableWidth, maxHeight + topMargin + bottomMargin); // last horizontal line
            painter->translate(0, maxHeight + topMargin + bottomMargin);
        }


        if(newPageFlag)
            newPageFlag = false;
        else
            j++;

    }
    int y = painter->transform().dy();
    painter->restore();
    painter->save();
    for(int i = 0; i < columnCount; i++)
    { // vertical lines
        painter->drawLine(0, 0, 0, - painter->transform().dy() + y);
        painter->translate(columnWidth[i], 0);
    }
    painter->drawLine(0, 0, 0, - painter->transform().dy() + y); // last vertical line
    painter->restore();

    testSize.end();
    delete image;

    painter->restore(); // before table print

    painter->end();

    return true;
}

QString ReportOdf::lastError()
{
    return error;
}
