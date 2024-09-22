#ifndef CAT_REMOVE_H
#define CAT_REMOVE_H

#include <QDialog>
class DanetDbMan;

namespace Commodity
{
namespace Cat
{

namespace Ui {
class Remove;
}

class Remove : public QDialog
{
    Q_OBJECT

public:
    explicit Remove(DanetDbMan *db, const int ID,const QString CAT, QWidget *parent = nullptr);
    ~Remove();

private slots:
    void on_okBtn_clicked();

private:
    Ui::Remove *ui;
    DanetDbMan *dbMan;
    const int id;
};

}
}

#endif // CAT_REMOVE_H
