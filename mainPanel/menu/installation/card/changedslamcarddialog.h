#ifndef CHANGEDSLAMCARDDIALOG_H
#define CHANGEDSLAMCARDDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class ChangeDslamCardDialog;
}

class ChangeDslamCardDialog : public QDialog
{
    Q_OBJECT
public:
    //this, dbMan, abbr, devexId, device,clickedSlotsId, clickedCard,clickedShelf, clickedSlot, clickedSubSlot
    explicit ChangeDslamCardDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const int DevexId, const QString device, const int SlotsId, const int cardId, const int shelf, const int slot, const int subSlot);
    ~ChangeDslamCardDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_cardCB_currentIndexChanged(int index);

private:
    Ui::ChangeDslamCardDialog *ui;
    DanetDbMan *dbMan;
    const int devexId,slotsId;
};

#endif // CHANGEDSLAMCARDDIALOG_H
