#ifndef MXK819REMOVEBRASPLANDIALOG_H
#define MXK819REMOVEBRASPLANDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class Mxk819RemoveBrasPlanDialog;
}

class Mxk819RemoveBrasPlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Mxk819RemoveBrasPlanDialog(QWidget *parent, DanetDbMan *Db, const int DevexId, QString DslamName);
    ~Mxk819RemoveBrasPlanDialog();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::Mxk819RemoveBrasPlanDialog *ui;
    DanetDbMan *dbMan;
    const QString dslamName;
    const int devexId;
};

#endif // MXK819REMOVEBRASPLANDIALOG_H
