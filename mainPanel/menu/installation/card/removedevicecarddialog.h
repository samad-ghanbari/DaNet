#ifndef REMOVEDEVICECARDDIALOG_H
#define REMOVEDEVICECARDDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveDeviceCardDialog;
}

class RemoveDeviceCardDialog : public QDialog
{
    Q_OBJECT
public:
    //this, dbMan, abbr, devexId, device,clickedSlotsId, clickedCard,clickedShelf, clickedSlot, clickedSubSlot
    explicit RemoveDeviceCardDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const int DevexId, const QString device,const int SlotsId, const QString card, const int shelf, const int slot, const int subSlot);
    explicit RemoveDeviceCardDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const int DevexId, const QString device);
    ~RemoveDeviceCardDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_checkBox_toggled(bool checked);
    void on_removeAllCardsBtn_clicked();

private:
    Ui::RemoveDeviceCardDialog *ui;
    DanetDbMan *dbMan;
    const int devexId,slotsId;
    const bool all;
};

#endif // REMOVEDEVICECARDDIALOG_H
