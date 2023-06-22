#include "nettools.h"
#include <QtMath>
#include <QStandardItemModel>

NetTools::NetTools(QObject *parent) : QObject(parent)
{
}

QMap<QString, int> NetTools::classAFirstOctetRange()
{
    QMap<QString, int> map;
    map.insert("start", 1);
    map.insert("end", 126);
    return map;
}

QMap<QString, int> NetTools::classBFirstOctetRange()
{
    QMap<QString, int> map;
    map.insert("start", 128);
    map.insert("end", 191);
    return map;
}

QMap<QString, int> NetTools::classCFirstOctetRange()
{
    QMap<QString, int> map;
    map.insert("start", 192);
    map.insert("end", 223);
    return map;
}

QMap<QString, int> NetTools::classDFirstOctetRange()
{
    QMap<QString, int> map;
    map.insert("start", 224);
    map.insert("end", 239);
    return map;
}

QMap<QString, int> NetTools::classEFirstOctetRange()
{
    QMap<QString, int> map;
    map.insert("start", 240);
    map.insert("end", 254);
    return map;
}

QString NetTools::classAFirstOctetMSB()
{
    return " 0 ";
}

QString NetTools::classBFirstOctetMSB()
{
    return " 10 ";
}

QString NetTools::classCFirstOctetMSB()
{
    return " 110 ";
}

QString NetTools::classDFirstOctetMSB()
{
    return " 1110 ";
}

QString NetTools::classEFirstOctetMSB()
{
    return " 1111 ";
}

QString NetTools::ipClassIdentify(QString ip)
{
    QMap<int, int> map;
    map = ipMaskOctets(ip);
    int octet0 = map.value(0);
    if(octet0 >= 0 && octet0 < 127) return "A";
    else if(octet0 == 127 ) return "LoopBack";
    else if(octet0 >= 128 && octet0 < 192) return "B";
    else if(octet0 >= 192 && octet0 < 224) return "C";
    else if(octet0 >= 224 && octet0 < 240) return "D";
    else if(octet0 >= 240 && octet0 <= 254) return "E";

    return "-1";
}

QString NetTools::classADefaultSubnetMask()
{
    return QString(" 255.0.0.0 ");
}

QString NetTools::classBDefaultSubnetMask()
{
    return QString(" 255.255.0.0 ");
}

QString NetTools::classCDefaultSubnetMask()
{
    return QString(" 255.255.255.0 ");
}

QString NetTools::classDDefaultSubnetMask()
{
    return QString("Used For Multicasting");
}

QString NetTools::classEDefaultSubnetMask()
{
    return QString("Used For Reseaching");
}

QString NetTools::classANumberOfNetwork()
{
    return QString(" 2^7 - 2 = 126 ");
}

QString NetTools::classBNumberOfNetwork()
{
    return QString(" 2^14 - 2 = 16382 ");
}

QString NetTools::classCNumberOfNetwork()
{
    return QString(" 2^21 - 2 = 2097150 ");
}

QString NetTools::classDNumberOfNetwork()
{
    return QString("Used For Multicasting");
}

QString NetTools::classENumberOfNetwork()
{
    return QString("Used For Reseaching");
}

QString NetTools::classAHostPerNetwork()
{
    return QString(" 2^24 - 2 = 16777214 ");
}

QString NetTools::classBHostPerNetwork()
{
    return QString(" 2^16 - 2 = 65534 ");
}

QString NetTools::classCHostPerNetwork()
{
    return QString(" 2^8 - 2 = 254 ");
}

QString NetTools::classDHostPerNetwork()
{
    return QString("Used For Multicasting");
}

QString NetTools::classEHostPerNetwork()
{
    return QString("Used For Researching");
}

QString NetTools::classAPrivateNetwork()
{
    return QString("10.0.0.0/8");
}

QString NetTools::classBPrivateNetwork()
{
    return QString("172.16.0.0/12");
}

QString NetTools::classCPrivateNetwork()
{
    return QString("192.168.0.0/16");
}

QString NetTools::classDPrivateNetwork()
{
    return QString("Used For Multicasting");
}

QString NetTools::classEPrivateNetwork()
{
    return QString("Used For Researching");
}

