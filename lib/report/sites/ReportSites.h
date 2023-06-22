#ifndef REPORTSITES_H
#define REPORTSITES_H

#include <QObject>
#include <QPen>
#include <QFont>
#include <QMap>
#include <QPrinter>

class QAbstractItemModel;
class QPainter;

class ReportSites : public QObject
{
    Q_OBJECT
public:
  ReportSites(QObject *parent, QAbstractItemModel *Model, const QString OutputPath, const QString MotherAbbr, const QString Mother, const int PaperSize, const int Compressfactor, const int Orient, const QFont SiteNameFont, const QString CustomInfo="");
  ReportSites(QObject *parent, QAbstractItemModel *Model, const QString OutputPath, const QString MotherAbbr, const QString Mother, const int PaperSize, const int Compressfactor, const int Orient, const QFont SiteNameFont, const QList<int> RemoveSiteCN, const QString CustomInfo="");
  ~ReportSites();

  QString lastError();
  QString stringToVerticalString(QString string);
  void preparePage(QPainter *painter);
  bool print();
  void drawSite(QPainter *painter, const QString abbr, const QString siteName, const QString dev, const QString ip, const QString pppoe, const bool removeFlag=false);
  int getSubSiteCount(int uplinkId);
  void fillSubSiteMap(const int cascade);
  int getNode(const int uplinkId);

private:
  QPainter *painter;
  const bool RemoveSiteFlag;
  const QAbstractItemModel *model;
  const QString outputPath, mother, motherAbbr;
  QPrinter::PaperSize paperSize;
  QPrinter::Orientation orient;
  QPen pen;
  QFont siteNameFont;

  // margins
  int headerHeight;
  int bottomHeight;
  int leftMargin;
  int rightMargin;

  int exchHeight;
  int siteRectWidth;

  double compressFactor;

  int pageNumber, currentCascade, removeCascade, removeNode;
  QString currentDate;

  QMap<int, QList<int> > sitePosMap;//siteId, (x, y)
  QMap<int, int> subSiteMap;// up-id, nth sub; 1,4 > 4th sub; ... 1-2 > 2nd sub
  QMap<int, QList<int> > linePosMap; //subSite:1 > pos(x,y) ; subsite:2 > pos(x,y)


  QString error, customInfo;
  bool CUSTOM_INFO;
};

#endif // REPORTSITES_H
