#include "ReportDSLAM.h"
#include <QAbstractItemModel>
#include <QDate>
#include <QStringList>
#include <QPainter>
#include <QPrinter>

ReportDSLAM::ReportDSLAM(QObject *Parent, const QString OutputPath, const bool HuaweiDslam , bool SingleAgg, const QString DslamName, const bool InterSwitchFlag, const QMap<int, QString> ServiceMap, const QMap<int, QStringList> NmsMap, QMap<QString, QString> PortMap, QStringList Agg1List, QStringList Agg2List, QStringList AggInfo, QStringList Bras1List, QStringList Bras2List, QMap<int, QString> BrasInfo, QStringList CxList,  const QString CustomInfo) : //QStringList Agg1CxList,
    QObject(Parent),
    painter(new QPainter()),
    outputPath(OutputPath),
    dslamName(DslamName),
    interSwitchFlag(InterSwitchFlag),
    serviceMap(ServiceMap),
    portMap(PortMap),
    nmsMap(NmsMap),
    agg1List(Agg1List),
    agg2List(Agg2List),
    aggInfo(AggInfo),
    bras1List(Bras1List),
    bras2List(Bras2List),
    brasInfo(BrasInfo),
    cxList(CxList)
    //agg1CxList(Agg1CxList)
{

    singleAgg = SingleAgg;

    if(CustomInfo.trimmed().isEmpty())
    {
        CUSTOM_INFO = false;
        customInfo = "";
        headerHeight = 60;
    }
    else
    {
        CUSTOM_INFO = true;
        customInfo = CustomInfo;
        headerHeight = 90;
    }

    huaweiDslam = HuaweiDslam;
    UPLINK = false;
    if(!HuaweiDslam)
    {
        //zhone
        QString tmp = serviceMap.value(500);
        if(tmp.contains("-", Qt::CaseInsensitive)) UPLINK = true;
        if(tmp.contains(",", Qt::CaseInsensitive)) UPLINK = true;
    }

    vlanOffset =0 ;
    foreach (int v, BrasInfo.keys())
    {
        if(v > 499 && v < 600)
        {
            vlanOffset = v - 500;
            break;
        }
    }


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

ReportDSLAM::~ReportDSLAM()
{
    delete painter;
    painter = NULL;
}

void ReportDSLAM::preparePage()
{ // print a border on each page
    painter->resetTransform();
    painter->save();
    painter->setPen(Qt::darkMagenta);
    painter->setFont(QFont("tahoma", 16, QFont::Bold));
    QRect rec(0,0,painter->viewport().width()-50,50);
    painter->drawText(rec,Qt::AlignLeft | Qt::AlignVCenter, current_date);
    painter->drawText(rec,Qt::AlignCenter,"DaNet DSLAM Report");
    QPixmap im(":/Image/tct.jpg");
    painter->drawPixmap(painter->viewport().width()-50, 0,im);
    painter->setPen(Qt::darkMagenta);

    if(CUSTOM_INFO)
    {
        rec.setRect(10,60,painter->viewport().width()-20,30);
        painter->setPen(Qt::darkCyan);
        painter->setFont(QFont("tahoma", 14, QFont::Bold));
        painter->drawText(rec, Qt::AlignLeft,customInfo);
        painter->setPen(Qt::darkMagenta);
        painter->setFont(QFont("tahoma", 16, QFont::Bold));
    }

    painter->setFont(QFont("tahoma", 10));
    painter->translate(10, painter->viewport().height() );
    painter->setPen(QPen(QColor(0, 0, 0), 2));
    painter->drawText(0, 0, QString("Page %1").arg(pageNumber));
    pageNumber += 1;
    painter->restore();
    painter->translate(10,headerHeight);

}

bool ReportDSLAM::print()
{

    //check inputs
    bool d = dslamName.isEmpty();
    bool s = (serviceMap.count() > 0)? false : true;
    //bool p = (portMap.count() > 0)? false : true;
    bool n = (nmsMap.count() == 2)? false : true;
    bool a1 = (agg1List.count() == 6)? false: true;
    bool a2 = (agg2List.count() == 4)? false : true;
    bool a = (aggInfo.count() == 2)? false : true;
    bool b1 = (bras1List.count() == 3)? false : true;
    bool b2 = (bras2List.count() == 3)? false : true;
    bool b = (brasInfo.count() == 4) ? false : true;
    bool c = (cxList.count() == 6)? false : true;
    //bool ac = (agg1CxList.count() == 3)? false : true;

    if(d || s  || n || a1 || a2 || a || b1 || b2 || b || c ) //|| p || ac
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
        pr->setDocName("DSLAM Report");
        pr->setPrintProgram("DaNet Application");

        if(printTable(pr))
            return true;
        else
            return false;
    }
}

