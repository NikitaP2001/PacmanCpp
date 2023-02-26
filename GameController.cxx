#include <thread>
#include <chrono>

#include "GameController.h"

GameModel::GameModel() noexcept
    : m_pacman()
{    
}


void GameModel::stepEvent()
{
    auto ent = entities();

    for (auto &entity : ent) {

        Creature* creature;
        if (creature = dynamic_cast<Creature*>(entity)) {

            creature->moveEvent();

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