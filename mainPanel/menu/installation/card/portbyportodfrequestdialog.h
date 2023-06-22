#ifndef PORTBYPORTODFREQUESTDIALOG_H
#define PORTBYPORTODFREQUESTDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class PortByPortOdfRequestDialog;
}

class PortByPortOdfRequestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PortByPortOdfRequestDialog(QWidget *parent,DanetDbMan *DbMan,const int ExchId,const QString Abbr,const int Saloon,const int DeviceId,const QString Device,const QString itf, const bool END_PORT, const QList<int> PinIdList);
    ~PortByPortOdfRequestDialog();
    void checkEntry();
    int getPinId();

private slots:
    void on_odfSaloonCB_currentIndexChanged(int index);

    void on_odfCB_currentIndexChanged(int index);

    void on_posCB_currentIndexChanged(int index);

    void on_pinCB_currentIndexChanged(int index);

    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::PortByPortOdfRequestDialog *ui;
    DanetDbMan *dbMan;
    const int exchId, deviceId;
    const QList<int> pinIdList;
    int pinId;
    bool canceled;
    QMap<int,int> odfMap, posMap;
};

#endif // PORTBYPORTODFREQUESTDIALOG_H
