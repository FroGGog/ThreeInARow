#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include <QPainter>

class IGameObject
{
public:

    virtual ~IGameObject() = default;

    virtual void update(double dt) = 0;

    virtual void render(QPainter& painter) const = 0;

private:

};


#endif // IGAMEOBJECT_H
