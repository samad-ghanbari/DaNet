#ifndef ODFMANDIALOG_H
#define ODFMANDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QMenu>

class DanetDbMan;
class CenterAlignedModel;
class AddOdfDialog;
class RemoveOdfDialog;
class EditOdfDialog;
class AddPosDialog;
class RemovePosDialog;
class EditPosNoDialog;

namespace Ui {
class OdfManDialog;
}

class OdfManDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OdfManDialog(QWidget *parent, DanetDbMan *db);
    ~OdfManDialog();

private:

    Ui::OdfManDialog *ui;
    DanetDbMan *dbMan;
    CenterAlignedModel *odfModel,*posModel;
    AddOdfDialog *addOdfDialog;
    RemoveOdfDialog *removeOdfDialog;
    EditOdfDialog *editOdfDialog;
    AddPosDialog *addPosDialog;
    RemovePosDialog *removePosDialog;
    EditPosNoDialog *editPosNoDialog;
    QMenu odfContextMenu, posContextMenu;
    QString mother;
    int exchSiteId;

    //
    int odfId,row, column, odfNo, posCount, ppp, posId, posNo, clickedRow;

private slots:
    void removeRefreshSlot(int i);
    void addRefreshSlot(int i);
    void editRefreshSlot(int i);
    void refreshPosSlot(int i);
    void fillOdfTV(int exchSiteId, int saloon);
    void fillPosTV(int i);
    void on_typeCB_currentIndexChanged(int index);
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(const QString &arg1);
    void on_siteCB_currentIndexChanged(int index);
    void odfTV2ClickSlot(QModelIndex index);
    void odfContextMenuSlot(QPoint ptr);
    void posContextMenuSlot(QPoint ptr);

    void on_actionRemovePOS_triggered();
    void on_odfAddBtn_clicked();
    void on_actionRemoveODF_triggered();
    void on_actionEditODF_triggered();
    void on_posAddBtn_clicked();
    void on_actionChangePosNo_triggered();
    void on_odfNoCB_currentIndexChanged(int index);
    void on_saloonCB_currentIndexChanged(int index);
};

#endif // ODFMANDIALOG_H
