#pragma once

#include "Cell.h"

class MoveState {
    cell_pos_t currX;
    cell_pos_t currY;
    cell_pos_t tarX;
    cell_pos_t tarY;
    double transferState;
};

class Entity {

public:

    void move();

    void setCommand();

    struct D2D_RECT_F sprite();


private:

    /* speed in cells per second */
    double m_speed = 0.6; 

    int m_spriteNumber = 0;

};