QString NetTools::classAPrivateNetworkMask()
{
    return QString("255.0.0.0");
}

QString NetTools::classBPrivateNetworkMask()
{
    return QString("255.240.0.0");
}

QString NetTools::classCPrivateNetworkMask()
{
    return QString("255.255.0.0");
}

QString NetTools::classDPrivateNetworkMask()
{
    return QString("Used For Multicasting");
}

QString NetTools::classEPrivateNetworkMask()
{
    return QString("Used For Researching");
}

bool NetTools::ipv4Validate(QString ip)
{
    QStringList list = ip.split(".", QString::SkipEmptyParts);
    int SIZE = list.size();
    if(SIZE < 4 || SIZE > 4) return false;
    int a;
    bool ok;
    for(int i=0; i < 4; i++)
    {
        a = QString(list.at(i)).toInt(&ok, 10);
        if(!ok || a < 0 || a > 255) return false;
    }
    return true;
}

bool NetTools::ipv4PrefixValidate(const QString ipPrefix)
{
    QStringList ipp;
    QString ip, prefix;
    if(!ipPrefix.contains('/'))
        return false;

    if(ipPrefix.endsWith('/'))
        return false;

    ipp = ipPrefix.split('/',QString::SkipEmptyParts);
    ip = ipp[0];
    prefix = ipp[1];

    if(! prefixValidate(prefix.toInt()))
        return false;

    QStringList list = ip.split(".", QString::SkipEmptyParts);
    int SIZE = list.size();
    if(SIZE < 4 || SIZE > 4) return false;
    int a;
    bool ok;
    for(int i=0; i < 4; i++)
    {
        a = QString(list.at(i)).toInt(&ok, 10);
        if(!ok || a < 0 || a > 255) return false;
    }
    return true;
}

bool NetTools::subnetMaskValidate(QString mask)
{
    if(!ipv4Validate(mask)) return false;
    QMap<int, int> map;
    int index;
    bool verify = false;
    map = ipMaskOctets(mask);

    if(map.value(0) < 255)
    {
        if( map.value(1) == 0 && map.value(2) == 0 && map.value(3) == 0 ){ verify = true; index=0;}
    }
    else if(map.value(1) < 255)
    {
        if( map.value(2) == 0 && map.value(3) == 0 ) {verify = true; index = 1;}
    }
    else if(map.value(2) < 255)
    {
        if( map.value(3) == 0 ){ verify = true; index = 2;}
    }
    else {verify = true; index = 3;}

    if(verify)
    {
        if(map.value(index) == 0 || map.value(index) == 128 || map.value(index) == 192 || map.value(index) == 224 || map.value(index) == 240 || map.value(index) == 248
                || map.value(index) == 252 || map.value(index) == 254 || map.value(index) == 255 )
            verify = true;
        else verify = false;

        if(index == 3 && map.value(3) == 254) verify = false;
    }

    return verify;
}

bool NetTools::prefixValidate(int prefix)
{
    if(prefix < 4 || prefix > 32 ) return false; else return true;
}

QString NetTools::netId(QString ip, int prefix)
{
    QMap<QString, int> indexValue = prefixIndexValue(prefix);
    int index = indexValue.value("index");
    QMap<int, int> octets = ipMaskOctets(ip);
    octets[index] = (octets[index] & indexValue.value("value"));// netID
    for(int i = (index+1); i < 4; i++)
        octets[i] = 0;
    return octetsToIp(octets[0], octets[1], octets[2], octets[3]);
}

QString NetTools::broadcast(QString ip, int prefix)
{
    ip = NetTools::netId(ip, prefix);
    QMap<int, int> ipOctet = ipMaskOctets(ip);
    QString mask = prefixToSubnetMask(prefix);
    QMap<int, int> maskOctet = ipMaskOctets(mask);

    int BC[4];
    for(int i = 0; i < 4; i++)
    {
        if(maskOctet.value(i) == 255)
            BC[i] = ipOctet.value(i);
        else
            BC[i] = ipOctet.value(i) + (256 - maskOctet.value(i) - 1 );
    }


    QString bc = octetsToIp(BC[0], BC[1], BC[2], BC[3]);

    return bc;
}

