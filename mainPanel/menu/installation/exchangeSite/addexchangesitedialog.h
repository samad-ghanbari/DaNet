#ifndef ADDEXCHANGESITEDIALOG_H
#define ADDEXCHANGESITEDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class AddExchangeSiteDialog;
}

class AddExchangeSiteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddExchangeSiteDialog(QWidget *parent, DanetDbMan *db, bool JustSite=false);
    ~AddExchangeSiteDialog();
    void checkEntry();
    QString getAbbr();
    int getMotherId();
    bool isDone();


private:
    Ui::AddExchangeSiteDialog *ui;
    DanetDbMan *dbMan;
    const bool justSite;
    bool ADDED;


private slots:
    void on_typeCB_currentIndexChanged(int index);
    void on_areaCB_currentIndexChanged(int index);
    void on_exchLE_textChanged(const QString &arg1);
    void on_abbrLE_textChanged(const QString &arg1);
    void on_addressTE_textChanged();
    void on_okBtn_clicked();
    void on_dataSaloonCB_currentIndexChanged(int index);
    void on_switchSaloonCB_currentIndexChanged(int index);
    void on_pcmSaloonCB_currentIndexChanged(int index);
    void on_uplinkExchCB_currentIndexChanged(int index);
    void on_motherCB_currentIndexChanged(int index);
    void on_cascadeSB_valueChanged(int arg1);
    void on_nodeSB_valueChanged(int arg1);
    void on_addBackBtn_clicked();
    void on_siteNextBtn_clicked();
    void on_siteBackBtn_clicked();
    void on_exchNextBtn_clicked();
};

#endif // ADDEXCHANGESITEDIALOG_H
