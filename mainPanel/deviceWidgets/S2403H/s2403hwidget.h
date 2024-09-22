#ifndef S2403HWIDGET_H
#define S2403HWIDGET_H

#include <QWidget>

namespace Ui {
class S2403HWidget;
}

class S2403HWidget : public QWidget
{
    Q_OBJECT

public:
    explicit S2403HWidget(QWidget *parent = 0);
    ~S2403HWidget();

private:
    Ui::S2403HWidget *ui;
};

#endif // S2403HWIDGET_H
