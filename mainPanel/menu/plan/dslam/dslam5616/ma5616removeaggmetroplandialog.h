#ifndef MA5616REMOVEAGGMETROPLANDIALOG_H
#define MA5616REMOVEAGGMETROPLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MA5616RemoveAggMetroPlanDialog;
}

class MA5616RemoveAggMetroPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5616RemoveAggMetroPlanDialog(QWidget *parent, DanetDbMan *Db, const int DevexId, QString DslamName);
    ~MA5616RemoveAggMetroPlanDialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::MA5616RemoveAggMetroPlanDialog *ui;
    DanetDbMan *dbMan;
    const QString dslamName;
    const int devexId;
};

#endif // MA5616REMOVEAGGMETROPLANDIALOG_H
