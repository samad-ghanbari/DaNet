#ifndef EDITEXCHANGESITEDIALOG_H
#define EDITEXCHANGESITEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditExchangeSiteDialog;
}

class EditExchangeSiteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditExchangeSiteDialog(QWidget *parent, DanetDbMan *db, const int id, const int area, const QString exch, const QString abbr, const int type_no, const QString dataSaloon, const QString switchSaloon, const QString pcmSaloon, const int motherId, const int uplinkId, const int cascade, const int node, const QString address);
    ~EditExchangeSiteDialog();
    void checkEntry();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_exchLE_textChanged(const QString &arg1);
    void on_abbrLE_textChanged(const QString &arg1);
    void on_typeCB_currentIndexChanged(int index);
    void on_addressTE_textChanged();
    void on_okBtn_clicked();
    void on_dataSaloonCB_currentIndexChanged(int index);
    void on_switchSaloonCB_currentIndexChanged(int index);
    void on_pcmSaloonCB_currentIndexChanged(int index);
    void on_cascadeSB_valueChanged(int arg1);
    void on_nodeSB_valueChanged(int arg1);
    void on_uplinkExchCB_currentIndexChanged(int index);
    void on_motherCB_currentIndexChanged(int index);
    void on_exchNextBtn_clicked();
    void on_siteBackBtn_clicked();
    void on_siteNextBtn_clicked();
    void on_editBackBtn_clicked();


private:
    Ui::EditExchangeSiteDialog *ui;
    DanetDbMan *dbMan;
    const QString oldAbbr;
    int id;
};

#endif // EDITEXCHANGESITEDIALOG_H
