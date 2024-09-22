#ifndef DSLAMMXK819PORTDESCDIALOG_H
#define DSLAMMXK819PORTDESCDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class DslamMxk819PortDescDialog;
}

class DslamMxk819PortDescDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DslamMxk819PortDescDialog(QWidget *parent, DanetDbMan *db,const QString dslamName,const int PortId,const QString interface,const QString desc);
    ~DslamMxk819PortDescDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_descLE_textChanged(const QString &arg1);

private:
    Ui::DslamMxk819PortDescDialog *ui;
    DanetDbMan *dbMan;
    const int portId;
};

#endif // DSLAMMXK819PORTDESCDIALOG_H
