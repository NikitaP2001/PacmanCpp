#pragma once

#include <map>
#include <chrono>

#include <d2d1helper.h>

#include "Sprite.h"
#include "Cell.h"


class Entity {

public:

public:        

    D2D_RECT_F sprite();

    virtual Cell::position position() const;

    void setPosition(Cell::position newPosition) noexcept;

    virtual ~Entity() = default;

protected:
  
    void setDirection(Cell::Direction direction) noexcept;

    void setSprite(const Sprite& newSprite);

    void setAnimPos(int number);

private:                

    Cell::position m_position{};

    Sprite m_sprite;

};


class Food : public Entity {

public:

    Food()
    {
        Sprite pacmanSprite;
        using dir = Cell::Direction;
        pacmanSprite.setSpriteForDirection({ 185, 515 }, dir::undefined);        
        setSprite(pacmanSprite);
    }

    _NODISCARD bool isEaten()
    {
        return m_isEaten;
    }

    void eat()
    {
        m_isEaten = true;
    }

private:

    bool m_isEaten = false;

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

    Pacman();

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