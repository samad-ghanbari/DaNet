#ifndef INTERFACEREPORTDIALOG_H
#define INTERFACEREPORTDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QPoint>
#include <QMenu>
class DanetDbMan;
class QStandardItemModel;

namespace Ui {
class InterfaceReportDialog;
}

class InterfaceReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InterfaceReportDialog(QWidget *parent, DanetDbMan *db);
    ~InterfaceReportDialog();
    void checkAddBtn();
    void checkPreviewBtn();

private slots:
    void contextMenuSlot(QPoint ptr);
    void on_cancelBtn_clicked();
    void on_abbrCB_currentIndexChanged(int index);
    void on_devexCB_currentIndexChanged(int index);
    void on_deviceCB_currentIndexChanged(int index);
    void on_addBtn_clicked();
    void on_okBtn_clicked();
    void on_exchChB_toggled(bool checked);
    void on_deviceChB_toggled(bool checked);
    void on_areaCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_peerChB_toggled(bool checked);
    void on_peerLE_textChanged(const QString &arg1);
    void on_actionRemove_Record_triggered();

private:
    Ui::InterfaceReportDialog *ui;
    DanetDbMan *dbMan;
    QStandardItemModel *model;
    QMenu contextMenu;
    QModelIndex index;
    int clickedRow;
};

#endif // INTERFACEREPORTDIALOG_H
