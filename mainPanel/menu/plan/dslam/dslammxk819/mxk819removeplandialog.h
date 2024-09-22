#ifndef MXK819REMOVEPLANDIALOG_H
#define MXK819REMOVEPLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class Mxk819RemovePlanDialog;
}

class Mxk819RemovePlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Mxk819RemovePlanDialog(QWidget *parent, DanetDbMan *Db, const int DevexId, QString DslamName);
    ~Mxk819RemovePlanDialog();


private slots:
    void on_checkBox_toggled(bool checked);

    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::Mxk819RemovePlanDialog *ui;
    DanetDbMan *dbMan;
    const QString dslamName;
    const int devexId;

};

#endif // MXK819REMOVEPLANDIALOG_H