bool ReportDSLAM::printTable(QPrinter *printer)
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
    painter->fillRect(rect, QColor(150, 200, 150));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignCenter, dslamName);
    painter->translate(0, 40);
    painter->setFont(contentFont);

    ///

    //agg1 agg2 int count
    int agg1IntCount= 0, agg2IntCount= 0;
    bool agg1Flag= false,agg1Int2Flag= false, agg1Int3Flag= false, agg1Int4Flag= false, agg2Flag=false, agg2Int2Flag=false, cxFlag=false, cxInt2Flag = false;

    QString temp;
    temp = agg1List.at(0).trimmed();
    if(!temp.isEmpty())
        agg1Flag = true;

    temp = agg2List.at(0).trimmed();
    if(!temp.isEmpty())
        agg2Flag = true;


    temp = cxList.at(0).trimmed();
    if(!temp.isEmpty())
        cxFlag = true;


    temp = agg1List.at(2).trimmed();
    if(!temp.isEmpty())
    {
        agg1IntCount++;

        temp = agg1List.at(3).trimmed();
        if(!temp.isEmpty())
        {
            agg1IntCount++;
            agg1Int2Flag = true;
        }

        temp = agg1List.at(4).trimmed();
        if(!temp.isEmpty())
        {
            agg1IntCount++;
            agg1Int3Flag = true;
        }
        temp = agg1List.at(5).trimmed();
        if(!temp.isEmpty())
        {
            agg1IntCount++;
            agg1Int4Flag = true;
        }
    }

    temp = agg2List.at(2).trimmed();
    if(!temp.isEmpty())
    {
        agg2Flag = true;
        agg2IntCount++;
        temp = agg2List.at(3).trimmed();
        if(!temp.isEmpty())
        {
            agg2IntCount++;
            agg2Int2Flag = true;
        }
    }

    temp = cxList.at(3).trimmed();
    if(!temp.isEmpty())
        cxInt2Flag = true;

    /////

    int tableWidth = painter->viewport().width()-20;
    int tableHeight = painter->viewport().height()-bottomHeight-headerHeight-40;
    QList<int> headerWidth;
    int totalWidth;
    headerWidth << 10 << 30 << 30 << 30 << 60 << 30 << 30 ; //7 columns
    totalWidth = 0;
    foreach (int i, headerWidth)
        totalWidth += i;

    for(int i = 0; i < headerWidth.count(); i++)
    {
        headerWidth[i] = tableWidth * headerWidth[i] / totalWidth;
    }

    double rowHeight;
    //dslam part 19 -- service:6  nms:3  port: x
    // Agg 5 - 6(by agg2 site)
    // bras 5-9(2 bras)
    //metro 5
    // total 30 + portDescriptionCount + 1 > max 39
    int totalRowCount;
    int pmc = portMap.count();
    if(pmc == 0) pmc = 2;
    pmc++;
    if(agg1Flag || agg2Flag)
    {
        totalRowCount = 28 + pmc;//30

        //        if(agg1Flag)
        //            rowHeight = tableHeight / 38 ; // with max row consideration
        //        else
        //            rowHeight = tableHeight / 39;
    }
    else
        totalRowCount = 22 + pmc; //24

    rowHeight = tableHeight / totalRowCount;

    //########################DSLAM
    //DSLAM SERVICE PART
    double DslamPartHeight;
    DslamPartHeight = (8+pmc) * rowHeight; //10


    //left Header
    rect.setRect(0,0,headerWidth[0], DslamPartHeight);
    painter->fillRect(rect, QColor(230, 230, 240));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignCenter, "D\nS\nL\nA\nM");
    painter->translate(headerWidth[0],0);
    painter->setFont(topicFont);
    //services
    double serviceHeight = 5 * rowHeight; //7
    rect.setRect(0, 0, headerWidth[1], serviceHeight);
    painter->fillRect(rect, QColor(230, 230, 240));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignCenter, "SERVICES");
    painter->translate(headerWidth[1], 0);//vlan num top left

    rect.setRect(0,0,headerWidth[2], rowHeight);
    painter->fillRect(rect, QColor(230, 230, 240));
    painter->drawRect(rect);
    painter->setFont(topicFont);
    painter->setPen(Qt::darkMagenta);
    if(huaweiDslam)
        painter->drawText(rect, Qt::AlignCenter, "VLAN Number");
    else
        painter->drawText(rect, Qt::AlignCenter, "Tagged VLAN");

    painter->setPen(pen);
    painter->translate(0, rowHeight);
    painter->setFont(contentFont);

//    painter->drawRect(rect);
//    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter," "+ serviceMap.value(300));
//    painter->translate(0, rowHeight);

//    painter->drawRect(rect);
//    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter," "+ serviceMap.value(400));
//    painter->translate(0, rowHeight);

    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+serviceMap.value(500));
    painter->translate(0, rowHeight);

    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+serviceMap.value(600));
    painter->translate(0, rowHeight);

    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter," "+ serviceMap.value(700));
    painter->translate(0, rowHeight);

    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+serviceMap.value(800));
    painter->translate(0, rowHeight);

    //    painter->drawRect(rect);
    //    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter," "+ serviceMap.value(900));
    //    painter->translate(0, dslamRowHeight);

    //    painter->drawRect(rect);
    //    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+serviceMap.value(1000));
    //    painter->translate(0, dslamRowHeight);


    painter->translate(headerWidth[2], -serviceHeight);
    //--
    if(huaweiDslam)
    {
        rect.setRect(0,0,headerWidth[3], rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "VLAN TYPE");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);

