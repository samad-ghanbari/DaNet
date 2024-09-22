#ifndef EDITPOSNODIALOG_H
#define EDITPOSNODIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditPosNoDialog;
}

class EditPosNoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditPosNoDialog(QWidget *parent , DanetDbMan *db, QString AreaAbbr,int saloon, int PosId, int OdfNo, int PosNo);
    ~EditPosNoDialog();

private slots:
    void on_cancelbtn_clicked();
    void on_modifyBtn_clicked();

private:
    Ui::EditPosNoDialog *ui;
    DanetDbMan *dbMan;
    int posId;
};

#endif // EDITPOSNODIALOG_H