QString NetTools::netFirstIp(QString net, int prefix)
{
    net = NetTools::netId(net, prefix);
    QMap<int, int> ipOctet = ipMaskOctets(net);

    if(prefix < 32)
        ipOctet.insert(3, (ipOctet.value(3) + 1));

    QString ip = octetsToIp(ipOctet.value(0), ipOctet.value(1), ipOctet.value(2), ipOctet.value(3));

    return ip;
}

QString NetTools::netLastIp(QString net, int prefix)
{
    net = NetTools::broadcast(net, prefix);
    QMap<int, int> ipOctet = ipMaskOctets(net);

    int ip[4];
    for(int i = 0; i < 4; i++)
        ip[i] = ipOctet.value(i);

    ip[3] = ip[3] - 1;
    QString IP = octetsToIp(ip[0], ip[1], ip[2], ip[3]);

    return IP;
}

QMap<int, int> NetTools::ipMaskOctets(QString ip)
{
    QMap<int, int> map; // index, octet value
    QStringList list = ip.split(".");
    map.insert(0, QString(list.at(0)).toInt() );
    map.insert(1, QString(list.at(1)).toInt() );
    map.insert(2, QString(list.at(2)).toInt() );
    map.insert(3, QString(list.at(3)).toInt() );
    return map;
}

QString NetTools::octetsToIp(int octet0, int octet1, int octet2, int octet3)
{
    if(octet0 < 0 || octet1 < 0 || octet2 < 0 || octet3 < 0) return NULL;
    if(octet0 > 255 || octet1 > 255 || octet2 > 255 || octet3 > 255) return NULL;
    return (QString::number(octet0) + '.' + QString::number(octet1) + '.' + QString::number(octet2) + '.' + QString::number(octet3) );
}

int NetTools::subnetMaskToPrefix(QString mask)
{
    if(!subnetMaskValidate(mask)) return -1;
    QMap<int, int> maskValueNetBit; // 255:8   0:0
    //QMap<int, int> maskValueHostBit; // 255:0  0:8

    maskValueNetBit.insert(255, 8);
    maskValueNetBit.insert(254, 7);
    maskValueNetBit.insert(252, 6);
    maskValueNetBit.insert(248, 5);
    maskValueNetBit.insert(240, 4);
    maskValueNetBit.insert(224, 3);
    maskValueNetBit.insert(192, 2);
    maskValueNetBit.insert(128, 1);
    maskValueNetBit.insert(0  , 0);

    //    maskValueHostBit.insert(255, 0);
    //    maskValueHostBit.insert(255, 1);
    //    maskValueHostBit.insert(255, 2);
    //    maskValueHostBit.insert(255, 3);
    //    maskValueHostBit.insert(255, 4);
    //    maskValueHostBit.insert(255, 5);
    //    maskValueHostBit.insert(255, 6);
    //    maskValueHostBit.insert(255, 7);
    //    maskValueHostBit.insert(255, 8);

    QMap<int, int> map;
    map = ipMaskOctets(mask);
    int CIDR = maskValueNetBit.value(map.value(0)) + maskValueNetBit.value(map.value(1)) + maskValueNetBit.value(map.value(2)) + maskValueNetBit.value(map.value(3));
    return CIDR;
}

QString NetTools::prefixToSubnetMask(int prefix)
{
    if(!prefixValidate(prefix)) return NULL;

    QMap<int, int> prefixToSubnetMaskValue;

    prefixToSubnetMaskValue.insert(0, 0);
    prefixToSubnetMaskValue.insert(1, 128);
    prefixToSubnetMaskValue.insert(2, 192);
    prefixToSubnetMaskValue.insert(3, 224);
    prefixToSubnetMaskValue.insert(4, 240);
    prefixToSubnetMaskValue.insert(5, 248);
    prefixToSubnetMaskValue.insert(6, 252);
    prefixToSubnetMaskValue.insert(7, 254);
    prefixToSubnetMaskValue.insert(8, 255);

    int index;
    QString mask;

    if(prefix <= 8)
    {
        index = prefixToSubnetMaskValue.value(prefix);
        mask = QString::number(index) + ".0.0.0";
    }
    else if(prefix > 8 && prefix <= 16)
    {
        prefix = prefix - 8;
        index = prefixToSubnetMaskValue.value(prefix);
        mask = "255." +QString::number(index) + ".0.0";
    }
    else if(prefix > 16 && prefix <=24)
    {
        prefix = prefix - 16;
        index = prefixToSubnetMaskValue.value(prefix);
        mask = "255.255." +QString::number(index) + ".0";
    }
    else if(prefix > 24)
    {
        prefix = prefix - 24;
        index = prefixToSubnetMaskValue.value(prefix);
        mask = "255.255.255." +QString::number(index);
    }

    return mask;
}

