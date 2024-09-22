#ifndef MA5616REMOVEBRASPLANDIALOG_H
#define MA5616REMOVEBRASPLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MA5616RemoveBrasPlanDialog;
}

class MA5616RemoveBrasPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5616RemoveBrasPlanDialog(QWidget *parent , DanetDbMan *Db, const int DevexId, QString DslamName);
    ~MA5616RemoveBrasPlanDialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::MA5616RemoveBrasPlanDialog *ui;
    DanetDbMan *dbMan;
    const QString dslamName;
    const int devexId;
};

#endif // MA5616REMOVEBRASPLANDIALOG_H
