#pragma once

#include <set>
#include <vector>

#include "Cell.h"
#include "Entity.h"

class MazeModel {

public:

    MazeModel();    

    D2D_SIZE_U mapSize() const noexcept;

    std::vector<Cell::Wall> getWalls(bool getAll);

private:

    /* @throw logic_error, bad_alloc */
    void addWall(D2D_RECT_U position);

private:

    std::set<Cell::Wall> walls;

    D2D_SIZE_U m_mapSize{ 32, 24 };    
    
    static constexpr const char* m_kInvalidPos = "Given position is invalid in current map context";
};