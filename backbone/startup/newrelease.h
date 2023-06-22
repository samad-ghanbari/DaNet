#ifndef NEWRELEASE_H
#define NEWRELEASE_H

#include <QDialog>

namespace Ui {
class NewRelease;
}

class NewRelease : public QDialog
{
    Q_OBJECT

public:
    explicit NewRelease(QWidget *parent=0);
    ~NewRelease();
    void setVersionLabel(QString ver);
    void setPathLabel(QString path);

private slots:
    void on_pushButton_clicked();

private:
    Ui::NewRelease *ui;
};

#endif // NEWRELEASE_H
