#ifndef ASR1002WIDGET_H
#define ASR1002WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class ASR1002Widget;
}

class ASR1002Widget : public QWidget
{
    Q_OBJECT

public:
    explicit ASR1002Widget(QWidget *parent, QSqlQueryModel *model);
    ~ASR1002Widget();
    void hideSlots();
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::ASR1002Widget *ui;
};

#endif // ASR1002WIDGET_H
