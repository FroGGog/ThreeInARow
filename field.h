#ifndef FIELD_H
#define FIELD_H

#include <QPainter>
#include <QDebug>
#include <memory>
#include <set>

#include "memory_pool.h"
#include "igameobject.h"

class Field
{
public:
    Field();

    void update(double dt);

    void render(QPainter& painter);
    void resize(double cell_size);

    void click(int x, int y);

private:

    using PoolType = MemoryPool<CircleObject, 100>;
    using CircleObjPtr = std::unique_ptr<CircleObject, PoolType::PoolDeleter>;

    std::array<std::array<CircleObjPtr, 10>, 10> m_circles;

    std::set<std::pair<int, int>> m_coord_to_delete;

    PoolType m_objects_pool;

    int m_cell_size;

    void findAndMarkMatches()
    {
        for(size_t row = 0; row < 10; ++row)
        {
            for(size_t coll = 0; coll + 2 < 10; ++coll)
            {
                if(m_circles[row][coll] == nullptr ||
                    m_circles[row][coll + 1] == nullptr ||
                    m_circles[row][coll + 2] == nullptr)
                {
                    continue;
                }

                // all three in a row equal
                if(m_circles[row][coll]->getColor() == m_circles[row][coll + 1]->getColor() &&
                        m_circles[row][coll + 1]->getColor() == m_circles[row][coll + 2]->getColor())
                {
                    QColor saved_color = m_circles[row][coll]->getColor();

                    size_t start = coll;

                    while(start < 10 && m_circles[row][start] != nullptr && m_circles[row][start]->getColor() == saved_color)
                    {
                        m_coord_to_delete.insert(std::pair<int, int>{row, start});
                        start++;
                    }
                    coll = start - 1;
                }
            }
        }

        for(size_t coll = 0; coll < 10; ++coll)
        {
            for(size_t row = 0; row + 2 < 10; ++row)
            {
                if(m_circles[row][coll] == nullptr || m_circles[row + 1][coll] == nullptr ||
                    m_circles[row + 2][coll] == nullptr)
                {
                    continue;
                }

                // all three in a collumn equal
                if(m_circles[row][coll]->getColor() == m_circles[row + 1][coll]->getColor() &&
                    m_circles[row + 1][coll]->getColor() == m_circles[row + 2][coll]->getColor())
                {
                    QColor saved_color = m_circles[row][coll]->getColor();

                    size_t start = row;
                    while(start < 10 && m_circles[start][coll] != nullptr && m_circles[start][coll]->getColor() == saved_color)
                    {
                        m_coord_to_delete.insert(std::pair<int, int>{start, coll});
                        start++;
                    }
                    row = start - 1;
                }
            }
        }
        destroyMarkedObjects();

    }

    void processDestroyedObjects()
    {
        for(size_t coll = 0; coll < 10; ++coll)
        {
            for(size_t row = 0; row < 10; ++row)
            {
                if(m_circles[row][coll] != nullptr && m_circles[row][coll]->getRadius() <= 0)
                {
                    m_circles[row][coll] = nullptr;
                }
            }
        }
    }

    void proceedGravity()
    {
        for(size_t coll = 0; coll < 10; ++coll)
        {
            for(int row = 9; row >= 0; --row)
            {
                if(m_circles[row][coll] == nullptr)
                {
                    for(int k = row - 1; k >= 0; --k)
                    {
                        if(m_circles[k][coll] != nullptr && m_circles[k][coll]->isIdle())
                        {
                            auto falling_obj = std::move(m_circles[k][coll]);
                            falling_obj->startFalling(row);
                            m_circles[row][coll] = std::move(falling_obj);
                            break;
                        }
                    }
                }
            }
        }
    }

    void spawnNewObjects()
    {
        for(size_t coll = 0; coll < 10; ++coll)
        {
            if(m_circles[0][coll] == nullptr &&!m_circles[1][coll]->isFalling())
            {
                ObjectColor color = pickRandomColor();
                if(coll >= 2)
                {
                    while(m_circles[0][coll - 1] && m_circles[0][coll - 2] &&
                        m_circles[0][coll - 1]->getColor() == convertColor(color) &&
                           m_circles[0][coll - 2]->getColor() == convertColor(color))
                    {
                        color = pickRandomColor();
                    }
                }

                while(m_circles[1][coll] && m_circles[2][coll] && m_circles[1][coll]->getColor() == convertColor(color)
                        && m_circles[2][coll]->getColor() == convertColor(color))
                {
                    color = pickRandomColor();
                }

                auto obj = m_objects_pool.create(coll, 0, color);

                if(obj != nullptr)
                {
                    // 1% bomb chance
                    if(QRandomGenerator::global()->bounded(100) < 1)
                    {
                        obj->setBomb(true);
                    }
                    // update geometry with default base value
                    obj->updateGeometry(m_cell_size);
                    obj->Spawn();
                    m_circles[0][coll] = std::move(obj);

                }
            }
            else
            {
                continue;
            }
        }
    }

    void triggerBomb(CircleObject* ptr)
    {
        auto bomb_color = ptr->getColor();

        for(size_t coll = 0; coll < 10; ++coll)
        {
            for(size_t row = 0; row < 10; ++row)
            {

                if(m_circles[row][coll] && m_circles[row][coll]->getColor() == bomb_color)
                {
                    m_coord_to_delete.insert({row, coll});
                }
            }
        }
        destroyMarkedObjects();
    }

    void destroyMarkedObjects()
    {
        for(const auto& elem : m_coord_to_delete)
        {
            m_circles[elem.first][elem.second]->Destroy();
        }
        m_coord_to_delete.clear();
    }


};


#endif // FIELD_H
