#ifndef COMM_ADDEDIT_H
#define COMM_ADDEDIT_H

#include <QDialog>
class DanetDbMan;

namespace Commodity {

namespace Ui {
class AddEdit;
}

class AddEdit : public QDialog
{
    Q_OBJECT

public:
    explicit AddEdit(const int catId, DanetDbMan *db, QWidget *parent = nullptr);
    ~AddEdit();
    void setValues(const int comId, const QString comm, const int purchaseId, const QString location, const QString code, const int total, const int left, const bool en);
    void setLables(const QString titr, const QString cat);

private slots:
    void on_okBtn_clicked();

private:
    Ui::AddEdit *ui;
    DanetDbMan *dbMan;
    const int cat_id;
    int comm_id;
};


} // namespace Info
#endif // COMM_ADDEDIT_H
