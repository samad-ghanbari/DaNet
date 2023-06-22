#ifndef EDITODFDIALOG_H
#define EDITODFDIALOG_H

#include <QDialog>

class DanetDbMan;


namespace Ui {
class EditOdfDialog;
}

class EditOdfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditOdfDialog(QWidget *parent, DanetDbMan *db,const int exchId,const QString AreaAbbr,const int Saloon,const int row, const int column,const int OdfId,const int OdfNo,const int PosCount,const int ppp);
    ~EditOdfDialog();
    bool changedOrNot(){return changed;}
    int getSaloon(){return saloon;}
    void checkEntry();

private slots:
    void on_cancelBtn_clicked();
    void on_modifyBtn_clicked();

    void on_saloonCB_currentIndexChanged(int index);

    void on_odfRowSB_valueChanged(int arg1);

    void on_odfColumnSB_valueChanged(int arg1);

    void on_odfNoSB_valueChanged(int arg1);

    void on_posCountSB_valueChanged(int arg1);

    void on_pppCB_currentIndexChanged(int index);

private:
    Ui::EditOdfDialog *ui;
    DanetDbMan *dbMan;
    int odfId, saloon;
    bool changed;
};

#endif // EDITODFDIALOG_H
