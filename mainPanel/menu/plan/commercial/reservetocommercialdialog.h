#ifndef RESERVETOCOMMERCIALDIALOG_H
#define RESERVETOCOMMERCIALDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class ReserveToCommercialDialog;
}

class ReserveToCommercialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReserveToCommercialDialog(QWidget *parent, DanetDbMan *db);
    explicit ReserveToCommercialDialog(QWidget *parent, DanetDbMan *db,const int  AreaNo,const int ExchId,const int DevexId, const int IntId);
    ~ReserveToCommercialDialog();
    void emptyForm();
    void checkEntry();
    QList<int> getInterfacesId();
    bool interfaceReservability(QList<int> intIds);

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_deviceCB_currentIndexChanged(int index);
    void on_shelfCB_currentIndexChanged(int index);
    void on_slotCB_currentIndexChanged(int index);
    void on_subSlotCB_currentIndexChanged(int index);
    void on_portChB_toggled(bool checked);
    void on_startPortCB_currentIndexChanged(int index);
    void on_endPortCB_currentIndexChanged(int index);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_intTypeCB_currentIndexChanged(int index);

private:
    Ui::ReserveToCommercialDialog *ui;
    DanetDbMan *dbMan;
    bool shelfSupport, subSlotSupport;

    QMap<QString, int> equType, intTypeCode,ModuleType, project, networkType;
    int portStatus;


};

#endif // RESERVETOCOMMERCIALDIALOG_H
