#ifndef MXK819REMOVEAGGMETROPLANDIALOG_H
#define MXK819REMOVEAGGMETROPLANDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class Mxk819RemoveAggMetroPlanDialog;
}

class Mxk819RemoveAggMetroPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Mxk819RemoveAggMetroPlanDialog(QWidget *parent, DanetDbMan *Db, const int DevexId, QString DslamName);
    ~Mxk819RemoveAggMetroPlanDialog();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::Mxk819RemoveAggMetroPlanDialog *ui;
    DanetDbMan *dbMan;
    const QString dslamName;
    const int devexId;
};

#endif // MXK819REMOVEAGGMETROPLANDIALOG_H
