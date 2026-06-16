#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include <QPainter>
#include <QRandomGenerator>

#include <variant>

class CircleObject;

struct IdleState
{
    void update(CircleObject& owner, double dt);
    void render(const CircleObject& owner, QPainter& painter) const;
};
struct DestroyingState
{
    void update(CircleObject& owner, double dt);
    void render(const CircleObject& owner, QPainter& painter) const;

    int start_radius = 0;
};
struct FallingState
{
    void update(CircleObject& owner, double dt);
    void render(const CircleObject& owner, QPainter& painter) const;

    double target_y = 0.0;
    int target_row = 0;
    double fall_speed = 200.0;
};
struct SpawningState
{
    void update(CircleObject& owner, double dt);
    void render(const CircleObject& owner, QPainter& painter) const;

    int target_radius = 0;
    int current_radius = 0;
};

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
        std::visit([this, dt](auto& state)
                   {
            state.update(*this, dt);
        }, m_state);
    }

    void updateGeometry(double cell_size)
    {
        m_cell_size = cell_size;

        m_xpos = m_coll * cell_size;
        m_ypos = m_row * cell_size;
        m_xr = 0.4 * cell_size;
        m_yr = m_xr;
    }

    void render (QPainter& painter) const override
    {
        std::visit([this, &painter](auto& state)
                   {
            state.render(*this, painter);
        }, m_state);
    }

    // Setters
    void setColor(QColor color)
    {
        m_color = color;
    }
    void setRadius(int radius)
    {
        m_xr = radius;
        m_yr = radius;
    }
    void setPosition(QPoint point)
    {
        m_xpos = point.x();
        m_ypos = point.y();
    }
    void setGridRowPosition(int new_row)
    {
        m_row = new_row;
    }

    // states
    void Destroy()
    {
        m_state = DestroyingState{m_xr};
    }
    void startFalling(int target_row)
    {
        double target_y = target_row * m_cell_size + m_yoffset;
        m_state = FallingState{target_y, target_row};
    }
    void Idle()
    {
        m_state = IdleState{};
    }

    // Getters
    QColor getColor() const
    {
        return m_color;
    }

    QPoint getCenter() const
    {
        return {m_xpos + m_cell_size / 2, m_ypos + m_cell_size / 2};
    }

    std::pair<int, int> getGridPos() const
    {
        return {m_coll, m_row};
    }

    int getRadius() const
    {
        return m_xr;
    }

    int getCellSize() const
    {
        return m_cell_size;
    }

    bool isIdle() const
    {
        return std::holds_alternative<IdleState>(m_state);
    }



private:
    int m_coll, m_row;
    int m_xpos, m_ypos;
    int m_xr, m_yr;

    int m_xoffset = 10;
    int m_yoffset = 25;

    int m_cell_size = 0;

    QColor m_color;

    std::variant<IdleState, DestroyingState, FallingState, SpawningState> m_state = IdleState{};

};

// Idle state
inline void IdleState::update(CircleObject& owner, double dt)
{
    return;
}
inline void IdleState::render(const CircleObject& owner, QPainter& painter) const
{
    painter.setBrush(owner.getColor());
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(owner.getCenter(), owner.getRadius(), owner.getRadius());
}

// Destory state
inline void DestroyingState::update(CircleObject& owner, double dt)
{
    start_radius -= 100.0 * dt;
    if (start_radius < 0) {
        start_radius = 0;
    }
    owner.setRadius(static_cast<int>(start_radius));
}

inline void DestroyingState::render(const CircleObject& owner, QPainter& painter) const
{
    painter.setBrush(owner.getColor());
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(owner.getCenter(), start_radius, start_radius);
}

// Falling state
inline void FallingState::update(CircleObject& owner, double dt)
{
    double current_y = static_cast<double>(owner.getCenter().y());

    current_y += fall_speed * dt;


    if(current_y >= target_y)
    {
        owner.setGridRowPosition(target_row);

        owner.setPosition(QPoint{owner.getGridPos().first * owner.getCellSize(), target_row * owner.getCellSize()});
        owner.Idle();
    }
    else
    {
        int new_visual_y = static_cast<int>(current_y - owner.getCellSize() / 2.0);
        owner.setPosition(QPoint{owner.getGridPos().first * owner.getCellSize(), new_visual_y});
    }

}

inline void FallingState::render(const CircleObject& owner, QPainter& painter) const
{
    painter.setBrush(owner.getColor());
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(owner.getCenter(), owner.getRadius(), owner.getRadius());
}

// Spawning state

inline void SpawningState::update(CircleObject& owner, double dt)
{
    current_radius += 100.0 * dt;
    if (current_radius >= target_radius) {
        current_radius = target_radius;
        owner.Idle();
    }
    owner.setRadius(static_cast<int>(current_radius));
}

inline void SpawningState::render(const CircleObject& owner, QPainter& painter) const
{
    painter.setBrush(owner.getColor());
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(owner.getCenter(), owner.getRadius(), owner.getRadius());
}


#endif // IGAMEOBJECT_H
