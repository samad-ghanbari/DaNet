#ifndef S9303WIDGET_H
#define S9303WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class S9303Widget;
}

class S9303Widget : public QWidget
{
    Q_OBJECT

public:
    explicit S9303Widget(QWidget *parent,QSqlQueryModel *model);
    ~S9303Widget();
    void hideSlots();
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::S9303Widget *ui;
};

#endif // S9303WIDGET_H
