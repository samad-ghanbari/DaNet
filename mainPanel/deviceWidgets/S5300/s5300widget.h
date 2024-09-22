#ifndef S5300WIDGET_H
#define S5300WIDGET_H

#include <QWidget>

namespace Ui {
class S5300Widget;
}

class S5300Widget : public QWidget
{
    Q_OBJECT

public:
    explicit S5300Widget(QWidget *parent = 0);
    ~S5300Widget();

private:
    Ui::S5300Widget *ui;
};

#endif // S5300WIDGET_H
