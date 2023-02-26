#include <stdexcept>

#include "Entity.h"

using namespace Cell;

Cell::position Creature::position() const
{
    Cell::position pos = Entity::position();
    if (isMoving()) {
        auto direction = Cell::targetDirection(pos, m_targetPos);
        switch (direction) {        
        case Cell::Direction::up:
            pos.y -= m_transferState;
            break;
        case Cell::Direction::left:
            pos.x -= m_transferState;
            break;
        case Cell::Direction::down:
            pos.y += m_transferState;
            break;
        case Cell::Direction::right:
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