//        painter->drawRect(rect);
//        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Smart");
//        painter->translate(0, rowHeight);

//        painter->drawRect(rect);
//        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Smart");
//        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Smart");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Smart");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Smart");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Smart");
        painter->translate(0, rowHeight);


        //        painter->drawRect(rect);
        //        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Smart");
        //        painter->translate(0, dslamRowHeight);

        //        painter->drawRect(rect);
        //        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Smart");
        //        painter->translate(0, dslamRowHeight);


        painter->translate(headerWidth[3], -serviceHeight);

        rect.setRect(0,0,headerWidth[4]+headerWidth[5]+headerWidth[6], rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter, "Description");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);


//        painter->drawRect(rect);
//        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Reserved");
//        painter->translate(0, rowHeight);

//        painter->drawRect(rect);
//        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " BTV-TCT");
//        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " TCT-PPPOE");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " INET-PPPOE");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " SINET-PPPOE");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " VOIP-PPPOE");
        painter->translate(0, rowHeight);

        //        painter->drawRect(rect);
        //        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Sip-Trunk-PPPOE");
        //        painter->translate(0, dslamRowHeight);

        //        painter->drawRect(rect);
        //        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Customer-VPN");
        //        painter->translate(0, dslamRowHeight);

        //--
        //DSLAM NMS
        painter->translate(-headerWidth[1]-headerWidth[2]-headerWidth[3], 0);
        double nmsHeight = 3*rowHeight;
        rect.setRect(0,0,headerWidth[1], nmsHeight);
        painter->setFont(headersFont);
        painter->fillRect(rect, QColor(230, 230, 240));//(230, 230, 240)
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter, "NMS");
        painter->setFont(topicFont);
        painter->translate(headerWidth[1], 0);//vlan int top left

        rect.setRect(0,0,headerWidth[2],rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "VLAN Interface");
        painter->setPen(pen);
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " 10");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " 11");

        painter->translate(headerWidth[2], -2*rowHeight);
        rect.setRect(0,0,headerWidth[3],rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "VLAN Type");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Standard");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Standard");


        painter->translate(headerWidth[3], -2*rowHeight);
        rect.setRect(0,0,headerWidth[4],rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "IP Address");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+nmsMap.value(10).at(0));
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+nmsMap.value(11).at(0));


        painter->translate(headerWidth[4], - 2*rowHeight);
        rect.setRect(0,0,headerWidth[5],rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Binding");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Global-Connection");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " MPLS-VPN_nms");

        painter->translate(headerWidth[5], - 2*rowHeight);
        rect.setRect(0,0,headerWidth[6],rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Gateway");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+nmsMap.value(10).at(1));
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+nmsMap.value(11).at(1));
    }
    else
    {
        QString tmp;
        rect.setRect(0,0,headerWidth[3], rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Description");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);


//        painter->drawRect(rect);
//        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Reserved");
//        painter->translate(0, rowHeight);

//        painter->drawRect(rect);
//        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " BTV-TCT");
//        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " TCT-PPPOE");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " INET-PPPOE");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " SINET-PPPOE");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " VOIP-PPPOE");
        painter->translate(0, rowHeight);

        //        painter->drawRect(rect);
        //        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Sip-Trunk-PPPOE");
        //        painter->translate(0, dslamRowHeight);

        //        painter->drawRect(rect);
        //        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Customer-VPN");
        //        painter->translate(0, dslamRowHeight);


        painter->translate(headerWidth[3], -serviceHeight);

        rect.setRect(0,0,headerWidth[4]+headerWidth[5]+headerWidth[6], rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter, "Bridge Type");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);

//        painter->drawRect(rect);
//        tmp = UPLINK?uplinkIntraVlan(serviceMap.value(300), 300) : " Uplink";
//        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, tmp);
//        painter->translate(0, rowHeight);

//        painter->drawRect(rect);
//        tmp = UPLINK? " Uplink & Intralink" : " Uplink";
//        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, tmp);
//        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        tmp = UPLINK?uplinkIntraVlan(serviceMap.value(500), 500) : " Uplink";
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, tmp);
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        tmp = UPLINK?uplinkIntraVlan(serviceMap.value(600), 600) : " Uplink";
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, tmp);
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        tmp = UPLINK?uplinkIntraVlan(serviceMap.value(700), 700) : " Uplink";
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, tmp);
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        tmp = UPLINK? uplinkIntraVlan(serviceMap.value(800), 800) : " Uplink";
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, tmp);
        painter->translate(0, rowHeight);

        //        painter->drawRect(rect);
        //        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, uplinkIntraVlan(serviceMap.value(900)));
        //        painter->translate(0, dslamRowHeight);

        //        painter->drawRect(rect);
        //        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Uplink & Intralink");
        //        painter->translate(0, dslamRowHeight);

        //--
        //DSLAM NMS
        painter->translate(-headerWidth[1]-headerWidth[2]-headerWidth[3], 0);
        double nmsHeight = 3*rowHeight;
        rect.setRect(0,0,headerWidth[1], nmsHeight);
        painter->setFont(headersFont);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter, "ZMS");
        painter->setFont(topicFont);
        painter->translate(headerWidth[1], 0);//vlan int top left

        rect.setRect(0,0,headerWidth[2],rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "IPOBridge VLAN");
        painter->setPen(pen);
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " 10");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " 11");

        painter->translate(headerWidth[2], -2*rowHeight);
        rect.setRect(0,0,headerWidth[3],rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Bridge Type");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " TLS");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " TLS");


        painter->translate(headerWidth[3], -2*rowHeight);
        rect.setRect(0,0,headerWidth[4],rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "IP Address");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+nmsMap.value(10).at(0));
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+nmsMap.value(11).at(0));


        painter->translate(headerWidth[4], - 2*rowHeight);
        rect.setRect(0,0,headerWidth[5],rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Binding");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Global-Connection");
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " MPLS-VPN_nms");

        painter->translate(headerWidth[5], - 2*rowHeight);
        rect.setRect(0,0,headerWidth[6],rowHeight);
        painter->fillRect(rect, QColor(230, 230, 240));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Gateway");
        painter->setPen(pen);
        painter->translate(0, rowHeight);
        painter->setFont(contentFont);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+nmsMap.value(10).at(1));
        painter->translate(0, rowHeight);

        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+nmsMap.value(11).at(1));
    }

    //--

    //PORT DESC
    painter->translate(-headerWidth[1]-headerWidth[2]-headerWidth[3]-headerWidth[4]-headerWidth[5], rowHeight);
    double portHeight = (pmc )* rowHeight;
    rect.setRect(0,0,headerWidth[1], portHeight);
    painter->fillRect(rect, QColor(230, 230, 240));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignCenter, "Port\nDescription");
    painter->translate(headerWidth[1], 0);

    rect.setRect(0,0,headerWidth[2], rowHeight);
    painter->fillRect(rect,QColor(230, 230, 240) );
    painter->drawRect(rect);
    painter->setFont(topicFont);
    painter->setPen(Qt::darkMagenta);
    painter->drawText(rect, Qt::AlignCenter, "PORT");
    painter->setPen(pen);

    int hw = headerWidth[3]+headerWidth[4]+headerWidth[5]+headerWidth[6];
    painter->translate(headerWidth[2],0);
    rect.setRect(0,0,hw, rowHeight);
    painter->fillRect(rect,QColor(230, 230, 240) );
    painter->drawRect(rect);
    painter->setPen(Qt::darkMagenta);
    painter->drawText(rect, Qt::AlignCenter, "Description");
    painter->setPen(pen);
    painter->translate(-headerWidth[2], rowHeight);
    painter->setFont(contentFont);
    if(portMap.count() == 0)
        portMap.insert("", "");
    foreach (QString p, portMap.keys())
    {
        rect.setRect(0,0,headerWidth[2], rowHeight);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+p);
        painter->translate(headerWidth[2], 0);
        rect.setRect(0,0,hw, rowHeight);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+portMap.value(p));
        painter->translate(-headerWidth[2], rowHeight);
    }
    painter->translate(-headerWidth[0]-headerWidth[1], 0);

    //########################AGG

    if(agg1Flag || agg2Flag)
    {
        int aggPartHeight;

        if(agg1Flag)
            aggPartHeight = 5 * rowHeight;
        else
            aggPartHeight = 6 * rowHeight;

        int aggHeight;
        if(agg1Flag)
            aggHeight = aggPartHeight - rowHeight;
        else
            aggHeight = aggPartHeight - 2*rowHeight;

        aggHeight = aggHeight /(agg1Flag + agg2Flag);
        int agg1RowHeight, agg2RowHeight;
        if(agg1IntCount > 0)
            agg1RowHeight = aggHeight / agg1IntCount;
        else
            agg1RowHeight = 0;

        if(agg2IntCount > 0)
            agg2RowHeight = aggHeight / agg2IntCount;
        else
            agg2RowHeight = 0;

        rect.setRect(0,0, headerWidth[0], aggPartHeight);
        painter->fillRect(rect, QColor(200, 230, 250));
        painter->drawRect(rect);
        painter->setFont(headersFont);
        painter->drawText(rect, Qt::AlignCenter, "A\nG\nG");
        painter->translate(headerWidth[0],0);
        painter->setFont(topicFont);

        rect.setRect(0,0,headerWidth[1], rowHeight);
        painter->fillRect(rect, QColor(200, 230, 250));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Switch");
        painter->setPen(pen);

        painter->translate(headerWidth[1], 0);
        rect.setRect(0,0,headerWidth[2], rowHeight);
        painter->fillRect(rect, QColor(200, 230, 250));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Eth-Trunk");
        painter->setPen(pen);

        painter->translate(headerWidth[2], 0);
        rect.setRect(0,0,headerWidth[3], rowHeight);
        painter->fillRect(rect, QColor(200, 230, 250));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Interface");
        painter->setPen(pen);

        painter->translate(headerWidth[3], 0);
        rect.setRect(0,0,headerWidth[5]+headerWidth[6], rowHeight);
        painter->fillRect(rect, QColor(200, 230, 250));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Description");
        painter->setPen(pen);

        painter->translate(headerWidth[4], 0);
        rect.setRect(0,0,headerWidth[4], rowHeight);
        painter->fillRect(rect, QColor(200, 230, 250));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Allow-Pass VLAN");
        painter->setPen(pen);

        painter->translate(-headerWidth[1]-headerWidth[2]-headerWidth[3]-headerWidth[4], rowHeight);
        if(agg1Flag)
        {
            painter->setFont(topicFont);
            rect.setRect(0,0, headerWidth[1], aggHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, agg1List.at(0)); // agg1
            if(agg2Flag) painter->translate(0,aggHeight);
        }


        if(agg2Flag)
        {
            rect.setRect(0,0, headerWidth[1], aggHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, agg2List.at(0)); // agg2
            if(agg1Flag) painter->translate(0,-aggHeight);
        }

        painter->setFont(contentFont);

        painter->translate(headerWidth[1], 0);
        if(agg1Flag)
        {
            rect.setRect(0,0,headerWidth[2], aggHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignCenter, " "+agg1List.at(1)); // eth1
            if(agg2Flag) painter->translate(0,aggHeight);
        }


        if(agg2Flag)
        {
            rect.setRect(0,0,headerWidth[2], aggHeight);
            painter->drawRect(rect);
            painter->drawText(rect,  Qt::AlignCenter, " "+agg2List.at(1)); // eth2
            if(agg1Flag) painter->translate(0,-aggHeight);
        }

        //int
        painter->translate(headerWidth[2], 0);
        if(agg1Flag)
        {
            rect.setRect(0,0, headerWidth[3], agg1RowHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+ agg1List.at(2));

            if(agg1Int2Flag)
            {
                rect.setRect(0,agg1RowHeight, headerWidth[3], agg1RowHeight);
                painter->drawRect(rect);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+ agg1List.at(3));// int2 of agg1
            }
            if(agg1Int3Flag)
            {
                rect.setRect(0,2*agg1RowHeight, headerWidth[3], agg1RowHeight);
                painter->drawRect(rect);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+ agg1List.at(4));// int3 of agg1
            }
            if(agg1Int4Flag)
            {
                rect.setRect(0,3*agg1RowHeight, headerWidth[3], agg1RowHeight);
                painter->drawRect(rect);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+ agg1List.at(5));// int4 of agg1
            }

            if(agg2Flag) painter->translate(0,aggHeight);
        }


        if(agg2Flag)
        {
            rect.setRect(0,0, headerWidth[3], agg2RowHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+ agg2List.at(2));// int1 of agg2

            if(agg2Int2Flag)
            {
                rect.setRect(0,agg2RowHeight, headerWidth[3], agg2RowHeight);
                painter->drawRect(rect);
                painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+ agg2List.at(3));// int2 of agg2
            }
            if(agg1Flag) painter->translate(0,-aggHeight);
        }

        painter->translate(headerWidth[3], 0);
        if(!agg1Flag && agg2Flag)
            rect.setRect(0,0,headerWidth[4], aggPartHeight-2*rowHeight);
        else
            rect.setRect(0,0,headerWidth[4], aggPartHeight-rowHeight);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, " "+aggInfo.at(1));

        painter->translate(headerWidth[4], 0);// ap top left
        rect.setRect(0,0,headerWidth[5]+headerWidth[6], aggPartHeight-rowHeight);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, " "+aggInfo.at(0)); //allow pass



        int hw = -headerWidth[0] - headerWidth[1]-headerWidth[2]-headerWidth[3]-headerWidth[4];
        if(agg1Flag)
        {
            painter->translate(hw,aggPartHeight-rowHeight);// bras top left
        }
        else
        {
            //inter agg
            painter->translate(hw+headerWidth[0],aggHeight);
            rect.setRect(0,0,headerWidth[1]+headerWidth[2]+headerWidth[3]+headerWidth[4], rowHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " Inter AGG1 and AGG2");
            painter->translate(-headerWidth[0],rowHeight);
        }
    }

    //########################BRAS

    bool bras2Flag = false;
    temp = bras2List.at(0).trimmed();
    if(!temp.isEmpty())
        bras2Flag = true;

    int brasPartHeight;
    if(bras2Flag)
        brasPartHeight = 9 * rowHeight;
    else
        brasPartHeight = 5 * rowHeight;

    rect.setRect(0,0, headerWidth[0], brasPartHeight);
    painter->fillRect(rect, QColor(180, 240, 200));
    painter->drawRect(rect);
    painter->setFont(headersFont);
    painter->drawText(rect, Qt::AlignCenter, "B\nR\nA\nS");
    painter->setFont(topicFont);

    painter->translate(headerWidth[0], 0);
    rect.setRect(0,0,headerWidth[1], rowHeight);
    painter->fillRect(rect, QColor(180, 240, 200));
    painter->drawRect(rect);
    painter->setPen(Qt::darkMagenta);
    painter->drawText(rect, Qt::AlignCenter, "BRAS");
    painter->setPen(pen);
    painter->translate(headerWidth[1], 0);
    rect.setRect(0,0,headerWidth[2], rowHeight);
    painter->fillRect(rect, QColor(180, 240, 200));
    painter->drawRect(rect);
    painter->setPen(Qt::darkMagenta);
    painter->drawText(rect, Qt::AlignCenter, "AGG1 Eth-Trunk");
    QString tmp;
    if(singleAgg)
    {
        painter->setPen(pen);
        painter->translate(headerWidth[3], 0);
        rect.setRect(0,0,headerWidth[4]/2, rowHeight);
        painter->fillRect(rect, QColor(180, 240, 200));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "VLAN Number");

        painter->setPen(pen);
        painter->translate(headerWidth[4]/2, 0);
        rect.setRect(0,0,headerWidth[3]+headerWidth[5]+headerWidth[6]+headerWidth[4]/2, rowHeight);
        painter->fillRect(rect, QColor(180, 240, 200));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Description");
        painter->setPen(pen);
        hw = headerWidth[1]+headerWidth[3]+headerWidth[4]/2;
        painter->translate(-hw, rowHeight);// ME60-1 top left

        int meHeight = 4 * rowHeight;
        painter->setFont(topicFont);
        rect.setRect(0,0, headerWidth[1],meHeight);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, bras1List.at(0)); // me-1
        if(bras2Flag)
        {
            rect.setRect(0,meHeight, headerWidth[1],meHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, bras2List.at(0)); // me-2
        }

        painter->setFont(contentFont);
        painter->translate(headerWidth[3], 0);//sw eth
        rect.setRect(0,0, headerWidth[3], meHeight);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter, bras1List.at(1));
        if(bras2Flag)
        {
            rect.setRect(0,meHeight, headerWidth[3], meHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignCenter, bras2List.at(1));
        }

        painter->translate(headerWidth[3], 0); // >>> vlan

        foreach (int v, brasInfo.keys())
        {
            rect.setRect(0,0,headerWidth[4]/2, rowHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+QString::number(v));

            rect.setRect(headerWidth[4]/2, 0, headerWidth[2]+headerWidth[5]+headerWidth[6]+headerWidth[4]/2, rowHeight);
            painter->drawRect(rect);
            tmp = brasInfo.value(v);
            tmp = tmp.replace(" ","");
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter |  Qt::TextWordWrap, " "+tmp);

            painter->translate(0, rowHeight);
        }

    }
    else
    {
        painter->setPen(pen);
        painter->translate(headerWidth[2], 0);
        rect.setRect(0,0,headerWidth[3], rowHeight);
        painter->fillRect(rect, QColor(180, 240, 200));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "AGG2 Eth-Trunk");
        painter->setPen(pen);
        painter->translate(headerWidth[3], 0);
        rect.setRect(0,0,headerWidth[4]/2, rowHeight);
        painter->fillRect(rect, QColor(180, 240, 200));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "VLAN Number");
        painter->setPen(pen);
        painter->translate(headerWidth[4]/2, 0);
        rect.setRect(0,0,headerWidth[5]+headerWidth[6]+headerWidth[4]/2, rowHeight);
        painter->fillRect(rect, QColor(180, 240, 200));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Description");
        painter->setPen(pen);
        hw = headerWidth[1]+headerWidth[2]+headerWidth[3]+headerWidth[4]/2;
        painter->translate(-hw, rowHeight);// ME60-1 top left

        int meHeight = 4 * rowHeight;
        painter->setFont(topicFont);
        rect.setRect(0,0, headerWidth[1],meHeight);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, bras1List.at(0)); // me-1
        if(bras2Flag)
        {
            rect.setRect(0,meHeight, headerWidth[1],meHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, bras2List.at(0)); // me-2
        }

        painter->setFont(contentFont);
        painter->translate(headerWidth[2], 0);//sw eth
        rect.setRect(0,0, headerWidth[2], meHeight);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter, bras1List.at(1));
        if(bras2Flag)
        {
            rect.setRect(0,meHeight, headerWidth[2], meHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignCenter, bras2List.at(1));
        }

        painter->translate(headerWidth[2], 0); // ME1-eth2
        rect.setRect(0,0, headerWidth[3], meHeight);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter, bras1List.at(2));
        if(bras2Flag)
        {
            rect.setRect(0,meHeight, headerWidth[3], meHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignCenter, bras2List.at(2));
        }

        painter->translate(headerWidth[3], 0);//vlan num
        QString tmp;
        foreach (int v, brasInfo.keys())
        {
            rect.setRect(0,0,headerWidth[4]/2, rowHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+QString::number(v));

            rect.setRect(headerWidth[4]/2, 0, headerWidth[5]+headerWidth[6]+headerWidth[4]/2, rowHeight);
            painter->drawRect(rect);
            tmp = brasInfo.value(v);
            tmp = tmp.replace(" ","");
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter |  Qt::TextWordWrap, " "+tmp);

            painter->translate(0, rowHeight);
        }

    }

    if(bras2Flag)
    {
        foreach (int v, brasInfo.keys())
        {
            rect.setRect(0,0,headerWidth[4]/2, rowHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+QString::number(v));

            rect.setRect(headerWidth[4]/2, 0, headerWidth[2]+headerWidth[5]+headerWidth[6]+headerWidth[4]/2,rowHeight);
            painter->drawRect(rect);
            tmp = brasInfo.value(v);
            tmp = tmp.replace(" ","");
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter|  Qt::TextWordWrap, " "+tmp);

            painter->translate(0, rowHeight);
        }
    }

    //########################METRO
    painter->translate(-headerWidth[0]-headerWidth[1]-headerWidth[3], 0);

    int cxRowHeight, metroHeight;
    metroHeight = 5 * rowHeight;

    if(cxFlag)
    {
        if(cxInt2Flag)
            cxRowHeight = 2 * rowHeight;
        else
            cxRowHeight = 4 * rowHeight;

        rect.setRect(0,0, headerWidth[0],metroHeight);
        painter->fillRect(rect, QColor(220, 200, 200));
        painter->drawRect(rect);
        painter->setFont(headersFont);
        painter->drawText(rect, Qt::AlignCenter, "M\nE\nT\nR\nO");
        painter->translate(headerWidth[0], 0);
        painter->setFont(topicFont);

        rect.setRect(0,0,headerWidth[1], rowHeight);
        painter->fillRect(rect,QColor(220, 200, 200));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Switch");
        painter->setPen(pen);

        rect.setRect(0,rowHeight, headerWidth[1], 4*rowHeight);
        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, cxList.at(0));

        painter->translate(headerWidth[1], 0);
        rect.setRect(0,0,headerWidth[2], rowHeight);
        painter->fillRect(rect,QColor(220, 200, 200));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Eth-Trunk");
        painter->setPen(pen);

        rect.setRect(0,rowHeight, headerWidth[2], 4*rowHeight);
        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignCenter, cxList.at(1));

        painter->translate(headerWidth[2], 0);//interface
        rect.setRect(0,0,headerWidth[3], rowHeight);
        painter->fillRect(rect,QColor(220, 200, 200));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Interface");
        painter->setPen(pen);

        rect.setRect(0,rowHeight, headerWidth[3], cxRowHeight);
        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter," "+ cxList.at(2));

        if(cxInt2Flag)
        {
            rect.setRect(0,rowHeight+cxRowHeight, headerWidth[3], cxRowHeight);
            painter->drawRect(rect);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, " "+cxList.at(3));
        }

