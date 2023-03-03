#include <thread>
#include <chrono>

#include "GameController.h"

using namespace Cell;

GameModel::GameModel() noexcept
    : m_pacman(), 
    pathFinder(m_maze.mapSize().width, m_maze.mapSize().height, m_maze.getWallset())
{    
    D2D_SIZE_U size = m_maze.mapSize();
    m_food.resize(static_cast<std::size_t>(size.height) * size.width);
    for (UINT ix = 0; ix < size.width; ix++) {
        for (UINT iy = 0; iy < size.height; iy++) {
            Cell::position pos{ static_cast<float>(ix), static_cast<float>(iy) };
            std::size_t index = ix * size.height + iy;
            m_food[index].setPosition(pos);
        }
    }
}


void GameModel::playerControlEvent()
{
    auto direction = m_kKeyToDirection.at(m_lastPlayerKey);
    Cell::position currPos = m_pacman.position();
    Cell::position target = Cell::directionToTarget(currPos, direction);
    if (m_maze.canMoveByDirection(currPos, direction))
        m_pacman.setTarget(target);
}


void GameModel::keyEvent()
{
    if (!m_pacman.isMoving()) {

        if (m_kKeyToDirection.contains(m_lastPlayerKey)) {            

            playerControlEvent();

        } else {           
                                    
            Cell::position currPos = m_pacman.position();
            if (targetFood == nullptr || targetFood->isEaten()) {

                auto foodIt = m_food.begin();
                while (foodIt != m_food.end() && foodIt->isEaten())
                    foodIt++;

                targetFood = foodIt != m_food.end() ? &*foodIt : nullptr;
                auto sqr = [](float val) -> float
                {
                    return val * val;
                };
                auto dist = [sqr](Cell::position first, Cell::position second) -> float
                {
                    return sqrt(sqr(first.x - second.x) + sqr(first.y - second.y));
                };
                while (foodIt != m_food.end()) {
                    float distToCurrFood = dist(currPos, foodIt->position());
                    float distToTarFood = dist(targetFood->position(), foodIt->position());
                    if (distToCurrFood < distToTarFood && !foodIt->isEaten())                        
                        targetFood = &*foodIt;                   
                    foodIt++;
                }
            }

            if (targetFood != nullptr && !targetFood->isEaten()) {
                Cell::position nearFoodPos = targetFood->position();
                MazePath::Coord src = { static_cast<std::size_t>(currPos.x), static_cast<std::size_t>(currPos.y) };
                MazePath::Coord cTarget = { static_cast<std::size_t>(nearFoodPos.x), static_cast<std::size_t>(nearFoodPos.y) };
                Cell::position target = pathFinder.search(src, cTarget);
                m_pacman.setTarget(target);
            }                        
                     
        }

    }
    
}


void GameModel::stepEvent()
{
    auto ent = entities();

    m_pacman.moveEvent();

    for (auto itFood = m_food.begin(); itFood != m_food.end(); itFood++) {
        if (itFood->position() == m_pacman.position()) {
            itFood->eat();
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