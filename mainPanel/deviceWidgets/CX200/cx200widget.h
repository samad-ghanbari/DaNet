#ifndef CX200WIDGET_H
#define CX200WIDGET_H

#include <QWidget>

namespace Ui {
class CX200Widget;
}

class CX200Widget : public QWidget
{
    Q_OBJECT

public:
    explicit CX200Widget(QWidget *parent = 0);
    ~CX200Widget();

private:
    Ui::CX200Widget *ui;
};

#endif // CX200WIDGET_H
