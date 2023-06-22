#ifndef DSLAMMA5616PORTDESCDIALOG_H
#define DSLAMMA5616PORTDESCDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class DslamMa5616PortDescDialog;
}

class DslamMa5616PortDescDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DslamMa5616PortDescDialog(QWidget *parent, DanetDbMan *db,const QString dslamName,const int PortId,const QString interface,const QString desc);
    ~DslamMa5616PortDescDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_descLE_textChanged(const QString &arg1);

private:
    Ui::DslamMa5616PortDescDialog *ui;
    DanetDbMan *dbMan;
    const int portId;
};

#endif // DSLAMMA5616PORTDESCDIALOG_H
