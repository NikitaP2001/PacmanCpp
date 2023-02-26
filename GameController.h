#pragma once

#include <vector>
#include <map>

#include "Entity.h"
#include "MazeModel.h"

class GameModel {

public:

    GameModel() noexcept;

    void keyEvent()
    {
        if (m_kKeyToDirection.contains(m_lastPlayerKey) && !m_pacman.isMoving()) {
            auto direction = m_kKeyToDirection.at(m_lastPlayerKey);
            Cell::position target = Cell::directionToTarget(m_pacman.position(), direction);
            m_pacman.setTarget(target);
        }        
    }

    void stepEvent();

    MazeModel* maze()
    {
        return &m_maze;
    }

    std::vector<Entity*> entities()
    {
        return { &m_pacman };
    }

    void setPlayerKey(SHORT vKeyCode)
    {
        m_lastPlayerKey = vKeyCode;
    }


private:

    inline static const std::map<SHORT, Cell::Direction> m_kKeyToDirection = {
        {'W', Cell::Direction::up },
        {'A', Cell::Direction::left},
        {'D', Cell::Direction::right},
        {'S', Cell::Direction::down},
    };

    SHORT m_lastPlayerKey = 0;

    MazeModel m_maze;

    Pacman m_pacman;

    std::vector<Food> m_food;


};

class GameController {

public:

    GameController() = default;

    void init();    

    void run();

    void setState(bool state) noexcept;

    GameModel* model() noexcept
    {
        return &m_model;
    }

private:



private:

    bool m_continue = true;


    GameModel m_model;

};