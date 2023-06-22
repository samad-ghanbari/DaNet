#ifndef ODFMANAGEMENTDIALOG_H
#define ODFMANAGEMENTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QMenu>
#include <lib/centeralignedmodel.h>
class DanetDbMan;


namespace Ui {
class ODFManagementDialog;
}

class ODFManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ODFManagementDialog(QWidget *parent, DanetDbMan *db);
    explicit ODFManagementDialog(QWidget *parent, DanetDbMan *db,const bool mainDB, const int Area, const int AbbrId, const int OdfId, const int PosId, const int PinId);
    ~ODFManagementDialog();
    void fillTable(int posId);

private slots:
    void refreshSlot(int done);
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_saloonCB_currentIndexChanged(int index);
    void contextMenuSlot(QPoint ptr);
    void pinTV2ClickSlot(QModelIndex ind);
    void on_odfCB_currentIndexChanged(int index);
    void on_posCB_currentIndexChanged(int index);
    void on_actionAssignODFPin_triggered();
    void on_actionDepleteODFPin_triggered();
    void on_actionEditODFPin_triggered();


private:
    Ui::ODFManagementDialog *ui;
    DanetDbMan *dbMan;
    CenterAlignedModel *tableModel;
    const bool mainDb;

    QMap<int, QString> odfPPP;
    QMenu contextMenu;
    QModelIndex clickedIndex;
    //`id`,`port_id`,`local_device`,`local_label`,`pin_no`,`remote`,`remote_label`
    int clickedRow,clickedPinId, clickedPortId;
    QString clickedLocalDevice, clickedLocalLabel,clickedPinNo,
        clickedRemote, clickedRemoteLabel;

    bool SEARCH_FLAG;
    const int abbrId,odfId,posId,pinId;
    int exchId, siteId, selectRow, saloon;
};

#endif // ODFMANAGEMENTDIALOG_H
