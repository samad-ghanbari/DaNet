#ifndef OUTOFSERVICE_H
#define OUTOFSERVICE_H

#include <QDialog>

namespace Ui {
class OutOfService;
}

class OutOfService : public QDialog
{
    Q_OBJECT

public:
    explicit OutOfService(QWidget *parent=0);
    ~OutOfService();

private slots:
    void on_pushButton_clicked();

private:
    Ui::OutOfService *ui;
};

#endif // OUTOFSERVICE_H
