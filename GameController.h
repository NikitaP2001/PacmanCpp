#pragma once

#include <vector>
#include <map>

#include "Entity.h"
#include "MazeModel.h"
#include "SearchPath.h"

class GameModel {

public:

    GameModel() noexcept;

    void playerControlEvent();

    void keyEvent();

    void stepEvent();

    MazeModel* maze()
    {
        return &m_maze;
    }

    std::vector<Entity*> entities()
    {
        std::vector<Entity*> all;
        all.push_back(&m_pacman);
        for (auto& entry : m_food)
            if (!entry.isEaten())
                all.push_back(&entry);        
        return all;
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

    MazePath pathFinder;

    Pacman m_pacman;

    std::vector<Food> m_food;

    Food* targetFood = nullptr;


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