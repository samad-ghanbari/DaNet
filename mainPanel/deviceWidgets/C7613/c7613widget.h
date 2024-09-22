#ifndef C7613WIDGET_H
#define C7613WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class C7613Widget;
}

class C7613Widget : public QWidget
{
    Q_OBJECT

public:
    explicit C7613Widget(QWidget *parent, QSqlQueryModel *model);
    ~C7613Widget();
    void hideSlots();
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::C7613Widget *ui;
};

#endif // C7613WIDGET_H
