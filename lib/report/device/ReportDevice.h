#ifndef REPORTDEVICE_H
#define REPORTDEVICE_H

#include <QObject>
#include <QPen>
#include <QFont>
#include <QMap>
#include <QPrinter>

class QAbstractItemModel;
class QPainter;

class ReportDevice : public QObject
{
    Q_OBJECT
public:
    //this, model,dslamNms,dslampppoe, abbr,exchange,saloon, path,ps, orient,font
    ReportDevice(QObject *parent, QAbstractItemModel *Model,    const QString Device,const int SlotCount,const int StartSlot,const int DevexId, const QString DevexName, const QList<QString> exchInfo, const QString Saloon, const QString OutputPath, const int PaperSize, const int Orient, const QFont Font, const QString CustomInfo="");
    ReportDevice(QObject *parent, QAbstractItemModel *AllModel, const QString Device,const int SlotCount,const int StartSlot, const QMap<int, int> shelfDevexId, const QList<QString> DslamRackNames, const QList<QString> DslamRackNms, const QList<QString> DslamRackPppoe, const QList<QString> exchInfo, const QString Saloon, const QString OutputPath, const int PaperSize, const int Orient, const QFont Font, const QString CustomInfo="");
    ~ReportDevice();
    void init(const int PaperSize, const int Orient, const QFont Font);
    int getSubSlotCount(const int DevexId,const int Slot, const bool checkSubSlot = false);
    QString lastError();
    QString stringToVerticalString(QString string);//mpu > m\np\nu
    void preparePage();
    bool print();
    void paintDslam16SlotDevice();
    void paintDslam16SlotShelf(const int shelfNo);
    void paintMA5616Device();
    void paintS9303Device();
    void paintS9306Device();
    void paintS5300Device();
    void paintNE40X16Device();
    void paintNE40X8Device();
    void paintNE5000X16ADevice();
    void paintNE5000X16Device();
    void paint7613Device();
    void paintASR9010Device();
    void paintASR1013Device();
    void paintASR1002XDevice();
    void paintUnkownDevice();

private:
    QPainter *painter;
    const QAbstractItemModel *model;
    const QString outputPath, abbr, exchange, address,saloon, device;
    const int devexId,slotCount,startSlot;
    const bool DslamDevice;
    QString devexName;
    QPrinter::PaperSize paperSize;
    QPrinter::Orientation orient;
    QPen pen;
    QFont font, headerFont;

    // margins
    int headerHeight;
    int bottomHeight;
    int leftMargin;
    int rightMargin;

    double rackHeight, rackWidth, shelfHeaderHeight, shelfBottomHeight, shelfHeight, slotWidth, slotHeight, slotGap;

    int pageNumber;
    QString currentDate;

    QList<QString> dslamRackName, dslamRackNms,dslamRackPppoe;
    QMap<int, int> dslamDevexId; // shelf-devexId;

    bool CUSTOM_INFO;
    QString error, customInfo;
};

#endif // REPORTDEVICE_H
