#include <vector>
#include <list>

#include "Cell.h"
#include "Tool.h"

namespace Cell {

    position operator*(position left, std::size_t scale) noexcept
    {
        return { left.x * scale, left.y * scale };
    }


    bool operator!=(position left, position right)
    {
        return (left.x != right.x) || (left.y != right.y);
    }

    bool operator==(position left, position right)
    {
        return !operator!=(left, right);
    }


    Direction targetDirection(position current, position target)
    {
        Direction dir = Direction::undefined;
        if (current.x != target.x || current.y != target.y) {
            if (current.x == target.x) {
                if (current.y > target.y)
                    dir = Direction::up;
                else
                    dir = Direction::down;
            }
            else if (current.y == target.y) {
                if (current.x > target.x)
                    dir = Direction::left;
                else
                    dir = Direction::right;
            }
            else
                throw std::logic_error("impossible to define direction");
        }
        return dir;
    }


    position directionToTarget(position current, Direction direction)
    {        
        switch (direction) {
        case Direction::up:
            current.y = round(current.y) - 1;
            break;
        case Direction::down:
            current.y = round(current.y) + 1;
            break;
        case Direction::left:
            current.x = round(current.x) - 1;
            break;
        case Direction::right:
            current.x = round(current.x) + 1;
            break;
        default:
            throw std::logic_error("impossible to determine target");
        }
        return current;
    }


    bool Wall::operator<(const Wall& other) const
    {
        bool less = false;

        if (rect.left != other.rect.left || rect.top != other.rect.top)
            less = rect.left < other.rect.left || (rect.left == other.rect.left && rect.top < other.rect.top);
        else
            less = rect.right < other.rect.right || (rect.right == other.rect.right && rect.bottom < other.rect.bottom);
        return less;
    }

    Wall crossedWallByDirectioin(position current, Direction way)
    {
        Wall crossed;

        switch (way) {
        case Direction::up:
            crossed.rect.left = current.x;
            crossed.rect.top = current.y;
            crossed.rect.right = current.x + 1;
            crossed.rect.bottom = current.y;
            break;
        case Direction::down:
            crossed.rect.left = current.x;
            crossed.rect.top = current.y + 1;
            crossed.rect.right = current.x + 1;
            crossed.rect.bottom = current.y + 1;            
            break;
        case Direction::left:
            crossed.rect.left = current.x;
            crossed.rect.top = current.y;
            crossed.rect.right = current.x;
            crossed.rect.bottom = current.y + 1;
            break;
        case Direction::right:
            crossed.rect.left = current.x + 1;
            crossed.rect.top = current.y;
            crossed.rect.right = current.x + 1;
            crossed.rect.bottom = current.y + 1;            
            break;
        default:
            throw std::logic_error("invalid way value");
        }
        return crossed;
    }

    struct Cell {

        D2D_POINT_2U leftUpper;

        bool operator<(const Cell& other) const
        {            
            return leftUpper.x < other.leftUpper.x
                || (leftUpper.x == other.leftUpper.x && leftUpper.y < other.leftUpper.y);
        }
    };    


    static std::list<Wall> getAllWalls(D2D_SIZE_U fieldSize)
    {
        std::list<Wall> walls;
        UINT width = fieldSize.width;
        UINT height = fieldSize.height;
        for (UINT32 ix = 0; ix < width; ix++) {

            for (UINT32 iy = 0; iy < height; iy++) {
                if (iy == height - 1)
                    walls.push_back({ ix, iy + 1, ix + 1, iy + 1 });

                if (ix == 0) {                    
                    walls.push_back({ ix, iy, ix + 1, iy });
                    walls.push_back({ ix, iy, ix, iy + 1 });
                    walls.push_back({ ix + 1, iy, ix + 1, iy + 1 });
                }
                else {                    
                    walls.push_back({ ix, iy, ix + 1, iy });
                    walls.push_back({ ix + 1, iy, ix + 1, iy + 1 });
                }
            }

        }
        return walls;
    }


    static std::vector<std::set<Cell>> getAllCells(D2D_SIZE_U fieldSize)
    {
        std::vector<std::set<Cell>> cells;
        for (UINT32 ix = 0; ix < fieldSize.width; ix++) {

            for (UINT32 iy = 0; iy < fieldSize.height; iy++) {
                Cell newCell{ ix, iy };
                cells.push_back({ newCell });
            }                

        }
        return cells;
    }


    bool isWallCorner(D2D_SIZE_U fieldSize, const Wall &wall)
    {
        bool corner = false;
        auto&& wallRec = wall.rect;
        if ((wallRec.left == 0 && wallRec.right == 0) || (wallRec.top == 0 && wallRec.bottom == 0) 
            || (wallRec.left == fieldSize.width && wallRec.right == fieldSize.width) 
            || (wallRec.top == fieldSize.height && wallRec.bottom == fieldSize.height))        
            corner = true;
        return corner;
    }


    void createMaze(D2D_SIZE_U fieldSize, std::set<Wall>& newWalls)
    {
        std::list<Wall> allWalls = getAllWalls(fieldSize);
        std::vector<std::set<Cell>> cells = getAllCells(fieldSize);

        while (!allWalls.empty()) {
            auto itWall = select_randomly(allWalls.begin(), allWalls.end());            
            if (!isWallCorner(fieldSize, *itWall)) {

                Cell cellLeft, cellRight;
                if (itWall->rect.bottom == itWall->rect.top) {
                    cellLeft = { itWall->rect.left, itWall->rect.bottom - 1 };
                    cellRight = { itWall->rect.left, itWall->rect.bottom};
                } else {
                    cellLeft = { itWall->rect.left - 1, itWall->rect.top };
                    cellRight = { itWall->rect.left, itWall->rect.top };
                }

                auto itFirstCell = cells.end();
                auto itSecondCell = cells.end();

                for (auto itCells = cells.begin(); itCells != cells.end(); itCells++) {
                    if (itFirstCell == cells.end() && itCells->contains(cellLeft))
                        itFirstCell = itCells;
                    if (itSecondCell == cells.end() && itCells->contains(cellRight))
                        itSecondCell = itCells;
                }                                               
                if (itFirstCell != itSecondCell) {
                    itFirstCell->insert(itSecondCell->begin(), itSecondCell->end());
                    cells.erase(itSecondCell);
                    allWalls.erase(itWall);
                    continue;
                }
            }
            newWalls.insert(*itWall);
            allWalls.erase(itWall);
        }        
    }

}