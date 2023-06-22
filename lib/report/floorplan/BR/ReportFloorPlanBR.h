#ifndef REPORTFLOORPLANBR_H
#define REPORTFLOORPLANBR_H

#include <QObject>
#include <QPen>
#include <QFont>
#include <QMap>
#include <QPrinter>

class QAbstractItemModel;
class QPainter;

class ReportFloorPlanBR : public QObject
{
    Q_OBJECT
public:
  ReportFloorPlanBR(QObject *parent, QAbstractItemModel *Model, const QString Abbr, const QString Exchange, const QString Saloon, const QString OutputPath, const int PaperSize, const int Orient, const QFont Font, const QString CustomInfo="");
  ~ReportFloorPlanBR();
  QString lastError();
  void preparePage();
  bool print();
  int getStartRackNo();// minimum rack no for all rows
  QMap<int, QList<QString>> getrackInfo();
  void drawRack(const int rackRow,const int rackNo, const int shelfCount, const QList<QString> devices);
  int odfCount(const int rackRow,const int rackNo); //odf count before this this position
  void drawOdf(const int rackRow,const int rackNo,const QString device, const int odfNo, const int ppp);
  void drawRackRow(const int rackRow);

private:
  QPainter *painter;
  const QAbstractItemModel *model;
  const QString outputPath;
  const QString abbr, exchange, saloon;
  QPrinter::PaperSize paperSize;
  QPrinter::Orientation orient;
  QPen pen;
  QFont font;

  // margins
  int headerHeight;
  int bottomHeight;
  int leftMargin;
  int rightMargin;

  int pageNumber, rackHeight, rackWidth,odfWidth, rackGap, SRN, currentRackRow, modelRow;//Start Rack NO : SRN
  QString currentDate, error, customInfo;
  bool startRackRow, CUSTOM_INFO;

};

#endif // REPORTFLOORPLANBR_H