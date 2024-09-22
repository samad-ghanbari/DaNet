#ifndef CARDINSTALLDIALOG_H
#define CARDINSTALLDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QMenu>
class QSqlQuery;
class QSqlQueryModel;
class DanetDbMan;

namespace Ui {
class CardInstallDialog;
}

class CardInstallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CardInstallDialog(QWidget *parent, DanetDbMan *db);
    explicit CardInstallDialog(QWidget *parent, DanetDbMan *db, const int area,const int ExchId,const int type, const int SiteId, const int DevexId);
    ~CardInstallDialog();
    void init();
    void fillSlotsTV(const int devexId);
    void BtnMan(bool FillBtn, bool DepleteBtn, bool addBtn );
    void paintDevice(const QString device);

    void drawASR9010();
    void drawASR1013();
    void drawASR1002();
    void drawC7613();
    void drawCX200();
    void drawME60X16();
    void drawME60X8();
    void drawNE40X16();
    void drawNE40X8();
    void drawCX600X16();
    void drawCX600X8();
    void drawNE5000X16A();
    void drawNE5000X16();
    void drawS2403H();
    void drawS5300();
    void drawS9303();
    void drawS9306();
    void drawDslam4s();
    void drawdslam16s(const QString DeviceName);

private slots:
    void refreshSlot(int done);
    void on_areaCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_devexCB_currentIndexChanged(int index);
    void slotContextMenuSlot(QPoint ptr);
    void on_emptyDslamBtn_clicked();
    void on_fillDslamBtn_clicked();
    void on_abbrCB_currentTextChanged(const QString &arg1);
    void on_addCardBtn_clicked();
    void on_actionRemove_Card_triggered();
    void on_actionEdit_Purchase_triggered();
    void on_actionEdit_Description_triggered();
    void on_actionchange_Card_Type_triggered();

private:
    Ui::CardInstallDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *slotModel, *deviceModel;
    QMenu slotContextMenu;
    QString mother;
    int exchSiteId;
    int clickedRow,clickedSlotsId, clickedDevexId, clickedCardId, clickedShelf, clickedSlot,
    clickedSubSlot;
    QString clickedCard, clickedPurchase, clickedDesc;
};

#endif // CARDINSTALLDIALOG_H
