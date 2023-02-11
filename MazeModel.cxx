#include <stdexcept>
#include <thread>

#include "MazeModel.h"

MazeModel::MazeModel()
{
    addWall(2, 2, 2, 3);
    addWall(2, 3, 3, 3);
    addWall(3, 3, 4, 3);
}


std::size_t MazeModel::mapWidth() const noexcept
{
    return m_mapWidth;
}


std::size_t MazeModel::mapHeight() const noexcept
{
    return m_mapHeigth;
}


void MazeModel::eventLoop()
{

}


void MazeModel::addWall(cell_pos_t xUL, cell_pos_t yUL, cell_pos_t xRD, cell_pos_t yRD)
{
    if (xRD - xUL <= 1 && yRD - yUL <= 1 && xUL < m_mapWidth && xRD < m_mapWidth
        && yUL < m_mapHeigth && yRD < m_mapHeigth)
    {
        walls.insert({ xUL, yUL, xRD, yRD });
    } else {
        throw std::logic_error(m_kInvalidPos);
    }
}


std::vector<Wall> MazeModel::getWalls(bool getAll)
{
    if (!getAll)
        throw std::logic_error("not supported");
    std::vector<Wall> result(walls.begin(), walls.end());            
    return result;
}