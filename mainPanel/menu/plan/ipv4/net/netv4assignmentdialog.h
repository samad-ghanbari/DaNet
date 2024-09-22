#ifndef NETV4ASSIGNMENTDIALOG_H
#define NETV4ASSIGNMENTDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class NetV4AssignmentDialog;
}

class NetV4AssignmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetV4AssignmentDialog(QWidget *parent, DanetDbMan *db, const int NetPoolId, const QString net, const int prefix, const QString desc);
    ~NetV4AssignmentDialog();

private slots:
    void on_descLE_textChanged(const QString &arg1);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::NetV4AssignmentDialog *ui;
    DanetDbMan *dbMan;
    const int netPoolId;
    int host;
    QString mask,bc, Network;
};

#endif // NETV4ASSIGNMENTDIALOG_H
