#ifndef NETTOOLS_H
#define NETTOOLS_H

#include <QObject>
class QStandardItemModel;

class NetTools : public QObject
{
    Q_OBJECT
public:
    explicit NetTools(QObject *parent);
    static QMap<QString, int> classAFirstOctetRange();
    static QMap<QString, int> classBFirstOctetRange();
    static QMap<QString, int> classCFirstOctetRange();
    static QMap<QString, int>classDFirstOctetRange();
    static QMap<QString, int> classEFirstOctetRange();

    static QString classAFirstOctetMSB();
    static QString classBFirstOctetMSB();
    static QString classCFirstOctetMSB();
    static QString classDFirstOctetMSB();
    static QString classEFirstOctetMSB();

    static QString ipClassIdentify(QString ip);

    static QString classADefaultSubnetMask();
    static QString classBDefaultSubnetMask();
    static QString classCDefaultSubnetMask();
    static QString classDDefaultSubnetMask();
    static QString classEDefaultSubnetMask();

    static QString classANumberOfNetwork();
    static QString classBNumberOfNetwork();
    static QString classCNumberOfNetwork();
    static QString classDNumberOfNetwork();
    static QString classENumberOfNetwork();

    static QString classAHostPerNetwork();
    static QString classBHostPerNetwork();
    static QString classCHostPerNetwork();
    static QString classDHostPerNetwork();
    static QString classEHostPerNetwork();

    static QString classAPrivateNetwork();
    static QString classBPrivateNetwork();
    static QString classCPrivateNetwork();
    static QString classDPrivateNetwork();
    static QString classEPrivateNetwork();

    static QString classAPrivateNetworkMask();
    static QString classBPrivateNetworkMask();
    static QString classCPrivateNetworkMask();
    static QString classDPrivateNetworkMask();
    static QString classEPrivateNetworkMask();

//_________________________________________________
    static bool ipv4Validate(QString ip);
    static bool ipv4PrefixValidate(const QString ipPrefix);
    static bool subnetMaskValidate(QString mask);
    static bool prefixValidate(int prefix);
    static QString netId(QString ip, int prefix);
    static QString broadcast(QString ip, int prefix);
    static QString netFirstIp(QString net, int prefix);
    static QString netLastIp(QString net, int prefix);
    static QMap<int, int> ipMaskOctets(QString ip);
    static QString octetsToIp(int octet0, int octet1, int octet2, int octet3);
    static int subnetMaskToPrefix(QString mask);
    static QString prefixToSubnetMask(int prefix);
    static int prefixHostCount(int prefix);
    static QMap<QString, int> prefixIndexValue(int prefix); // 28 => index:3 , value:240
    static QMap<QString, QString> cidrSplit(QString ipPrefix); // "192.168.1.1/24" > "192.168.1.1" "24"
    static QMap<QString, int> prefixAnalyze(int prefix, QString ipClass); // index, maskValue, hostPerSubnet,subnetCount, networkCount
    static QStandardItemModel *subnetBroadcastMask(QString ip, int prefix);
    static QMap<int, QString> ipSplit(QString ip, int prefix, bool CIDRdisplay);
    static void ipSplits(QStandardItemModel *model, QString ip, int prefix);
    static QString adjacentNetworkIp(QString net, const int prefix); // 192.168.1.0   25  > 192.168.1.128
    static bool isPrivateIp(const QString ip, const int prefix);
    static QStringList bundles(QString ip, int prefix);


};

#endif // NETTOOLS_H
