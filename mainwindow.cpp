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

    setWindowTitle("Match-3");

    setMinimumSize(400, 400);
    resize(640, 640);

    auto* renderWidget = new RenderWidget(this);

    QVBoxLayout *containerLayout = new QVBoxLayout(ui->renderWidgetContainer);

    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(0);
    containerLayout->addWidget(renderWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}
