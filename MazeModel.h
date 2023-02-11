#pragma once

#include <set>
#include <vector>

#include "Cell.h"
#include "Entity.h"

class MazeModel {

public:

    MazeModel();

    std::size_t mapWidth() const noexcept;

    std::size_t mapHeight() const noexcept;

    std::vector<Wall> getWalls(bool getAll);

    std::vector<Entity> getEntities();

    void eventLoop();

private:

    /* @throw logic_error, bad_alloc */
    void addWall(cell_pos_t xUL, cell_pos_t yUL, cell_pos_t xRX, cell_pos_t yRD);

private:

    std::set<Wall> walls;

    cell_pos_t m_mapWidth = 32;
    cell_pos_t m_mapHeigth = 24;
    
    static constexpr char* m_kInvalidPos = "Given position is invalid in current map context";
};