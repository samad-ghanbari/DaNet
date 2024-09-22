#ifndef DETACHINTERFACEODFDIALOG_H
#define DETACHINTERFACEODFDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class DetachInterfaceOdfDialog;
}

class DetachInterfaceOdfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DetachInterfaceOdfDialog(QWidget *parent, DanetDbMan *DbMan, const int ExchId, const QString clickedLabel, const int OdfId, const int PosId, const int clickedPinId, const int IntId);
    ~DetachInterfaceOdfDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_confirmChB_toggled(bool checked);

private:
    Ui::DetachInterfaceOdfDialog *ui;
    DanetDbMan *dbMan;
    const int exchId, odfId, posId, pinId, intId;
};

#endif // DETACHINTERFACEODFDIALOG_H
