#ifndef CONFIGURATIONANALYSERDIALOG_H
#define CONFIGURATIONANALYSERDIALOG_H

#include <QDialog>
#include <QFile>
#include <QMap>
class QStringListModel;
class MapModel;
class QStandardItemModel;

namespace Ui {
class ConfigurationAnalyserDialog;
}

class ConfigurationAnalyserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationAnalyserDialog(QWidget *parent = 0);
    ~ConfigurationAnalyserDialog();
    void emptyAll();
    void huaweiConfigureAnalyser();
    void IOSConfigureAnalyser();
    void IOSXRConfigureAnalyser();
    bool modelContains(QStandardItemModel *model, const QString value);

private slots:
    void on_osCB_currentIndexChanged(int index);
    void on_btn_clicked();
    void on_vpnTV_clicked(const QModelIndex &index);
    void on_intExportBtn_clicked();

private:
    Ui::ConfigurationAnalyserDialog *ui;
    MapModel *vlanModel, *etherModel;
    QStandardItemModel *vpnModel,*interfaceModel, *StaticModel, *VpnStaticModel;
    QStringListModel *expModel, *impModel;
    QFile configFile;
    QMap<int,QString> vlanDescMap, expMap, impMap, etherDescMap;
};

#endif // CONFIGURATIONANALYSERDIALOG_H
