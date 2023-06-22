#ifndef MA5600REMOVEAGGMETROPLANDIALOG_H
#define MA5600REMOVEAGGMETROPLANDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class MA5600RemoveAggMetroPlanDialog;
}

class MA5600RemoveAggMetroPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5600RemoveAggMetroPlanDialog(QWidget *parent, DanetDbMan *Db, const int DevexId, QString DslamName);
    ~MA5600RemoveAggMetroPlanDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::MA5600RemoveAggMetroPlanDialog *ui;
    DanetDbMan *dbMan;
    const QString dslamName;
    const int devexId;

};

#endif // MA5600REMOVEAGGMETROPLANDIALOG_H
