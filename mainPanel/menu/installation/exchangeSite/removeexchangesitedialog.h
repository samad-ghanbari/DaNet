#ifndef REMOVEEXCHANGESITEDIALOG_H
#define REMOVEEXCHANGESITEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveExchangeSiteDialog;
}

class RemoveExchangeSiteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveExchangeSiteDialog(QWidget *parent, DanetDbMan *db,int Id,int area,QString exch,QString abbr,QString type);
    ~RemoveExchangeSiteDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_removeBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveExchangeSiteDialog *ui;
    DanetDbMan  *dbMan;
    int id;
};

#endif // REMOVEEXCHANGESITEDIALOG_H
