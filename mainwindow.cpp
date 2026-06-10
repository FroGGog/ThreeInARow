#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QLayout>
#include <QVBoxLayout>
#include <QDebug>

#include "renderwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Hello world");

    auto* renderWidget = new RenderWidget(this);
    button = new QPushButton("Press");

    QVBoxLayout *containerLayout = new QVBoxLayout(ui->renderWidgetContainer);
    containerLayout->addWidget(renderWidget);
    containerLayout->addWidget(button);

    connect(button, &QPushButton::clicked, [this](){ qDebug() << "Hello world\n";});

}

MainWindow::~MainWindow()
{
    delete ui;
}
