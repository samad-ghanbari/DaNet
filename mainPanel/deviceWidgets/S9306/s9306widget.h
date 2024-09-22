#ifndef S9306WIDGET_H
#define S9306WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class S9306Widget;
}

class S9306Widget : public QWidget
{
    Q_OBJECT

public:
    explicit S9306Widget(QWidget *parent, QSqlQueryModel *model);
    ~S9306Widget();
    void hideSlots();
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::S9306Widget *ui;
};

#endif // S9306WIDGET_H
