#ifndef DSLAM4SWIDGET_H
#define DSLAM4SWIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class Dslam4sWidget;
}

class Dslam4sWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Dslam4sWidget(QWidget *parent,QSqlQueryModel *model);
    ~Dslam4sWidget();
    void hideSlots();
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::Dslam4sWidget *ui;
};

#endif // DSLAM4SWIDGET_H
