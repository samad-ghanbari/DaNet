#ifndef COMM_REMOVE_H
#define COMM_REMOVE_H

#include <QDialog>
class DanetDbMan;

namespace Commodity {

namespace Ui {
class Remove;
}

class Remove : public QDialog
{
    Q_OBJECT

public:
    explicit Remove(const int id, DanetDbMan *db, QWidget *parent = nullptr);
    ~Remove();
    void setValues(const QString comm, const QString purchase, const QString location, const QString code, const int total);
    void setLables(const QString titr, const QString cat);

private slots:
    void on_okBtn_clicked();

private:
    Ui::Remove *ui;
    DanetDbMan *dbMan;
    int comm_id;
};


} // namespace
#endif // COMM_REMOVE_H
