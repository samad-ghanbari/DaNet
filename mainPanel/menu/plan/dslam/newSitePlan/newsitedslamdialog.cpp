#include "newsitedslamdialog.h"
#include "lib/database/danetdbman.h"
#include "mainPanel/menu/installation/exchangeSite/addexchangesitedialog.h"
#include "mainPanel/menu/installation/device/adddeviceinstalldialog.h"
#include "mainPanel/menu/installation/card/cardinstalldialog.h"
#include "mainPanel/menu/plan/dslam/dslamdialog.h"

NewSiteDslamDialog::NewSiteDslamDialog(QWidget *Parent, DanetDbMan *db) :
    QDialog(Parent),
    dbMan(db),
    parent(Parent),
    addExchangeSiteDialog(nullptr),
    addDeviceInstallDialog(nullptr),
    cardInstallDialog(nullptr),
    dslamDialog(nullptr)
{
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

}

NewSiteDslamDialog::~NewSiteDslamDialog()
{
}

void NewSiteDslamDialog::startPlan()
{
    //##########   Add Site
    delete addExchangeSiteDialog;
    addExchangeSiteDialog = new AddExchangeSiteDialog(parent,dbMan, true);
    connect(addExchangeSiteDialog, SIGNAL(finished(int)), this, SLOT(exchangeSlot(int)));
    addExchangeSiteDialog->show();
    addExchangeSiteDialog->activateWindow();
}

void NewSiteDslamDialog::exchangeSlot(int confirm)
{
    confirm++;
    if(addExchangeSiteDialog->isDone())
    {
        QList<QVariant> list = dbMan->getExchangeInfo(addExchangeSiteDialog->getAbbr(), addExchangeSiteDialog->getMotherId());
        //`id`, `area`, `exchange`, `type`, `mother_exchange_id`, `site_cascade`, `site_node`
        siteId = list[0].toInt();
        area = list[1].toInt();
        site = list[2].toString();
        type = list[3].toInt();
        motherId = list[4].toInt();
        cascade = list[5].toInt();
        node = list[6].toInt();

        if(type != 3) return;
        //######### Add Device
        delete addDeviceInstallDialog;
        addDeviceInstallDialog = new AddDeviceInstallDialog(parent,dbMan,siteId,site,true);
        connect(addDeviceInstallDialog, SIGNAL(finished(int)), this, SLOT(deviceSlot(int)));
        addDeviceInstallDialog->show();
        addDeviceInstallDialog->activateWindow();

    }
    else
        this->close();
}

void NewSiteDslamDialog::deviceSlot(int confirm)
{
    confirm++;

    if(addDeviceInstallDialog->isDone())
    {
        devexId = addDeviceInstallDialog->getDevexId();

        //######## Add Card
        delete cardInstallDialog;
        cardInstallDialog = new CardInstallDialog(parent,dbMan, area, motherId, type, siteId, devexId);
        connect(cardInstallDialog, SIGNAL(finished(int)), this, SLOT(cardSlot(int)));
        cardInstallDialog->showMaximized();
        cardInstallDialog->activateWindow();
    }
    else
        this->close();
}

void NewSiteDslamDialog::cardSlot(int confirm)
{
    confirm++;

    //######## Plan DSLAM
    delete dslamDialog;
    dslamDialog = new DslamDialog(this,dbMan, area, motherId, type, siteId, devexId);
    connect(dslamDialog, SIGNAL(finished(int)), this, SIGNAL(finished(int)));
    dslamDialog->showMaximized();
    dslamDialog->activateWindow();
}

