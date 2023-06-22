#ifndef SUBNET2V4ASSIGNMENTDIALOG_H
#define SUBNET2V4ASSIGNMENTDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class Subnet2V4AssignmentDialog;
}

class Subnet2V4AssignmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Subnet2V4AssignmentDialog(QWidget *parent, DanetDbMan *db, const int SubnetPool2Id, const QString net, const int prefix);
    explicit Subnet2V4AssignmentDialog(QWidget *parent, DanetDbMan *db, const int SubnetPool2Id, const QString net, const int prefix, const int area, const int exchId, const QString ap, const QString desc);
    ~Subnet2V4AssignmentDialog();
    void entryCheck();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_descLE_textChanged(const QString &arg1);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_abbrCB_currentIndexChanged(int index);
    void on_appCB_currentIndexChanged(int index);
    void on_appLE_textChanged(const QString &arg1);

private:
    Ui::Subnet2V4AssignmentDialog *ui;
    DanetDbMan *dbMan;
    const int subnetPool2Id;
    QString Network;
};

#endif // SUBNET2V4ASSIGNMENTDIALOG_H
