#ifndef MA5600REMOVEPLANDIALOG_H
#define MA5600REMOVEPLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MA5600RemovePlanDialog;
}

class MA5600RemovePlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5600RemovePlanDialog(QWidget *parent, DanetDbMan *Db, const int DevexId, QString DslamName);
    ~MA5600RemovePlanDialog();


private slots:
    void on_checkBox_toggled(bool checked);

    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::MA5600RemovePlanDialog *ui;
    DanetDbMan *dbMan;
    const QString dslamName;
    const int devexId;

};

#endif // MA5600REMOVEPLANDIALOG_H
