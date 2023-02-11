#pragma once


using cell_pos_t = size_t;

/* assumed (xUL,yUL) point is left-upper most, otherwise
 * undefined behavior */
struct Wall {
    cell_pos_t xUL;
    cell_pos_t yUL;
    cell_pos_t xRD;
    cell_pos_t yRD;

    bool operator<(const Wall& other) const;

};
