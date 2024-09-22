#ifndef RECOMMENDATIONDIALOG_H
#define RECOMMENDATIONDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RecommendationDialog;
}

class RecommendationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecommendationDialog(QWidget *parent, DanetDbMan *db);
    ~RecommendationDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_recomTE_textChanged();
    void on_okBtn_clicked();

private:
    Ui::RecommendationDialog *ui;
    DanetDbMan *dbMan;
    QString lastname;
};

#endif // RECOMMENDATIONDIALOG_H
