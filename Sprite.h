#pragma once

#include <stdexcept>

#include "Cell.h"

class Sprite {
    using sheet_pos = D2D_POINT_2F;
public:

    explicit Sprite(int nStates = 1)
        : m_maxStates(nStates)
    {

    }

    void setSpriteNumber(int number)
    {
        if (m_spriteByDirection.empty())
            throw std::logic_error("No sprite set");
        if (number < 0 || number > m_maxStates)
            throw std::logic_error("Invilid sprite number");
        m_state = number;        
    }

    D2D_RECT_F current()
    {
        D2D_RECT_F rect{};
        if (m_spriteByDirection.contains(m_direction)) {
            auto leftUpper = m_spriteByDirection[m_direction];
            leftUpper.x += m_state * nextSprite;
            rect.left = leftUpper.x;
            rect.top = leftUpper.y;
            rect.right = rect.left + m_spriteSize.width;
            rect.bottom = rect.top + m_spriteSize.height;
        } else
            throw std::logic_error("no sprite set");

        return rect;
    }

    void setDirection(Cell::Direction newDirection) noexcept
    {
        m_direction = newDirection;
    }

    /* @throw bad alloc */
    void setSpriteForDirection(sheet_pos sprite, Cell::Direction direction)
    {
        m_spriteByDirection[direction] = sprite;
    }

private:

    int m_state = 0;
    int m_maxStates = 0;

    Cell::Direction m_direction = Cell::Direction::undefined;

    sheet_pos m_currSprite{};

    std::map<Cell::Direction, sheet_pos> m_spriteByDirection;

    static constexpr D2D_SIZE_F m_spriteSize{ 15, 15 };
    static constexpr int nextSprite = 17;
};