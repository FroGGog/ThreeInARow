#ifndef FIELD_H
#define FIELD_H

#include <QPainter>
#include <QDebug>
#include <memory>

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

    PoolType m_objects_pool;
};


#endif // FIELD_H