int NetTools::prefixHostCount(int prefix)
{
    int diff = 32 - prefix;
    if(diff == 0) return 1;
    if(diff == 1) return 2;
    int host = pow(2, diff) -2;
    return host;
}

QMap<QString, int> NetTools::prefixIndexValue(int prefix)
{
    QMap<QString, int> map;
    map.insert("index", -1);
    map.insert("value", -1);
    if(!prefixValidate(prefix)) return map;

    QMap<int, int> prefixToSubnetMaskValue;

    prefixToSubnetMaskValue.insert(0, 0);
    prefixToSubnetMaskValue.insert(1, 128);
    prefixToSubnetMaskValue.insert(2, 192);
    prefixToSubnetMaskValue.insert(3, 224);
    prefixToSubnetMaskValue.insert(4, 240);
    prefixToSubnetMaskValue.insert(5, 248);
    prefixToSubnetMaskValue.insert(6, 252);
    prefixToSubnetMaskValue.insert(7, 254);
    prefixToSubnetMaskValue.insert(8, 255);

    if(prefix <= 8)
    {
        map.insert("index", 0);
        map.insert("value", prefixToSubnetMaskValue.value(prefix));
    }
    else if(prefix > 8 && prefix <= 16)
    {
        prefix = prefix - 8;
        map.insert("index", 1);
        map.insert("value", prefixToSubnetMaskValue.value(prefix));
    }
    else if(prefix > 16 && prefix <=24)
    {
        prefix = prefix - 16;
        map.insert("index", 2);
        map.insert("value", prefixToSubnetMaskValue.value(prefix));
    }
    else if(prefix > 24)
    {
        prefix = prefix - 24;
        map.insert("index", 3);
        map.insert("value", prefixToSubnetMaskValue.value(prefix));
    }

    return map;
}

QMap<QString, QString> NetTools::cidrSplit(QString ipPrefix)
{
    QMap<QString, QString> map; // ip:....   ; cidr: ...
    ipPrefix = ipPrefix.remove(" ");
    QStringList list = ipPrefix.split("/");
    map.insert("ip", QString(list.at(0)));
    map.insert("cidr", QString(list.at(1)));
    return map;
}

QMap<QString, int> NetTools::prefixAnalyze(int prefix, QString ipClass)
{
    QMap<QString, int> map, indexValue;
    if(!prefixValidate(prefix)) {map.insert("index", -1); return map;}
    indexValue = prefixIndexValue(prefix);

    map.insert("index", indexValue.value("index"));
    map.insert("maskValue", indexValue.value("value"));
    map.insert("hostPerSubnet", ( pow(2, (32-prefix) )) ); // 2^Z

    int maskValue = map.value("maskValue");
    if(maskValue == 255)
        map.insert("subnetCount", 1);
    else
        map.insert("subnetCount", (256 / (256 - maskValue)));

    if(ipClass.compare("A") == 0)
        map.insert("networkCount", (pow(2, (prefix-1)) - 2) );
    else if(ipClass.compare("B") == 0)
        map.insert("networkCount", pow(2, (prefix-2)) );
    else if(ipClass.compare("C") == 0)
        map.insert("networkCount", pow(2, (prefix-3)) );
    else
        map.insert("networkCount", -1 );

    return map;
}

