#ifndef ODFPINREQUESTDIALOG_H
#define ODFPINREQUESTDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class OdfPinRequestDialog;
}

class OdfPinRequestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OdfPinRequestDialog(QWidget *parent, DanetDbMan *Db, const int ExchId, const QList<int> PinIdList, const QString exch, const int saloon);
    ~OdfPinRequestDialog();
    void checkEntry();
    int getOdfNo();
    int getPpp();
    int getPosNo();
    int getPinId();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_odfSaloonCB_currentIndexChanged(int index);
    void on_odfCB_currentIndexChanged(int index);
    void on_posCB_currentIndexChanged(int index);
    void on_pinCB_currentIndexChanged(int index);

private:
    Ui::OdfPinRequestDialog *ui;
    DanetDbMan *dbMan;
    const int exchId;
    const QList<int> pinIdList;
    QMap<int,int> odfMap, posMap;
    int odfNo,ppp,posNo,pinId;
};

#endif // ODFPINREQUESTDIALOG_H
