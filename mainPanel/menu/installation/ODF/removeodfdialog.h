#ifndef REMOVEODFDIALOG_H
#define REMOVEODFDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveOdfDialog;
}

class RemoveOdfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveOdfDialog(QWidget *parent,DanetDbMan *db,int exchId, QString areaAbbr,int saloon, int odfId, int odfNo, int posCount, int ppp);
    ~RemoveOdfDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveOdfDialog *ui;
    DanetDbMan *dbMan;
    int saloon, exchId, odfId;
};

#endif // REMOVEODFDIALOG_H
