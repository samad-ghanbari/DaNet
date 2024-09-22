#ifndef EDITDEVICEDESCDIALOG_H
#define EDITDEVICEDESCDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class EditDeviceDescDialog;
}

class EditDeviceDescDialog : public QDialog
{
    Q_OBJECT
//abbr,clickedDevexId, clickedSaloon,clickedRow,
//    clickedRack,clickedShelf,clickedDeviceName
public:
    explicit EditDeviceDescDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const QString saloon,  const int DevexId, const int Row, const int Rack,
                                     const int Shelf, const QString DeviceName, const QString desc);
    ~EditDeviceDescDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_checkBox_toggled(bool checked);
    void on_descTE_textChanged();

private:
    Ui::EditDeviceDescDialog *ui;
    DanetDbMan *dbMan;
    const int devexId;
};

#endif // EDITDEVICEDESCDIALOG_H
