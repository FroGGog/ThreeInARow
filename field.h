#ifndef FIELD_H
#define FIELD_H

#include <QPainter>

class Field
{
public:
    Field() = default;

    void update(double dt);

    void render(QPainter& painter);

    void click(int x, int y);

};



#endif // FIELD_H
