#ifndef MODULESMANDIALOG_H
#define MODULESMANDIALOG_H

#include <QDialog>
#include <QMenu>
class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class ModulesManDialog;
}

class ModulesManDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModulesManDialog(QWidget *parent, DanetDbMan *db);
    ~ModulesManDialog();
    void fillTV();

private slots:
    void refreshSlot(int i);
    void contextMenuSlot(QPoint ptr);
    void on_addModuleBtn_clicked();
    void on_actionEdit_Module_triggered();
    void on_actionRemove_Module_triggered();
    void on_adminLE_textChanged(const QString &arg1);

private:
    Ui::ModulesManDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    QMenu contextMenu;
    bool AdminUserFlag;
    int id,row;
    double km;
    QString module, vendor;
};

#endif // MODULESMANDIALOG_H
