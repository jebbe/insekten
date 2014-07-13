#ifndef turn_cc
#define turn_cc

#include "turn.h"

turn::turn() {
   this->from = 0;
   this->to = 0;
   this->kind = empty;
   this->color = false;
   this->pass = false;
}

turn::turn(board* from, board* to) {
   this->from = from;
   this->to = to;
   this->kind = empty;
   this->color = false;
   this->pass = false;
}

turn::turn(type kind, bool color, board* to) {
   this->kind = kind;
   this->color = color;
   this->to = to;
   this->from = 0;
   this->pass = false;
}

turn::turn(turn& other) {
   this->kind = other.kind;
   this->color = other.color;
   this->to = other.to;
   this->from = other.from;
   this->pass = other.pass;
}

turn::turn(bool pass) {
   this->kind = empty;
   this->color = false;
   this->to = 0;
   this->from = 0;
   this->pass = true;
}


// This is supposed to be an optimized version of the routine below,
// but it doesn't seem to achieve any performance improvement.
// void turn::perform() {
//    if(pass) return;
//    if(kind==empty) {
//       // Move around
//       
//       //from->remove_piece();
//       
//       // Find the uppermost piece and call it "it"
//       piece *it = from->ontop;
//       piece* it_prev = 0;
//       while(it->ontop != 0) {
//          it_prev = it;
//          it = it->ontop;
//       }
//       
//       // Remove unnecessary pointer upwards and tiles that 
//       // have no occupied neighbor any more
//       if(it_prev != 0) {
//          it_prev->ontop = 0;
//       } else {
//          from->ontop = 0;
//          // Check for every neighbor if we can remove it
//          for(int ii=0; ii<6; ii++) {
//             bool removable = true;
//             if(from->nbr[ii]->ontop != 0) {
//                removable = false;
//             } else {
//                for(int jj=0; jj<6; jj++) {
//                   if(from->nbr[ii]->nbr[jj] != 0 &&
//                      from->nbr[ii]->nbr[jj]->ontop != 0) {
//                         removable = false;
//                   }
//                }
//             }
//             if(removable) {
//                from->nbr[ii]->connected = false;
//             }
//          }
//       }
//       
//       //new piece(my_kind, my_color, to);
//       
//       // Place the new piece
//       it->at = to;
//       it->ontop = 0;  
//       to->place_piece(it);
//       
//    } else {
//       // Place a new tile
//       new piece(kind, color, to);
//    }
// }

void turn::perform() {
   if(pass) return;
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
      new piece(my_kind, my_color, to);
   } else {
      // Place a new tile
      new piece(kind, color, to);
   }
}

void turn::undo() {
   if(pass) return;
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
      new piece(my_kind, my_color, from);
   } else {
      // Remove a tile
      to->remove_piece();
   }
}

#endif
