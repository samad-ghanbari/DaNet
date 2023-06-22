#ifndef PORTSSTATDIALOG_H
#define PORTSSTATDIALOG_H

#include <QDialog>
class DanetDbMan;
class QStandardItemModel;

namespace Ui {
class PortsStatDialog;
}

class PortsStatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PortsStatDialog(QWidget *parent , DanetDbMan *db);
    ~PortsStatDialog();

private slots:
    void on_CancelBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_devCB_currentIndexChanged(int index);

private:
    Ui::PortsStatDialog *ui;
    DanetDbMan *dbMan;
    QStandardItemModel *model;

};

#endif // PORTSSTATDIALOG_H
