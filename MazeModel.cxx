#include <stdexcept>
#include <thread>

#include "MazeModel.h"

MazeModel::MazeModel()
{
    addWall({ 2, 2, 2, 3 });
    addWall({ 2, 3, 3, 3 });
    addWall({ 3, 3, 4, 3 });
}


D2D_SIZE_U MazeModel::mapSize() const noexcept
{
    return m_mapSize;
}


void MazeModel::addWall(D2D_RECT_U position)
{
    uint32_t xUL = position.left;
    uint32_t yUL = position.top;
    uint32_t xRD = position.right;
    uint32_t yRD = position.bottom;
    if (xRD - xUL <= 1 && yRD - yUL <= 1 && xUL < m_mapSize.width && xRD < m_mapSize.width
        && yUL < m_mapSize.height && yRD < m_mapSize.height)
    {
        walls.insert({ xUL, yUL, xRD, yRD });
    } else {
        throw std::logic_error(m_kInvalidPos);
    }
}


std::vector<Cell::Wall> MazeModel::getWalls(bool getAll)
{
    if (!getAll)
        throw std::logic_error("not supported");
    std::vector<Cell::Wall> result(walls.begin(), walls.end());            
    return result;
}