#ifndef NE40X8WIDGET_H
#define NE40X8WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class NE40X8Widget;
}

class NE40X8Widget : public QWidget
{
    Q_OBJECT

public:
    explicit NE40X8Widget(QWidget *parent,const bool NE40FLAG, QSqlQueryModel *model);
    ~NE40X8Widget();
    void hideSlots();
    QString stringToVerticalString(QString string);
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::NE40X8Widget *ui;
};

#endif // NE40X8WIDGET_H
