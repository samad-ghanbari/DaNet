#ifndef ABOUTDIALOG2_H
#define ABOUTDIALOG2_H

#include <QDialog>

namespace Ui {
class AboutDialog2;
}

class AboutDialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog2(QWidget *parent = nullptr);
    ~AboutDialog2();

public slots:
    void closeDialog();
    QString getVersion();

private:
    Ui::AboutDialog2 *ui;
};

#endif // ABOUTDIALOG2_H
