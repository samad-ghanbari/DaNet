#ifndef ADDDEVICECARDDIALOG_H
#define ADDDEVICECARDDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class AddDeviceCardDialog;
}

class AddDeviceCardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDeviceCardDialog(QWidget *parent, DanetDbMan *db, const int ExchId, const QString Abbr,const int DevexId, const QString Device, const bool AllSlots);
    ~AddDeviceCardDialog();
    void resetPortInfos();
    bool insertPort(int Type, int PortStart, int PortEnd, QList<int> pinIdList, int &index);
    bool insertPort(int Type, int PortStart, int PortEnd, bool NoODF);
    bool insertPort(int Type, int port,int pinId);
    void setPortNamingLable();
    bool portTypeSkipedOdf(const int portType);

private slots:

    void on_odfCB_currentIndexChanged(int index);
    void on_cardCB_currentIndexChanged(int index);
    void on_posCB_currentIndexChanged(int index);
    void confirmBtn();
    void on_okBtn_clicked();
    void on_eStartSB_valueChanged(int arg1);
    void on_gStartSB_valueChanged(int arg1);
    void on_g10StartSB_valueChanged(int arg1);
    void on_g40StartSB_valueChanged(int arg1);
    void on_g100StartSB_valueChanged(int arg1);
    void on_fillShelfBtn_clicked();
    void on_odfSaloonCB_currentIndexChanged(int index);
    void on_pinCB_currentIndexChanged(int index);
    void on_shelfChB_toggled(bool checked);
    void on_slotChB_toggled(bool checked);
    void on_subSlotChB_toggled(bool checked);
    void on_shelfSB_valueChanged(int arg1);
    void on_slotCB_currentIndexChanged(int index);
    void on_subslotSB_valueChanged(int arg1);

    int fiberPortCount();
    int electricalPortCount();

    void on_pbpChB_toggled(bool checked);

    void fiberElectricalCheck();

    void on_eODFChB_toggled(bool checked);

    void on_g1ODFChB_toggled(bool checked);

    void on_g10ODFChB_toggled(bool checked);

    void on_exchNextBtn_clicked();

    void on_posBackBtn_clicked();

    void on_posNextBtn_clicked();

    void on_portBackBtn_clicked();

    void on_portNextBtn_clicked();

    void on_odfBackBtn_clicked();

    void on_posOkBtn_clicked();

    void on_portOkBtn_clicked();

    void on_g40SkipODFChB_toggled(bool checked);

    void on_g100SkipODFChB_toggled(bool checked);

    void on_eSkipODFChB_toggled(bool checked);

    void on_g1SkipODFChB_toggled(bool checked);

    void on_g10SkipODFChB_toggled(bool checked);

private:
    Ui::AddDeviceCardDialog *ui;
    DanetDbMan *dbMan;
    const int devexId, exchId;
    const QString device;
    const bool allSlots;
    int saloon,deviceId,shelf, slot,slotsId, subSlot, slotCount,startShelf,startSlot,startSubslot;
    QMap<int, QString> cardMap;
    QMap<int, int> portCount,odfMap, posMap;
    bool SR, NoPort, pbpOdfAssign, odfNeeded;//switch or router?
    int index;

};

#endif // ADDDEVICECARDDIALOG_H
