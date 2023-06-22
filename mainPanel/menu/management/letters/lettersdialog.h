#ifndef LETTERSDIALOG_H
#define LETTERSDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class LettersDialog;
}

class LettersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LettersDialog(DanetDbMan *db, QWidget *parent = nullptr);
    ~LettersDialog();

private slots:
    void fillTV();
    void contextMenuSlot(QPoint ptr);
    void doubleClickedSlot(QModelIndex ind);
    void headerWidgetsChagedSlot();
    void on_addBtn_clicked();
    void on_actionView_triggered();
    void on_actionEdit_triggered();
    void on_actionRemove_triggered();

private:
    Ui::LettersDialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    QMenu contextMenu;
    int id, row;
    QString letter_id, letter_no,from_dep, to_dep, desc,ts;
};

#endif // LETTERSDIALOG_H
