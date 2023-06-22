#ifndef NE40X16WIDGET_H
#define NE40X16WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class NE40X16Widget;
}

class NE40X16Widget : public QWidget
{
    Q_OBJECT

public:
    explicit NE40X16Widget(QWidget *parent, const bool NE40FLAG, QSqlQueryModel *model);
    ~NE40X16Widget();
    void hideSlots();
    QString stringToVerticalString(QString string);
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::NE40X16Widget *ui;
};

#endif // NE40X16WIDGET_H
