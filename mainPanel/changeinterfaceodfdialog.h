#ifndef CHANGEINTERFACEODFDIALOG_H
#define CHANGEINTERFACEODFDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class ChangeInterfaceOdfDialog;
}

class ChangeInterfaceOdfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeInterfaceOdfDialog(QWidget *parent, DanetDbMan *DbMan, const int ExchId, const QString clickedLabel, const int OdfId, const int PosId, const int clickedPinId, const int IntId);
    ~ChangeInterfaceOdfDialog();
    void checkEntry();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_odfSaloonCB_currentIndexChanged(int index);
    void on_odfCB_currentIndexChanged(int index);
    void on_posCB_currentIndexChanged(int index);
    void on_pinCB_currentIndexChanged(int index);

private:
    Ui::ChangeInterfaceOdfDialog *ui;
    DanetDbMan *dbMan;
    const int exchId, odfId, posId, pinId, intId;
    QMap<int,int> odfMap, posMap;
};

#endif // CHANGEINTERFACEODFDIALOG_H
