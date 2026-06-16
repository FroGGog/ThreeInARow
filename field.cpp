#include "field.h"


Field::Field()
{
    for(size_t row = 0; row < 10; row++)
    {
        for(size_t coll = 0; coll < 10; coll++)
        {
            ObjectColor color = pickRandomColor();
            // horizontal check
            if(coll >= 2)
            {
                // TODO: add nullptr check
                while(m_circles[row][coll - 1] && m_circles[row][coll - 2] &&
                       m_circles[row][coll - 1]->getColor() == convertColor(color) &&
                    m_circles[row][coll - 2]->getColor() == convertColor(color))
                {
                    color = pickRandomColor();
                }

            }
            // vertical check
            if(row >= 2)
            {
                while(m_circles[row - 1][coll] && m_circles[row - 2][coll] &&
                       m_circles[row - 1][coll]->getColor() == convertColor(color) &&
                       m_circles[row - 2][coll]->getColor() == convertColor(color))
                {
                    color = pickRandomColor();
                }
            }

            auto obj = m_objects_pool.create(coll, row, color);

            if(obj != nullptr)
            {
                // update geometry with default base value
                obj->updateGeometry(64);
                m_circles[row][coll] = std::move(obj);
            }


        }
    }
}

void Field::update(double dt)
{
    for(const auto& row : m_circles)
    {
        for(const auto& elem : row)
        {
            if(elem != nullptr)
            {
                elem->update(dt);
            }
        }
    }

    processDestroyedObjects();
    proceedGravity();
    spawnNewObjects();
    findAndMarkMatches();
}

void Field::render(QPainter& painter)
{

    for(int row = 0; row < 10; ++row)
    {
        for(int coll = 0; coll < 10; ++coll)
        {
            if(m_circles[row][coll] != nullptr)
            {
                m_circles[row][coll]->render(painter);

                if(m_selected_pos.has_value() &&
                    m_selected_pos->first == row &&
                    m_selected_pos->second == coll)
                {
                    painter.setPen(QPen(Qt::white, 3));
                    painter.setBrush(Qt::NoBrush);
                    painter.drawEllipse(m_circles[row][coll]->getCenter(),
                                        m_circles[row][coll]->getRadius() + 4,
                                        m_circles[row][coll]->getRadius() + 4);
                }
            }
        }
    }
}

void Field::resize(double cell_size)
{
    m_cell_size = cell_size;

    for(const auto& row : m_circles)
    {
        for(const auto& elem : row)
        {
            if(elem != nullptr)
            {
                elem->updateGeometry(cell_size);
            }

        }
    }
}

void Field::click(int x, int y)
{
    std::pair<int, int> clicked_pos = {-1, -1};

    for(int row = 0; row < 10; ++row)
    {
        for(int coll = 0; coll < 10; ++coll)
        {
            if(m_circles[row][coll] != nullptr)
            {
                int dx = x - m_circles[row][coll]->getCenter().x();
                int dy = y - m_circles[row][coll]->getCenter().y();
                int distance_squared = dx * dx + dy * dy;

                int r = m_circles[row][coll]->getRadius();
                int r_squared = r * r;

                if(distance_squared <= r_squared)
                {
                    clicked_pos = {row, coll};
                }
            }
        }
    }
    if(clicked_pos.first == -1)
    {
        m_selected_pos.reset();
        return;
    }

    if(!m_selected_pos.has_value())
    {
        m_selected_pos = clicked_pos;
        if(m_circles[m_selected_pos->first][m_selected_pos->second] &&
            m_circles[m_selected_pos->first][m_selected_pos->second]->isBomb())
        {
            triggerBomb(m_circles[m_selected_pos->first][m_selected_pos->second].get());
            m_selected_pos.reset();
        }
    }
    else
    {
        int r1 = m_selected_pos->first;
        int c1 = m_selected_pos->second;
        int r2 = clicked_pos.first;
        int c2 = clicked_pos.second;

        bool is_adjacent = (std::abs(r1 - r2) + std::abs(c1 - c2)) == 1;

        if(is_adjacent)
        {

            std::swap(m_circles[r1][c1], m_circles[r2][c2]);

            if(m_circles[r1][c1])
            {
                m_circles[r1][c1]->setGridPos(r1, c1);
            }
            if(m_circles[r2][c2])
            {
                m_circles[r2][c2]->setGridPos(r2, c2);
            }

            findAndMarkMatches();

            m_selected_pos.reset();
        }
        else
        {
            m_selected_pos = clicked_pos;
        }

    }
}