QStandardItemModel *NetTools::subnetBroadcastMask(QString ip, int prefix)
{
    QString ipClass = ipClassIdentify(ip);
    QMap<QString, int> map = prefixAnalyze(prefix, ipClass);
    int row = map.value("subnetCount");
    int index = map.value("index");
    //int hostPerSub = map.value("hostPerSubnet");
    int maskValue = map.value("maskValue");
    QString mask = prefixToSubnetMask(prefix);
    QMap<int, int> ipOctet = ipMaskOctets(ip);
    QStandardItemModel *model = new QStandardItemModel(row , 3);
    model->setHorizontalHeaderLabels(QStringList() << "Network ID" << "Subnet Mask" << "BroadCast IP");
    QModelIndex inx;

    int NetId[4], BC[4];

    for(int ROW = 0; ROW < row; ROW++)
    {
        for(int OCTET = 0; OCTET < 4; OCTET++)
        {
            if(OCTET < index){ NetId[OCTET] = ipOctet.value(OCTET) ; BC[OCTET] = ipOctet.value(OCTET);}
            else if(OCTET == index)
            {
                NetId[OCTET] = (maskValue == 255)? ipOctet.value(OCTET) : (256 - maskValue) * ROW;
                BC[OCTET]    = (maskValue == 255)? ipOctet.value(OCTET) : (256 - maskValue) * (ROW+1)-1;
            }
            else if(OCTET > index) {NetId[OCTET] = 0; BC[OCTET] = 255;}
        }

        inx = model->sibling(ROW, 0, QModelIndex());
        model->setData(inx, QVariant(octetsToIp(NetId[0], NetId[1], NetId[2], NetId[3]) + " /" + QString::number(prefix)));
        inx = model->sibling(ROW, 1, QModelIndex());
        model->setData(inx, QVariant(mask));
        inx = model->sibling(ROW, 2, QModelIndex());
        model->setData(inx, QVariant(octetsToIp(BC[0], BC[1], BC[2], BC[3])));
    }

    return model;
}

QMap<int, QString> NetTools::ipSplit(QString ip, int prefix, bool CIDRdisplay) //   /28==> 2*/29
{
    QMap<QString, int> indexValue = prefixIndexValue(prefix);
    int netStep = 256 - indexValue.value("value");
    int index = indexValue.value("index");
    QMap<int, int> octets = ipMaskOctets(ip);
    octets[index] = octets[index] & indexValue.value("value");// netID
    for(int i = (index+1); i < 4; i++)
        octets[i] = 0;

    if(prefix ==  8){index = 1; netStep = 256;}
    if(prefix == 16){index = 2; netStep = 256;}
    if(prefix == 24){index = 3; netStep = 256;}

    QMap<int, QString> map;
    if(CIDRdisplay)
    {
        map.insert(0, octetsToIp(octets[0], octets[1], octets[2], octets[3]) + "/" + QString::number(prefix+1));

        octets[index] = octets[index] + netStep/2;
        map.insert(1, octetsToIp(octets[0], octets[1], octets[2], octets[3]) + "/" + QString::number(prefix+1));
    }
    else
    {
        map.insert(0, octetsToIp(octets[0], octets[1], octets[2], octets[3]));

        octets[index] = octets[index] + netStep/2;
        map.insert(1, octetsToIp(octets[0], octets[1], octets[2], octets[3]));
    }


    return map;
}

