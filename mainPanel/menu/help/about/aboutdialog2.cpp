#include "aboutdialog2.h"
#include "ui_aboutdialog2.h"

//about qml
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <QVBoxLayout>
#include <QTimer>

AboutDialog2::AboutDialog2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog2)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    QQuickView *view = new QQuickView();
    QQmlContext *rootContext = view->rootContext();
    rootContext->setContextProperty("aboutClass", this);

    //view->setSource(QUrl::fromLocalFile("/home/samad/share/programs/Cpp/QT/DaNet/About.qml"));

    QWidget *container = QWidget::createWindowContainer(view, this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(container);
    this->setLayout(layout);
    view->setSource(QUrl("qrc:/mainPanel/menu/help/about/About.qml"));

}

AboutDialog2::~AboutDialog2()
{
    delete ui;
}

void AboutDialog2::closeDialog()
{
    QTimer::singleShot(2500, this, SLOT(close()));
}

QString AboutDialog2::getVersion()
{
    return DANET_VER;
}
