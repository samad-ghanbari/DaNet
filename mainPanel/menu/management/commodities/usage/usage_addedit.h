#ifndef COMMODITY_USAGE_USAGE_ADDEDIT_H
#define COMMODITY_USAGE_USAGE_ADDEDIT_H

#include <QDialog>
class DanetDbMan;

namespace Commodity {
namespace Usage {

namespace Ui {
class Addedit;
}

class Addedit : public QDialog
{
    Q_OBJECT

public:
    explicit Addedit(const int commId,const int left, DanetDbMan *db, QWidget *parent = nullptr);
    ~Addedit();
    void setValues(const int usageId, const QString letter, const int left, const int amount, const QString desc);
    void setLables(const QString titr, const QString comm);

private slots:
    void on_okBtn_clicked();

private:
    Ui::Addedit *ui;
    DanetDbMan *dbMan;
    const int comm_id;
    int usage_id;
};


} // namespace Usage
} // namespace Commodity
#endif // COMMODITY_USAGE_USAGE_ADDEDIT_H
