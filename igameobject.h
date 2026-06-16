#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include <QPainter>
#include <QRandomGenerator>

enum ObjectColor {NONE, RED, BLUE, GREEN, YELLOW, MAGNETA, CYAN};


inline ObjectColor pickRandomColor()
{
    int number = QRandomGenerator::global()->bounded(1, 7);
    switch (number) {
    case 1:
        return ObjectColor::RED;
    case 2:
        return ObjectColor::BLUE;
    case 3:
        return ObjectColor::GREEN;
    case 4:
        return ObjectColor::YELLOW;
    case 5:
        return ObjectColor::MAGNETA;
    case 6:
        return ObjectColor::CYAN;
    default:
        return ObjectColor::NONE;
    }
}

inline QColor convertColor(ObjectColor color)
{
    switch (color) {
    case ObjectColor::RED:
        return Qt::red;
    case ObjectColor::BLUE:
        return Qt::blue;
    case ObjectColor::GREEN:
        return Qt::green;
    case ObjectColor::YELLOW:
        return Qt::yellow;
    case ObjectColor::MAGNETA:
        return Qt::magenta;
    case ObjectColor::CYAN:
        return Qt::cyan;
    default:
        return Qt::white;
    }
}

class IGameObject
{
public:

    virtual ~IGameObject() = default;

    virtual void update(double dt) = 0;

    virtual void render(QPainter& painter) const = 0;

private:

    ObjectColor color = ObjectColor::NONE;

};

class CircleObject : public IGameObject
{

public:
    CircleObject(int coll, int row, ObjectColor color) :
        m_coll(coll),
        m_row(row),
        m_color(convertColor(color)) {}

    void update(double dt) override
    {

    }

    void updateGeometry(double cell_size)
    {
        m_xpos = m_coll * cell_size;
        m_ypos = m_row * cell_size;

        m_xr = 0.4 * cell_size;
        m_yr = m_xr;

        m_xoffset = 0.5 * cell_size;
        m_yoffset = 0.5 * cell_size;
    }

    void render (QPainter& painter) const override
    {
        painter.setBrush(m_color);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPoint{m_xpos + m_xoffset, m_ypos + m_yoffset}, m_xr, m_yr);
    }

    QColor getColor() const
    {
        return m_color;
    }


private:
    int m_coll, m_row;
    int m_xpos, m_ypos;
    int m_xr, m_yr;

    int m_xoffset = 10;
    int m_yoffset = 25;

    QColor m_color;

};


#endif // IGAMEOBJECT_H