//        painter->translate(headerWidth[3], 0);//ap
//        rect.setRect(0,0,headerWidth[4], rowHeight);
//        painter->fillRect(rect,QColor(220, 200, 200));
//        painter->drawRect(rect);
//        painter->setFont(topicFont);
//        painter->setPen(Qt::darkMagenta);
//        painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "Allow-Pass VLAN");
//        painter->setPen(pen);

//        rect.setRect(0,rowHeight, headerWidth[4], 4*rowHeight);
//        painter->drawRect(rect);
//        painter->setFont(contentFont);
//        painter->drawText(rect, Qt::AlignCenter, cxList.at(4));

//        painter->translate(headerWidth[4], 0);//desc
//        rect.setRect(0,0,headerWidth[5]+headerWidth[6], rowHeight);
//        painter->fillRect(rect,QColor(220, 200, 200));
//        painter->drawRect(rect);
//        painter->setFont(topicFont);
//        painter->setPen(Qt::darkMagenta);
//        painter->drawText(rect, Qt::AlignCenter, "Description");
//        painter->setPen(pen);

//        rect.setRect(0,rowHeight, headerWidth[5]+headerWidth[6], 4*rowHeight);
//        painter->drawRect(rect);
//        painter->setFont(contentFont);
//        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, " "+cxList.at(5));

        painter->translate(headerWidth[3], 0);//desc
        rect.setRect(0,0,headerWidth[4]+headerWidth[5]+headerWidth[6], rowHeight);
        painter->fillRect(rect,QColor(220, 200, 200));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Description");
        painter->setPen(pen);

        rect.setRect(0,rowHeight, headerWidth[4]+headerWidth[5]+headerWidth[6], 4*rowHeight);
        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, " "+cxList.at(5));



    }
    else if(interSwitchFlag && false)
    {
        cxRowHeight = 2 * rowHeight;

        rect.setRect(0,0, headerWidth[0],metroHeight);
        painter->fillRect(rect, QColor(220, 200, 200));
        painter->drawRect(rect);
        painter->setFont(headersFont);
        painter->drawText(rect, Qt::AlignCenter, "M\nE\nT\nR\nO");
        painter->translate(headerWidth[0], 0);
        painter->setFont(topicFont);

        rect.setRect(0,0,headerWidth[1]+headerWidth[2], rowHeight);
        painter->fillRect(rect,QColor(220, 200, 200));
        painter->drawRect(rect);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Inter-Switch");
        painter->setPen(pen);

        rect.setRect(0,rowHeight, headerWidth[1]+headerWidth[2], cxRowHeight);
        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "AGG1 TO CX600");

        rect.setRect(0,rowHeight+cxRowHeight, headerWidth[1]+headerWidth[2], cxRowHeight);
        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "CX600 TO AGG1");


        painter->translate(headerWidth[1]+headerWidth[2], 0);
        rect.setRect(0,0,headerWidth[3], rowHeight);
        painter->fillRect(rect,QColor(220, 200, 200));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Eth-Trunk");
        painter->setPen(pen);

        rect.setRect(0,rowHeight, headerWidth[3], cxRowHeight);
        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignCenter, ""); //agg1CxList.at(0)

        rect.setRect(0,rowHeight+cxRowHeight, headerWidth[3], cxRowHeight);
        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignCenter,"" );//agg1CxList.at(1)

        painter->translate(headerWidth[3], 0);
        rect.setRect(0,0,headerWidth[4]+headerWidth[5]+headerWidth[6], rowHeight);
        painter->fillRect(rect,QColor(220, 200, 200));
        painter->drawRect(rect);
        painter->setFont(topicFont);
        painter->setPen(Qt::darkMagenta);
        painter->drawText(rect, Qt::AlignCenter, "Allow-Pass VLAN");
        painter->setPen(pen);

        rect.setRect(0,rowHeight, headerWidth[4]+headerWidth[5]+headerWidth[6], 2*cxRowHeight);

        painter->drawRect(rect);
        painter->setFont(contentFont);
        painter->drawText(rect, Qt::AlignCenter,"");// agg1CxList.at(2)
    }


    painter->end();

    return true;
}

