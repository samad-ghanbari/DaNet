#ifndef LOCALNEWDSLAMDIALOG_H
#define LOCALNEWDSLAMDIALOG_H

#include <QDialog>
#include <QWidget>


class AddDeviceInstallDialog;
class CardInstallDialog;
class DslamDialog;

class DanetDbMan;

namespace Ui {
class LocalNewDslamDialog;
}

class LocalNewDslamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LocalNewDslamDialog(QWidget *Parent, DanetDbMan *db);
    ~LocalNewDslamDialog();

private slots:
    void deviceSlot(int DONE);
    void cardSlot(int done);
    void on_okBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);

private:
    Ui::LocalNewDslamDialog *ui;
    DanetDbMan *dbMan;
    QWidget *parent;
    AddDeviceInstallDialog *addDeviceInstallDialog;
    CardInstallDialog *cardInstallDialog;
    DslamDialog *dslamDialog;
    int area,exchId, devexId;
};

#endif // LOCALNEWDSLAMDIALOG_H
