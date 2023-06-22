#ifndef SUBNET3V4ASSIGNMENTDIALOG_H
#define SUBNET3V4ASSIGNMENTDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class Subnet3V4AssignmentDialog;
}

class Subnet3V4AssignmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Subnet3V4AssignmentDialog(QWidget *parent, DanetDbMan *db, const int SubnetPoolId, const QString subnet, const int prefix);
    explicit Subnet3V4AssignmentDialog(QWidget *parent, DanetDbMan *db, const int SubnetPoolId, const QString subnet, const int prefix, const int area, const int exchId, const QString ap, const QString desc);
    ~Subnet3V4AssignmentDialog();
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
    Ui::Subnet3V4AssignmentDialog *ui;
    DanetDbMan *dbMan;
    const int subnetPoolId;
    QString Network;
};

#endif // SUBNET3V4ASSIGNMENTDIALOG_H
