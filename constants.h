#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace match3
{
    inline constexpr int BOMB_CHANCE = 1;

    inline constexpr int GRID_SIZE = 10;
    inline constexpr int POOL_SIZE = GRID_SIZE * GRID_SIZE;

    inline constexpr double SPAW_SPEED = 100.0;
    inline constexpr double DESTROY_SPEED = 100.0;

    inline constexpr int COLOR_COUNT = 6;
    inline constexpr int COLOR_MIN = 1;
    inline constexpr int COLOR_MAX = 7; // not included

    inline constexpr double CIRCLE_RADIUS_RATIO = 0.4;

    inline constexpr double BOMB_FLICKER_BASE_ALPHA = 150.0;
    inline constexpr double BOMB_FLICKER_ALPHA_RANGE = 105.0;
    inline constexpr double BOMB_FLICKER_FREQUENCY = 6.0;

    inline constexpr int DEFAULT_CELL_SIZE = 64;

    inline constexpr int SELECTION_RING_THICKNESS = 3;
    inline constexpr int SELECTION_RING_OFFSET = 4;


}

#endif // CONSTANTS_H
