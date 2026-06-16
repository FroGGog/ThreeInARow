#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QLayout>
#include <QVBoxLayout>

#include "renderwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Hello world");

    auto* renderWidget = new RenderWidget(this);

    QVBoxLayout *containerLayout = new QVBoxLayout(ui->renderWidgetContainer);
    containerLayout->addWidget(renderWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}
