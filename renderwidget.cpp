#include "renderwidget.h"

#include <QPainter>
#include <QMouseEvent>



RenderWidget::RenderWidget(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_OpaquePaintEvent);


    connect(&timer_, &QTimer::timeout, this, &RenderWidget::tick);

    timer_.start(16);
    frameTimer_.start();

    field_.resize(width() / 10);
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

    int field_size = std::min(width(), height());
    int offset_x = (width() - field_size) / 2;
    int offset_y = (height() - field_size) / 2;

    painter.translate(offset_x, offset_y);

    field_.render(painter);
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    int field_size = std::min(width(), height());
    int offset_x = (width() - field_size) / 2;
    int offset_y = (height() - field_size) / 2;

    int adjusted_x = event->pos().x() - offset_x;
    int adjusted_y = event->pos().y() - offset_y;

    if (adjusted_x >= 0 && adjusted_x < field_size &&
        adjusted_y >= 0 && adjusted_y < field_size) {
        field_.click(adjusted_x, adjusted_y);
    }

    QWidget::mousePressEvent(event);
}

void RenderWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int side = std::min(width(), height());
    field_.resize(side / 10);
}

void RenderWidget::tick()
{
    double dt = frameTimer_.restart() / 1000.0;

    field_.update(dt);

    update();
}
