#include <thread>
#include <chrono>

#include "GameController.h"

using namespace Cell;

GameModel::GameModel() noexcept
    : m_pacman()
{    
    D2D_SIZE_U size = m_maze.mapSize();
    m_food.resize(static_cast<std::size_t>(size.height) * size.width);
    for (UINT ix = 0; ix < size.width; ix++) {
        for (UINT iy = 0; iy < size.height; iy++) {
            Cell::position pos{ static_cast<float>(ix), static_cast<float>(iy) };
            m_food[ix * size.height + iy].setPosition(pos);
        }
    }
}


void GameModel::keyEvent()
{
    if (m_kKeyToDirection.contains(m_lastPlayerKey) && !m_pacman.isMoving()) {
        auto direction = m_kKeyToDirection.at(m_lastPlayerKey);
        Cell::position currPos = m_pacman.position();
        Cell::position target = Cell::directionToTarget(currPos, direction);
        if (m_maze.canMoveByDirection(currPos, direction))
            m_pacman.setTarget(target);
    }
}


void GameModel::stepEvent()
{
    auto ent = entities();

    m_pacman.moveEvent();

    for (auto itFood = m_food.begin(); itFood != m_food.end(); itFood++) {
        if (itFood->position() == m_pacman.position()) {
            m_food.erase(itFood);
            break;
        }
    }    
}


void GameController::init()
{

}


void GameController::run()
{
    try {
        while (m_continue) {

            m_model.keyEvent();

            m_model.stepEvent();


            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    catch (const std::logic_error& ex) {
        MessageBoxA(0, ex.what(), "Logic error", MB_OK | MB_ICONWARNING);
    }
}


void GameController::setState(bool state) noexcept
{
    m_continue = state;
}