#ifndef turn_cc
#define turn_cc

#include "turn.h"

turn::turn() {
   this->from = 0;
   this->to = 0;
   this->kind = empty;
   this->color = false;
}

turn::turn(board* from, board* to) {
   this->from = from;
   this->to = to;
   this->kind = empty;
   this->color = false;
}

turn::turn(type kind, bool color, board* to) {
   this->kind = kind;
   this->color = color;
   this->to = to;
   this->from = 0;
}

// TODO There's room for optimization here - instead of deleting and 
// creating a piece we could just move pointers around.

void turn::perform() {
   if(kind==empty) {
      // Move around
      piece *it = from->ontop;
      if(it == 0) {
         cerr << "Illegal move, there's no piece on this tile." << endl;
         exit(-1);
      }
      while(it->ontop != 0) it = it->ontop;
      type my_kind = it->kind;
      bool my_color = it->color;
      from->remove_piece();
      piece *my_piece = new piece(my_kind, my_color, to);
   } else {
      // Place a new tile
      piece* my_piece = new piece(kind, color, to);
   }
}

void turn::undo() {
   if(kind==empty) {
      // Move back
      piece *it = to->ontop;
      if(it == 0) {
         cout << "Illegal move, there's no piece to move back." << endl;
         exit(-1);
      }
      while(it->ontop != 0) it = it->ontop;
      type my_kind = it->kind;
      bool my_color = it->color;
      to->remove_piece();
      piece *my_piece = new piece(my_kind, my_color, from);
   } else {
      // Remove a tile
      to->remove_piece();
   }
}

#endif
