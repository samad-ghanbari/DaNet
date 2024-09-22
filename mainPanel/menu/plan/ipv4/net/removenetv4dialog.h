#ifndef REMOVENETV4DIALOG_H
#define REMOVENETV4DIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveNetV4Dialog;
}

class RemoveNetV4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveNetV4Dialog(QWidget *parent, DanetDbMan *db, const int NetPoolId, const QString net, const int prefix, const QString mask, const QString bc, const int host);
    ~RemoveNetV4Dialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveNetV4Dialog *ui;
    DanetDbMan *dbMan;
    const int netPoolId;
    int subnetPoolId;
    QString Network;
};

#endif // REMOVENETV4DIALOG_H
