#ifndef CAT_DIALOG_H
#define CAT_DIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>

class DanetDbMan;
class CenterAlignedModel;

namespace Commodity
{
namespace Cat {

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(DanetDbMan *db, QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
    DanetDbMan *dbMan;
    QMenu contextMenu;
    bool adminUser;
    CenterAlignedModel *model;
    int row, id;
    QString cat;
    void fillTV();

private slots:
    void contextMenuSlot(QPoint ptr);
    void doubleClickedSlot(QModelIndex ind);
    void on_addBtn_clicked();
    void on_actionEdit_Category_triggered();
    void on_actionRemove_Category_triggered();
};

}
}
#endif // CAT_DIALOG_H
