#include "Cell.h"

namespace Cell {

    position operator*(position left, std::size_t scale) noexcept
    {
        return { left.x * scale, left.y * scale };
    }


    bool operator!=(position left, position right)
    {
        return (left.x != right.x) || (left.y != right.y);
    }


    Direction targetDirection(position current, position target)
    {
        Direction dir = Direction::undefined;
        if (current.x != target.x || current.y != target.y) {
            if (current.x == target.x) {
                if (current.y > target.y)
                    dir = Direction::up;
                else
                    dir = Direction::down;
            }
            else if (current.y == target.y) {
                if (current.x > target.x)
                    dir = Direction::left;
                else
                    dir = Direction::right;
            }
            else
                throw std::logic_error("impossible to define direction");
        }
        return dir;
    }


    position directionToTarget(position current, Direction direction)
    {        
        switch (direction) {
        case Direction::up:
            current.y = round(current.y) - 1;
            break;
        case Direction::down:
            current.y = round(current.y) + 1;
            break;
        case Direction::left:
            current.x = round(current.x) - 1;
            break;
        case Direction::right:
            current.x = round(current.x) + 1;
            break;
        default:
            throw std::logic_error("impossible to determine target");
        }
        return current;
    }


    bool Wall::operator<(const Wall& other) const
    {
        bool less = false;

        if (rect.left != other.rect.left || rect.top != other.rect.top)
            less = rect.left < other.rect.left || (rect.left == other.rect.left && rect.top < other.rect.top);
        else
            less = rect.right < other.rect.right || (rect.right == other.rect.right && rect.bottom < other.rect.bottom);
        return less;
    }

}