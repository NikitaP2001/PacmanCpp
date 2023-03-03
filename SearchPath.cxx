#include <stdexcept>
#include <vector>
#include <stack>
#include <set>

#include "SearchPath.h"


MazePath::MazePath(std::size_t rows, std::size_t cols, std::set<Cell::Wall> walls)
    : m_kRow(rows), m_kCol(cols), m_walls(walls)
{

}


bool MazePath::isValid(int row, int col) const noexcept
{    
    return (row >= 0) && (row < m_kRow) && (col >= 0)
        && (col < m_kCol);
}


bool MazePath::isUnBlocked(int row, int col) const noexcept
{    
    Cell::position current { m_currPos.first, m_currPos.second };
    Cell::position target { row, col };
    auto dir = Cell::targetDirection(current, target);
    auto crossed = Cell::crossedWallByDirectioin(current, dir);

    return !m_walls.contains(crossed);
}


bool MazePath::isDestination(int row, int col, MazePath::Coord dest) const noexcept
{
    return (row == dest.first && col == dest.second);        
}

double MazePath::calculateHValue(int row, int col, MazePath::Coord dest) const noexcept
{
    return sqrt((row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second));
}

MazePath::Coord MazePath::tracePath()
{    
    int row = m_currPos.first;
    int col = m_currPos.second;

    Coord newCord{};

    while (!(m_cellDetails[row][col].parent_i == row
        && m_cellDetails[row][col].parent_j == col)) {        
        newCord = { row, col };
        int temp_row = m_cellDetails[row][col].parent_i;
        int temp_col = m_cellDetails[row][col].parent_j;        
        row = temp_row;
        col = temp_col;
    }    

    return newCord;
}


bool MazePath::rollToCell(Coord newPos, Coord dest)
{
    int i = newPos.first;
    int j = newPos.second; 
    double gNew = 0;
    double hNew = 0;
    double fNew = 0;

    if (isValid(i, j) && isUnBlocked(i, j)) {
                        
        if (isDestination(i, j, dest)) {            
            m_cellDetails[i][j].parent_i = m_currPos.first;
            m_cellDetails[i][j].parent_j = m_currPos.second;  
            m_currPos = newPos;            
            return true;

        } else if (!m_closedList[i][j]) {

            gNew = m_cellDetails[m_currPos.first][m_currPos.second].g + 1.0;
            hNew = calculateHValue(i, j, dest);
            fNew = gNew + hNew;

            if (m_cellDetails[i][j].f == FLT_MAX || m_cellDetails[i][j].f > fNew) {
                m_openList.insert(std::make_pair(fNew, std::make_pair(i, j)));                
                m_cellDetails[i][j].f = fNew;
                m_cellDetails[i][j].g = gNew;
                m_cellDetails[i][j].h = hNew;
                m_cellDetails[i][j].parent_i = m_currPos.first;
                m_cellDetails[i][j].parent_j = m_currPos.second;
            }
        }
    }    

    return false;
}


void MazePath::init(Coord src)
{    

    m_closedList = std::vector<std::vector<uint8_t>>(m_kRow, std::vector<uint8_t>(m_kCol));

    m_cellDetails = std::vector<std::vector<cell>>(m_kRow, std::vector<cell>(m_kCol));

    m_openList.clear();
    m_openList.insert(std::make_pair(0.0, std::make_pair(src.first, src.second)));
    
    m_cellDetails[src.first][src.second].f = 0.0;
    m_cellDetails[src.first][src.second].g = 0.0;
    m_cellDetails[src.first][src.second].h = 0.0;
    m_cellDetails[src.first][src.second].parent_i = src.first;
    m_cellDetails[src.first][src.second].parent_j = src.second;
}


Cell::position MazePath::search(Coord src, Coord dest)
{            
    Cell::position newPos{};
    if (isDestination(src.first, src.second, dest)) {        
        return { static_cast<float>(src.first), static_cast<float>(src.second) };
    }    

    init(src);        

    while (!m_openList.empty()) {
        pPair p = *m_openList.begin();
        
        m_openList.erase(m_openList.begin());
        
        int i = p.second.first;
        int j = p.second.second;
        m_closedList[i][j] = true;        

        m_currPos = { i, j };

        if (rollToCell({ i - 1, j }, dest)) {
            break;
        }

        if (rollToCell({ i + 1, j }, dest)) {
            break;
        }

        if (rollToCell({ i, j + 1 }, dest)) {      
            break;
        }

        if (rollToCell({ i, j - 1 }, dest)) {
            break;
        }       
    }
    
    if (dest == m_currPos) {

        Coord cNewPos = tracePath();
        newPos.x = cNewPos.first;
        newPos.y = cNewPos.second;

    } else
        throw std::logic_error("destionation not reached");

    return newPos;
}