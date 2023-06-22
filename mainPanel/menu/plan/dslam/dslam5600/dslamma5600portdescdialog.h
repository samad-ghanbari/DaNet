#ifndef DSLAMMA5600PORTDESCDIALOG_H
#define DSLAMMA5600PORTDESCDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class DslamMa5600PortDescDialog;
}

class DslamMa5600PortDescDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DslamMa5600PortDescDialog(QWidget *parent, DanetDbMan *db,const QString dslamName,const int PortId,const QString interface,const QString desc);
    ~DslamMa5600PortDescDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_descLE_textChanged(const QString &arg1);

private:
    Ui::DslamMa5600PortDescDialog *ui;
    DanetDbMan *dbMan;
    const int portId;
};

#endif // DSLAMMA5600PORTDESCDIALOG_H
