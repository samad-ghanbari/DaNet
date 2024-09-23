#ifndef REPORTDSLAM_H
#define REPORTDSLAM_H

#include <QObject>
#include <QModelIndex>
#include <QPen>
#include <QFont>

class QPrinter;
class QPainter;
class QAbstractItemModel;

class ReportDSLAM : public QObject
{
    Q_OBJECT
public:
  ReportDSLAM(QObject *Parent, const QString OutputPath, const bool HuaweiDslam, bool SingleAgg, const QString DslamName, const bool InterSwitchFlag, const QMap<int, QString> ServiceMap, const QMap<int, QStringList> NmsMap, QMap<QString, QString> PortMap, QStringList Agg1List, QStringList Agg2List, QStringList AggInfo, QStringList Bras1List, QStringList Bras2List, QMap<int, QString> BrasInfo, QStringList CxList, const QString CustomInfo = "");//, QStringList Agg1CxList
  ~ReportDSLAM();
  QString lastError();
  void preparePage();
  bool print();
  bool printTable(QPrinter *printer);
  QString uplinkIntraVlan(QString vlanRange, int vlanBase); //200-202 >>> 200:Uplink & 201-202: Intralink
  QString listToRange(QList<int> list);
  QList<int> rangeToList(QString range);
private:
  bool singleAgg;
  QPainter *painter;
  const QString outputPath;
  const QString dslamName;
  const bool interSwitchFlag;
  const QMap<int, QString> serviceMap;
  QMap<QString, QString> portMap;// "VAS", "100-102"     "0/7/0","To..."
  const QMap<int, QStringList> nmsMap;// 10, ip<<gw
  const QStringList agg1List, agg2List, aggInfo;// agg1 << ether <<port1<<port2 / vlan << desc
  const QStringList bras1List, bras2List;// bras << sw1 <<sw2
  const QMap<int, QString> brasInfo;// 500, "To..."
  const QStringList cxList;// cx << port1 << port2 <<vlan list << desc
  const QStringList agg1CxList; // <<ether << vlan

  QPen pen; // for table borders
  QFont headersFont, contentFont, topicFont;

  // cell margins
  int topMargin;
  int bottomMargin;
  int leftMargin;
  int rightMargin;
  bool newPageFlag;

  // margins for table
  int headerHeight;
  int bottomHeight;
  int leftBlank;
  int rightBlank;

  int tableHeaderHight;

  int pageNumber;
  QString current_date;
  QModelIndex index;
  QString error, customInfo;
  bool huaweiDslam, CUSTOM_INFO,UPLINK;
  int vlanOffset;




};

#endif // REPORTDSLAM_H
