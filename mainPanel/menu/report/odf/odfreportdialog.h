#ifndef ODFREPORTDIALOG_H
#define ODFREPORTDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QPoint>
#include <QMenu>
class DanetDbMan;
class QStandardItemModel;

namespace Ui {
class OdfReportDialog;
}

class OdfReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OdfReportDialog(QWidget *parent, DanetDbMan *db);
    ~OdfReportDialog();
    void checkAddBtn();
    void checkPreviewBtn();

private slots:
    void contextMenuSlot(QPoint ptr);
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_odfChB_toggled(bool checked);
    void on_odfCB_currentIndexChanged(int index);
    void on_posChB_toggled(bool checked);
    void on_posCB_currentIndexChanged(int index);
    void on_peerChB_toggled(bool checked);
    void on_peerLE_textChanged(const QString &arg1);
    void on_addBtn_clicked();
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_posSB_valueChanged(int arg1);
    void on_saloonCB_currentIndexChanged(int index);
    void on_actionRemove_Record_triggered();

private:
    Ui::OdfReportDialog *ui;
    DanetDbMan *dbMan;
    QStandardItemModel *model;
    QMenu contextMenu;
    QModelIndex index;
    int clickedRow;
};

#endif // ODFREPORTDIALOG_H
