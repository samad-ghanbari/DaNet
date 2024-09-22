#ifndef COMMODITY_USAGE_USAGE_REMOVE_H
#define COMMODITY_USAGE_USAGE_REMOVE_H

#include <QDialog>
class DanetDbMan;

namespace Commodity {
namespace Usage {

namespace Ui {
class Remove;
}

class Remove : public QDialog
{
    Q_OBJECT

public:
    explicit Remove(const int id, DanetDbMan *db, QWidget *parent = nullptr);
    ~Remove();
    void setValues(const QString letter,const int amount, const QString desc);
    void setLables(const QString titr, const QString comm);

private slots:
    void on_okBtn_clicked();

private:
    Ui::Remove *ui;
    DanetDbMan *dbMan;
    int usage_id;
};


} // namespace Usage
} // namespace Commodity
#endif // COMMODITY_USAGE_USAGE_REMOVE_H
