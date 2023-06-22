#ifndef INTERFACETYPEDIALOG_H
#define INTERFACETYPEDIALOG_H

#include <QDialog>
#include <QMenu>
class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class InterfaceTypeDialog;
}

class InterfaceTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InterfaceTypeDialog(QWidget *parent, DanetDbMan *db);
    ~InterfaceTypeDialog();
    void fillTV();


private slots:
    void refreshSlot(int done);

    void contextMenuSlot(QPoint ptr);

    void on_adminLE_textChanged(const QString &arg1);

    void on_actionEdit_IntType_triggered();

    void on_actionRemove_IntType_triggered();

    void on_addIntTypeBtn_clicked();

private:
    Ui::InterfaceTypeDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    QMenu contextMenu;
    bool AdminUserFlag;
    int id,row;
    double speed;
    QString intType, media;
};

#endif // INTERFACETYPEDIALOG_H
