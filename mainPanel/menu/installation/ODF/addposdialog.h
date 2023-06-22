#ifndef ADDPOSDIALOG_H
#define ADDPOSDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AddPosDialog;
}

class AddPosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPosDialog(QWidget *parent,DanetDbMan *db, QString AreaAbbr,int saloon, int OdfId, int OdfNo);
    ~AddPosDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::AddPosDialog *ui;
    DanetDbMan *dbMan;
    const int odfId;
};

#endif // ADDPOSDIALOG_H
