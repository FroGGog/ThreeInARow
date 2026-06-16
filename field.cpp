#include "field.h"


Field::Field()
{
    for(size_t i = 0; i < 10; i++)
    {
        for(size_t j = 0; j < 10; j++)
        {
            ObjectColor color = pickRandomColor();
            // horizontal check
            if(j >= 2)
            {
                while(m_circles[i][j - 1]->getColor() == convertColor(color) &&
                    m_circles[i][j - 2]->getColor() == convertColor(color))
                {
                    color = pickRandomColor();
                }

            }
            // vertical check
            if(i >= 2)
            {
                while(m_circles[i-1][j] && m_circles[i-2][j] &&
                       m_circles[i-1][j]->getColor() == convertColor(color) &&
                       m_circles[i-2][j]->getColor() == convertColor(color))
                {
                    color = pickRandomColor();
                }
            }

            auto obj = m_objects_pool.create(i, j, color);
            // update geometry with default base value
            obj->updateGeometry(64);
            if(obj != nullptr)
            {
                m_circles[i][j] = std::move(obj);
            }


        }
    }
}

void Field::update(double dt)
{

}

void Field::render(QPainter& painter)
{

    for(const auto& row : m_circles)
    {
        for(const auto& elem : row)
        {
            elem->render(painter);
        }

    }
}

void Field::resize(double cell_size)
{
    for(const auto& row : m_circles)
    {
        for(const auto& elem : row)
        {
            elem->updateGeometry(cell_size);
        }
    }
}

void Field::click(int x, int y)
{

}