QString ReportDSLAM::uplinkIntraVlan(QString vlanRange, int vlanBase)
{
    QList<int> rangeList = rangeToList(vlanRange);
    QString temp;

    if(rangeList.count() == 0) return "";
    if(rangeList.count() == 1)
        return " "+vlanRange+": Uplink";
    else
    {
        temp = " "+vlanRange + ": Uplink & ";
        int vlan = vlanBase + vlanOffset;
        int ind = rangeList.indexOf(vlan);
        rangeList.removeAt(ind);
        temp += listToRange(rangeList)+": Intralink";

        return temp;
    }
}

QString ReportDSLAM::listToRange(QList<int> list)
{
    int i,length, min,max, value;

    QMap<int, int> map;
    foreach(int vlan, list)
        map.insert(vlan,vlan);

    list.clear();
    foreach (int vlan, map.keys())
        list << vlan;


    QString range = "";
    length = list.length();
    if(length == 0) return range;

    if(length == 1)
        range = QString::number(list.at(0));
    else
    {
        i = 0;
        while((i < length))
        {
            min = list[i];
            max = min;
            i++;
            if(i < length)
            {
                value = list[i];
                if((max+1) == value)
                {
                    max = value;
                    i++;
                    while(i < length)
                    {
                        value = list[i];
                        if(max+1 == value)
                        {
                            max = value;
                            i++;
                            continue;
                        }
                        else
                            break;

                    }
                }
            }

            if(range.isEmpty())
            {
                if(max > min)
                    range = QString::number(min)+"-"+QString::number(max);
                else
                    range = QString::number(min);
            }
            else
            {
                if(max > min)
                    range += ","+QString::number(min)+"-"+QString::number(max);
                else
                    range += ","+QString::number(min);
            }

        }
    }


    return range;

}

QList<int> ReportDSLAM::rangeToList(QString range)
{
    QList<int> list;
    range += ",";
    bool ok;
    int value, min, max;
    QStringList splitList,splitRange;
    splitList = range.split(',',QString::SkipEmptyParts);
    if(splitList.length() > 0)
    {
        foreach (QString str, splitList)
        {
            if(str.contains("-"))
            {
                splitRange = str.split('-', QString::SkipEmptyParts);
                if(splitRange.length() == 2)
                {
                    min = (splitRange.at(0)).toInt(&ok);
                    if(ok)
                        max = (splitRange.at(1)).toInt(&ok);

                    if((max >= min) && ok)
                        for(int i = min; i <= max; i++)
                            list << i;
                }

            }
            else
            {
                value = str.toInt(&ok);
                if(ok)
                    list << value;
            }
        }
    }

    QMap<int, int> map;
    foreach(int vlan, list)
        map.insert(vlan,vlan);

    list.clear();
    foreach (int vlan, map.keys())
        list << vlan;

    return list;
}

QString ReportDSLAM::lastError()
{
    return error;
}
