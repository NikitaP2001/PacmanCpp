#pragma once

#include <map>
#include <chrono>

#include <d2d1helper.h>

#include "Sprite.h"
#include "Cell.h"


class Entity {

public:

public:

    explicit Entity()        
    {

    }

    D2D_RECT_F sprite()
    {
        return m_sprite.current();
    }

    virtual Cell::position position() const
    {
        return m_position;
    }

    void setPosition(Cell::position newPosition) noexcept
    {
        m_position = newPosition;
    }

    virtual ~Entity() = default;

protected:
  
    void setDirection(Cell::Direction direction) noexcept
    {
        m_sprite.setDirection(direction);
    }

    void setSprite(const Sprite& newSprite)
    {
        m_sprite = newSprite;
    }

    void setAnimPos(int number)
    {
        m_sprite.setSpriteNumber(number);
    }

private:                

    Cell::position m_position{};

    Sprite m_sprite;

};


class Food : public Entity {

};


class Creature : public Entity {

public:


    /* @throw logic error */
    void setTarget(Cell::position target)
    {
        m_targetPos = target;
        setDirection(Cell::targetDirection(position(), target));
    }

    void moveEvent();

    double getTransferState() noexcept
    {
        return m_transferState;
    }
    
    virtual Cell::position position() const override;


    void setSpeed(double speed) noexcept
    {
        m_speed = speed;
    }

    bool isMoving() const noexcept;

protected: 

    explicit Creature()
    {

    }

    virtual void moveSprite() = 0;

    float state()
    {
        return m_transferState;
    }

private:

    std::chrono::time_point<std::chrono::steady_clock> m_lastEvent{};

    /* speed in cells per second */
    double m_speed = 2.5;

    float m_transferState = 0;

    Cell::position m_targetPos{};

    static constexpr const int m_kMilisInSec = 1000;

    static constexpr std::chrono::time_point<std::chrono::steady_clock> m_kDefaultTime{};
};


class Pacman : public Creature {

public:

    explicit Pacman()
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

protected:

    void moveSprite() override;

private:

    static constexpr const int m_kAnimStateCount = 3;
    static constexpr const int m_kAnimCycles = 2;    
    static constexpr const float m_kMaxAnimState = 1.f / m_kAnimCycles;

};


class Ghost : public Creature {

public:

    explicit Ghost()
    {
        using dir = Cell::Direction;
        Sprite ghostSprite;        
        ghostSprite.setSpriteForDirection({ 134, 566 }, dir::undefined);
        ghostSprite.setSpriteForDirection({ 100, 566 }, dir::left);
        ghostSprite.setSpriteForDirection({ 134, 566 }, dir::up);
        ghostSprite.setSpriteForDirection({ 168, 566 }, dir::right);
        ghostSprite.setSpriteForDirection({ 202, 566 }, dir::down);
        setSprite(ghostSprite);
    }

private:

};