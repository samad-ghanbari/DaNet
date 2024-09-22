#ifndef RECOMMENDATIONSVIEWDIALOG_H
#define RECOMMENDATIONSVIEWDIALOG_H

#include <QDialog>
#include <QMenu>

class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class RecommendationsViewDialog;
}

class RecommendationsViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecommendationsViewDialog(QWidget *parent, DanetDbMan *db);
    ~RecommendationsViewDialog();
    void fillTV();

private slots:
    void on_actionDelete_Recommendation_triggered();
    void contextMenuSlot(QPoint ptr);

private:
    Ui::RecommendationsViewDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    QMenu contextMenu;
    int row,id;
    QString lastname, recomm;
};

#endif // RECOMMENDATIONSVIEWDIALOG_H
