#include "renderwidget.h"

#include <QPainter>
#include <QMouseEvent>

#include <iostream>


RenderWidget::RenderWidget(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_OpaquePaintEvent);

    connect(&timer_, &QTimer::timeout, this, &RenderWidget::tick);

    timer_.start(16);
    frameTimer_.start();
}

RenderWidget::~RenderWidget()
{

}

void RenderWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);

    field_.render(painter);
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    field_.click(event->pos().x(), event->pos().y());
    std::cout << event->pos().x() << event->pos().y() << '\n';
    QWidget::mousePressEvent(event);
}

void RenderWidget::resizeEvent(QResizeEvent *event)
{
    field_.resize((std::min(event->size().width(), event->size().height()) / 10));
}

void RenderWidget::tick()
{
    double dt = frameTimer_.restart() / 1000.0;

    field_.update(dt);

    update();
}
