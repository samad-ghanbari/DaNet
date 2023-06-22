#ifndef NEWSITEDSLAMDIALOG_H
#define NEWSITEDSLAMDIALOG_H

#include <QDialog>
#include <QWidget>
class DanetDbMan;

class AddExchangeSiteDialog;
class AddDeviceInstallDialog;
class CardInstallDialog;
class DslamDialog;

class NewSiteDslamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewSiteDslamDialog(QWidget *Parent, DanetDbMan *db);
    ~NewSiteDslamDialog();
    void startPlan();

private slots:
    void exchangeSlot(int confirm);
    void deviceSlot(int confirm);
    void cardSlot(int confirm);

private:
    DanetDbMan *dbMan;
    QWidget *parent;
    AddExchangeSiteDialog *addExchangeSiteDialog;
    AddDeviceInstallDialog *addDeviceInstallDialog;
    CardInstallDialog *cardInstallDialog;
    DslamDialog *dslamDialog;
    int area,type,motherId,siteId, cascade,node, devexId;
    QString site;
};

#endif // NEWSITEDSLAMDIALOG_H
