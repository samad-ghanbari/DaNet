#ifndef BRASNASIPMANDIALOG_H
#define BRASNASIPMANDIALOG_H

#include <QDialog>
#include <QMenu>
class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class BrasNasIpManDialog;
}

class BrasNasIpManDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrasNasIpManDialog(QWidget *parent, DanetDbMan *db);
    ~BrasNasIpManDialog();
    void fillTV();

private slots:
    void refreshSlot(int i);
    void contextMenuSlot(QPoint ptr);
    void on_actionEdit_NAS_IP_triggered();
    void on_actionRemove_NAS_IP_triggered();
    void on_addBtn_clicked();

private:
    Ui::BrasNasIpManDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    QMenu contextMenu;
    int row,clickedId, clickedDevexId, clickedArea;
    QString clickedNas, clickedBRAS;

};

#endif // BRASNASIPMANDIALOG_H
