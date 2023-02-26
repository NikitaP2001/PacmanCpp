#pragma once

#include <stdexcept>

#include <d2d1helper.h>

namespace Cell {
    
    using position = D2D_POINT_2F;

    position operator*(position left, std::size_t scale) noexcept;

    bool operator!=(position left, position right);

    enum class Direction {
        undefined,
        up,
        left,
        down,
        right,
    };

    /* @throw logic arror*/
    Direction targetDirection(position current, position target);

    /* @throw logic arror*/
    position directionToTarget(position current, Direction direction);

    struct Wall {

        D2D_RECT_U rect;

        bool operator<(const Wall& other) const;

    };

};
