#ifndef SUBNET1V4ASSIGNMENTDIALOG_H
#define SUBNET1V4ASSIGNMENTDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class Subnet1V4AssignmentDialog;
}

class Subnet1V4AssignmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Subnet1V4AssignmentDialog(QWidget *parent, DanetDbMan *db, const int SubnetPoolId, const QString net, const int prefix);
    explicit Subnet1V4AssignmentDialog(QWidget *parent, DanetDbMan *db, const int SubnetPoolId, const QString net, const int prefix, const int area, const int exchId,const int depNo, const QString ap, const QString desc);
    ~Subnet1V4AssignmentDialog();
    void entryCheck();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_descLE_textChanged(const QString &arg1);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_abbrCB_currentIndexChanged(int index);
    void on_appCB_currentIndexChanged(int index);
    void on_appLE_textChanged(const QString &arg1);

    void on_depCB_currentIndexChanged(int index);

private:
    Ui::Subnet1V4AssignmentDialog *ui;
    DanetDbMan *dbMan;
    const int subnetPoolId;
    QString Network;
};

#endif // SUBNET1V4ASSIGNMENTDIALOG_H
