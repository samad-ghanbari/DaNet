#ifndef REGEXPHELPDIALOG_H
#define REGEXPHELPDIALOG_H

#include <QDialog>
class QStandardItemModel;

namespace Ui {
class RegexpHelpDialog;
}

class RegexpHelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegexpHelpDialog(QWidget *parent = 0);
    ~RegexpHelpDialog();

private slots:
    void on_okBtn_clicked();

private:
    Ui::RegexpHelpDialog *ui;
    QStandardItemModel *model;
};

#endif // REGEXPHELPDIALOG_H