void NetTools::ipSplits(QStandardItemModel *model, QString ip, int prefix)
{
    ip = netId(ip, prefix);
    QStandardItem *root = model->invisibleRootItem();
    QList<QStandardItem *> list;
    list.insert(0, new QStandardItem(ip + "/" +QString::number(prefix)));
    root->appendRow(list.at(0));

    if(prefix > 29 || prefix < 4) return;

    int splitLayerCount;
    if(prefix < 30 && prefix >= 24) splitLayerCount = 30 - prefix;
    else if(prefix < 24 && prefix >= 16) splitLayerCount = 24 - prefix;
    else if(prefix < 16 && prefix >= 8 ) splitLayerCount = 16 - prefix;
    else if(prefix < 8  && prefix >  0 ) splitLayerCount = 8  - prefix;
    else return;

    if(splitLayerCount > 4) splitLayerCount = 4;

    QMap<int, QString> map;
    QMap<QString, QString> temp;
    for(int l = 1; l <= splitLayerCount; l++ )
    {
        switch (l)
        {
        case 1:
            map = ipSplit(ip, prefix, true);
            list.insert(1, new QStandardItem(map.value(0)));
            list.insert(2, new QStandardItem(map.value(1)));
            list.at(0)->appendRow(list.at(1));
            list.at(0)->appendRow(list.at(2));
            list.at(1)->setForeground(Qt::darkMagenta);
            list.at(2)->setForeground(Qt::darkMagenta);
            break;
        case 2:
            for(int i = 1; i <=2; i++)
            {
                ip = list.at(i)->text();
                temp = cidrSplit(ip);
                ip = temp.value("ip");
                prefix = temp.value("cidr").toInt();
                map = ipSplit(ip, prefix, true);
                list.insert((i*2+1), new QStandardItem(map.value(0)));
                list.insert((i*2+2), new QStandardItem(map.value(1)));
                list.at(i)->appendRow(list.at(i*2+1));
                list.at(i*2+1)->setForeground(Qt::darkBlue);
                list.at(i)->appendRow(list.at(i*2+2));
                list.at(i*2+2)->setForeground(Qt::darkBlue);
            }

            break;
        case 3:
            for(int i = 3; i <= 6; i++)
            {
                ip = list.at(i)->text();
                temp = cidrSplit(ip);
                ip = temp.value("ip");
                prefix = temp.value("cidr").toInt();
                map = ipSplit(ip, prefix, true);
                list.insert((i*2+1), new QStandardItem(map.value(0)));
                list.insert((i*2+2), new QStandardItem(map.value(1)));
                list.at(i)->appendRow(list.at(i*2+1));
                list.at(i)->appendRow(list.at(i*2+2));
                list.at(i*2+1)->setForeground(Qt::darkCyan);
                list.at(i*2+2)->setForeground(Qt::darkCyan);

            }
            break;
        case 4:
            for(int i = 7; i <= 14; i++)
            {
                ip = list.at(i)->text();
                temp = cidrSplit(ip);
                ip = temp.value("ip");
                prefix = temp.value("cidr").toInt();
                map = ipSplit(ip, prefix, true);
                list.insert((i*2+1), new QStandardItem(map.value(0)));
                list.insert((i*2+2), new QStandardItem(map.value(1)));
                list.at(i)->appendRow(list.at(i*2+1));
                list.at(i)->appendRow(list.at(i*2+2));
                list.at(i*2+1)->setForeground(Qt::darkYellow);
                list.at(i*2+2)->setForeground(Qt::darkYellow);
            }
            break;
        }
    }
}

QString NetTools::adjacentNetworkIp(QString net,const int prefix)
{
    net = netId(net, prefix);

    QMap<QString, int> indexValue = prefixIndexValue(prefix);

    QMap<int,int> netOctets = ipMaskOctets(net);
    int index = indexValue.value("index");
    int val = indexValue.value("value");
    val = (256 - val) + netOctets.value(index);
    if(val > 255) return "-1";

    netOctets.insert(index,val);
    net = octetsToIp(netOctets.value(0),netOctets.value(1),netOctets.value(2),netOctets.value(3));

    return net;
}

bool NetTools::isPrivateIp(const QString ip, const int prefix)
{
    QString net = netId(ip, prefix);
    QMap<int, int> octets = ipMaskOctets(net);
    int O0 = octets.value(0);
    int O1 = octets.value(1);

    if(O0 == 10) return true;

    if( O0 == 172 )
    {
        if((O1 >= 16) && (O1 <= 32) )
            return true;
    }

    if( (O0 == 192) && (O1 == 168) )
        return true;

    return false;
}

QStringList NetTools::bundles(QString ip, int prefix)
{
    QStringList list;
    QMap<QString, int> indexValue = prefixIndexValue(prefix);
    int index = indexValue.value("index");
    int value = indexValue.value("value");
    int netHop = 256 - value;

    ip = NetTools::netId(ip, prefix);
    QMap<int, int> octets = ipMaskOctets(ip);
    int start = octets.value(index);

    QString n;
    int IP[4];
    for(int j = 0; j < 4; j++)
        IP[j] = octets[j];

    for(int i = start+netHop; i < 256 ; i = i+netHop)
    {
        IP[index] = i;
        n = octetsToIp(IP[0], IP[1], IP[2], IP[3]) + "/"+QString::number(prefix);

        list << n;
    }

    return list;
}







