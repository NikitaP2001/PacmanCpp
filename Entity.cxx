#include <stdexcept>

#include "Entity.h"

using namespace Cell;


D2D_RECT_F Entity::sprite()
{
    return m_sprite.current();
}

position Entity::position() const
{
    return m_position;
}

void Entity::setPosition(Cell::position newPosition) noexcept
{
    m_position = newPosition;
}

void Entity::setDirection(Cell::Direction direction) noexcept
{
    m_sprite.setDirection(direction);
}


void Entity::setSprite(const Sprite & newSprite)
{
    m_sprite = newSprite;
}


void Entity::setAnimPos(int number)
{
    m_sprite.setSpriteNumber(number);
}


position Creature::position() const
{
    Cell::position pos = Entity::position();
    if (isMoving()) {
        auto direction = targetDirection(pos, m_targetPos);
        switch (direction) {        
        case Direction::up:
            pos.y -= m_transferState;
            break;
        case Direction::left:
            pos.x -= m_transferState;
            break;
        case Direction::down:
            pos.y += m_transferState;
            break;
        case Direction::right:
            pos.x += m_transferState;
            break;
        default:
            throw std::logic_error("Invalid direction");
            break;
        }
    }
    return pos;
}


void Creature::moveEvent()
{
    auto timeNow = std::chrono::steady_clock::now();
    if (isMoving()) {
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - m_lastEvent);
        double movement = (m_speed * duration.count()) / m_kMilisInSec;
        
        m_transferState += movement;
        if (m_transferState >= 1.f) {
            setPosition(m_targetPos);
            m_transferState = 0;
        }
        moveSprite();
    }

    m_lastEvent = timeNow;
}


bool Creature::isMoving() const noexcept
{
    return m_targetPos != Entity::position();
}


Pacman::Pacman()
{
    Sprite pacmanSprite(m_kAnimStateCount);
    using dir = Cell::Direction;
    pacmanSprite.setSpriteForDirection({ 100, 515 }, dir::undefined);
    pacmanSprite.setSpriteForDirection({ 100, 498 }, dir::left);
    pacmanSprite.setSpriteForDirection({ 100, 515 }, dir::up);
    pacmanSprite.setSpriteForDirection({ 100, 532 }, dir::right);
    pacmanSprite.setSpriteForDirection({ 100, 549 }, dir::down);
    setSprite(pacmanSprite);
}


void Pacman::moveSprite()
{        
    float animState = state();
    while (animState > m_kMaxAnimState) {
        animState -= m_kMaxAnimState;
    }    
    if (animState < m_kMaxAnimState / 3)
        Entity::setAnimPos(1);
    else if (animState < (m_kMaxAnimState * 2) / 3)
        Entity::setAnimPos(2);
    else
        Entity::setAnimPos(0);
}