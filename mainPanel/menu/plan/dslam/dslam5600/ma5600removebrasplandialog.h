#ifndef MA5600REMOVEBRASPLANDIALOG_H
#define MA5600REMOVEBRASPLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class MA5600RemoveBrasPlanDialog;
}

class MA5600RemoveBrasPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MA5600RemoveBrasPlanDialog(QWidget *parent, DanetDbMan *Db, const int DevexId, QString DslamName);
    ~MA5600RemoveBrasPlanDialog();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::MA5600RemoveBrasPlanDialog *ui;
    DanetDbMan *dbMan;
    const QString dslamName;
    const int devexId;
};

#endif // MA5600REMOVEBRASPLANDIALOG_H
