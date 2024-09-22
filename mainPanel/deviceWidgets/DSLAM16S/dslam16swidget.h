#ifndef Dslam16sWIDGET_H
#define Dslam16sWIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class Dslam16sWidget;
}

class Dslam16sWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Dslam16sWidget(QWidget *parent,const QString DslamName, QSqlQueryModel *model);
    ~Dslam16sWidget();
    void hideSlots();
    QString stringToVerticalString(QString string);
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::Dslam16sWidget *ui;
};

#endif // Dslam16sWIDGET_H
