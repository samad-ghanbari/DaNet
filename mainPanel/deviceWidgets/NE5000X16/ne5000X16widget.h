#ifndef NE5000X16WIDGET_H
#define NE5000X16WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class NE5000X16Widget;
}

class NE5000X16Widget : public QWidget
{
    Q_OBJECT

public:
    explicit NE5000X16Widget(QWidget *parent, QSqlQueryModel *model);
    ~NE5000X16Widget();
    void hideSlots();
    QString stringToVerticalString(QString string);
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::NE5000X16Widget *ui;
};

#endif // NE5000X16WIDGET_H
