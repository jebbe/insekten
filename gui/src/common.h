#ifndef COMMON_H
#define COMMON_H

#include "piece.h"

// All numbers should be even, since we divide by two a lot.

// Height of the tiles
const int HEIGHT = 72;
// Width of the tiles
const int WIDTH = 82;
// How far are pieces apart horizontally
const int SPACING = 41;
// How do we stack the pieces?
const int XSHIFT = 3;
const int YSHIFT = 3;
// Maximum number of pieces
const int NO_PIECES = 8;
// How large is the central widget scene?
const int MAIN_SIZE = 2000;

typedef struct {
   bool origin_selected;
   type origin_type;
   bool origin_color;
   int origin_x; // In game (not scene) notation
   int origin_y;
   bool dest_selected;
//    bool dest_color;
//    type dest_type;
//    int dest_x;
//    int dest_y;
} uiMove;

#endif