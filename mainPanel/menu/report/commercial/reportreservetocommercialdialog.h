#ifndef REPORTRESERVETOCOMMERCIALDIALOG_H
#define REPORTRESERVETOCOMMERCIALDIALOG_H

#include <QDialog>
#include <QMap>

class DanetDbMan;

namespace Ui {
class ReportReserveToCommercialDialog;
}

class ReportReserveToCommercialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportReserveToCommercialDialog(QWidget *parent, DanetDbMan *db);
    ~ReportReserveToCommercialDialog();
    void emptyForm();
    void checkEntry();
    QList<int> getInterfacesId();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_deviceCB_currentIndexChanged(int index);
    void on_shelfCB_currentIndexChanged(int index);
    void on_slotCB_currentIndexChanged(int index);
    void on_subSlotCB_currentIndexChanged(int index);

    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::ReportReserveToCommercialDialog *ui;
    DanetDbMan *dbMan;
    QMap<QString, int> equType, intTypeCode,ModuleType, project, networkType;
    int portStatus, slotCount, startShelf,startSlot,startSubslot;

};

#endif // REPORTRESERVETOCOMMERCIALDIALOG_H
