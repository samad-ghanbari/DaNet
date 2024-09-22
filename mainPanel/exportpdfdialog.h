#ifndef EXPORTPDFDIALOG_H
#define EXPORTPDFDIALOG_H

#include <QDialog>
class QSqlQueryModel;
class DanetDbMan;

namespace Ui {
class ExportPdfDialog;
}

class ExportPdfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportPdfDialog(QWidget *parent,DanetDbMan *db, const QList<int> intIds,const QList<int> posIds, const QList<int> PinIds);
    ~ExportPdfDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_odfChB_toggled(bool checked);
    void on_confirmChB_toggled(bool checked);

private:
    Ui::ExportPdfDialog *ui;
    DanetDbMan *dbMan;
    const QList<int> pinIds;
    QSqlQueryModel *intModel, *odfModel;
    bool odfExport;

};

#endif // EXPORTPDFDIALOG_H
