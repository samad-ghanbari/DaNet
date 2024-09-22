#ifndef LOCALDEVICEDIALOG_H
#define LOCALDEVICEDIALOG_H

#include <QDialog>
class DanetDbMan;
class QSqlQueryModel;
class QStringListModel;

namespace Ui {
class LocalDeviceDialog;
}

class LocalDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LocalDeviceDialog(QWidget *parent, DanetDbMan *db, const int deviceId, QString Abb1, QString Saloon, QString Device, const int DevexId);
    ~LocalDeviceDialog();
    void fillVlanTV();
    void fillEtherTrunkTV();
    void fillVpnViews();
    void drawASR9010();
    void drawASR1002();
    void drawASR1013();
    void drawC7613();
    void drawCX200();
    void drawME60X16();
    void drawME60X8();
    void drawNE40X16();
    void drawNE40X8();
    void drawCX600X16();
    void drawCX600X8();
    void drawNE5000X16();
    void drawNE5000X16A();
    void drawS2403H();
    void drawS5300();
    void drawS9303();
    void drawS9306();


private slots:
    void on_okBtn_clicked();

    void on_vpnTV_clicked(const QModelIndex &index);

private:
    Ui::LocalDeviceDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *vlanModel,*etherModel, *vpnModel, *slotModel;
    const int devexId;
    QStringListModel *expModel, *impModel;

    QString abbr, device, saloon;

};

#endif // LOCALDEVICEDIALOG_H
