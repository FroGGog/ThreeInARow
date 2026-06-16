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

    for(const auto& row : m_circles)
    {
        for(const auto& elem : row)
        {
            if(elem != nullptr)
            {
                elem->render(painter);
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
    for(const auto& row : m_circles)
    {
        for(const auto& elem : row)
        {
            if(elem != nullptr)
            {
                int dx = x - elem->getCenter().x();
                int dy = y - elem->getCenter().y();
                int distance_squared = dx * dx + dy * dy;

                int r = elem->getRadius();
                int r_squared = r * r;

                if(distance_squared <= r_squared)
                {
                    qDebug() << "Got collision\n";
                    if(elem->isBomb())
                    {
                        triggerBomb(elem.get());
                    }
                    else
                    {
                        elem->setColor(Qt::cyan);
                        findAndMarkMatches();
                    }

                }

            }
        }
    }
}
