#ifndef MA5616REMOVEPLANDIALOG_H
#define MA5616REMOVEPLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MA5616RemovePlanDialog;
}

class MA5616RemovePlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5616RemovePlanDialog(QWidget *parent, DanetDbMan *Db, const int DevexId, QString DslamName);
    ~MA5616RemovePlanDialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::MA5616RemovePlanDialog *ui;
    DanetDbMan *dbMan;
    const QString dslamName;
    const int devexId;
};

#endif // MA5616REMOVEPLANDIALOG_H
