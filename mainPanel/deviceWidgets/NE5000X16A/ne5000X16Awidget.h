#ifndef NE5000X16AWIDGET_H
#define NE5000X16AWIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class NE5000X16AWidget;
}

class NE5000X16AWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NE5000X16AWidget(QWidget *parent, const bool NE40FLAG, QSqlQueryModel *model);
    ~NE5000X16AWidget();
    void hideSlots();
    QString stringToVerticalString(QString string);
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::NE5000X16AWidget *ui;
};

#endif // NE5000X16AWIDGET_H
