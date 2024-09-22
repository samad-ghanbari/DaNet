#ifndef REMOVERECOMMENDATIONDIALOG_H
#define REMOVERECOMMENDATIONDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveRecommendationDialog;
}

class RemoveRecommendationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveRecommendationDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString lastname, const QString recom);
    ~RemoveRecommendationDialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::RemoveRecommendationDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // REMOVERECOMMENDATIONDIALOG_H
