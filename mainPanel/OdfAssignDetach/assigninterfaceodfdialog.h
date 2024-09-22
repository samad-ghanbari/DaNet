#ifndef ASSIGNINTERFACEODFDIALOG_H
#define ASSIGNINTERFACEODFDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class AssignInterfaceOdfDialog;
}

class AssignInterfaceOdfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AssignInterfaceOdfDialog(QWidget *parent, DanetDbMan *DbMan, const int ExchId, const QString clickedLabel,const int IntId);
    ~AssignInterfaceOdfDialog();
    void checkEntry();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_odfSaloonCB_currentIndexChanged(int index);
    void on_odfCB_currentIndexChanged(int index);
    void on_posCB_currentIndexChanged(int index);
    void on_pinCB_currentIndexChanged(int index);

private:
    Ui::AssignInterfaceOdfDialog *ui;
    DanetDbMan *dbMan;
    const int exchId, intId;
    QMap<int,int> odfMap, posMap;
};

#endif // ASSIGNINTERFACEODFDIALOG_H
