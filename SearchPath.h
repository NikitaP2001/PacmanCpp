#pragma once

#include <set>

#include "Cell.h"

class MazePath {

    struct cell {

        int parent_i = -1;
        int parent_j = -1;

        double f = FLT_MAX;
        double g = FLT_MAX;
        double h = FLT_MAX;
    };

public:
    using byte_grid = std::vector<std::vector<uint8_t>>;
    using Coord = std::pair<int, int>;
    using pPair = std::pair<double, Coord>;

public:

    MazePath(std::size_t rows, std::size_t cols, std::set<Cell::Wall> walls);

    Cell::position search(Coord src, Coord dest);    

private:

    void init(Coord src);

    Coord tracePath();

    _NODISCARD double calculateHValue(int row, int col, MazePath::Coord dest) const noexcept;

    _NODISCARD bool isDestination(int row, int col, MazePath::Coord dest) const noexcept;

    _NODISCARD bool isUnBlocked(int row, int col) const noexcept;

    _NODISCARD bool isValid(int row, int col) const noexcept;

    _NODISCARD bool rollToCell(Coord newPos, Coord dest);

private:

    byte_grid m_grid;    

    std::set<Cell::Wall> m_walls;

    Coord m_currPos;

    const std::size_t m_kRow;
    const std::size_t m_kCol;

    std::vector<std::vector<cell>> m_cellDetails;

    std::vector<std::vector<uint8_t>> m_closedList;

    std::set<pPair> m_openList;

};
