#ifndef EDITCARDDESCDIALOG_H
#define EDITCARDDESCDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditCardDescDialog;
}

class EditCardDescDialog : public QDialog
{
    Q_OBJECT
public:
    //this, dbMan, abbr, devexId, device,clickedSlotsId, clickedCard,clickedShelf, clickedSlot, clickedSubSlot
    explicit EditCardDescDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const int DevexId, const QString device,const int SlotsId, const QString card, const int shelf, const int slot, const int subSlot, const QString desc);
    ~EditCardDescDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_descTE_textChanged();

private:
    Ui::EditCardDescDialog *ui;
    DanetDbMan *dbMan;
    const int slotsId;
};

#endif // EDITCARDDESCDIALOG_H
