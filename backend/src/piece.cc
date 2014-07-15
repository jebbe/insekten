#ifndef piece_cc
#define piece_cc

#include "piece.h"

piece::piece(type kind, bool color, board* place_at) {
   
   this->kind = kind;
   this->color = color;
   this->at = place_at;
   this->ontop = 0;
  
   this->at->place_piece(this);
   
}

piece::~piece() {
   // remove all pieces on top of me
   piece *it, *prev;
   while(this->ontop!=0) {
      it = this;
      while(it->ontop!=0) {
         prev = it;
         it = it->ontop;
      }
      delete it;
      prev->ontop = 0;
   }
}

void piece::list_moves(vector<turn*> &turns, board* just_moved, 
                       bool whoseturn, bool rm_duplicates,
                       bool check_just_moved) {
   
   // Check if we are blocked by a dungbeetle
   if(this->ontop != 0) return;
   
   if(check_just_moved && at == just_moved) return;
   
   // Check if we can move without violating the one-hive-rule. The pillbug
   // has to check this move itself, since it can move other pieces around.
   if(!at->one_hive()) return;
   
   // Find possible moves for our specific piece
   if(whoseturn == color) {
      switch(kind) {
         case queen:
            find_moves_queen(turns);
            break;
         case ant:
            find_moves_ant(turns);
            break;
         case spider:
            find_moves_spider(turns);
            break;
         case cricket:
            find_moves_cricket(turns);
            break;
         case beetle:
            find_moves_beetle(turns);
            break;
         case ladybug:
            find_moves_ladybug(turns);
            break;
         case mosquito:
            find_moves_mosquito(turns);
            break;
         case pillbug:
            find_moves_pillbug(turns);
            break;
         default:
            cout << "Cannot find a move for an undefined piece." << endl;
            exit(-1);
      }
   }

   // Account for the possibility that the pillbug moves us around
   if(at->ontop == this) {
      for(int ii=0; ii<6; ii++) {
         
         bool nbr_is_pillbug = false;
         // Is my neighbor a pillbug?
         if(at->nbr[ii] != 0 &&
            at->nbr[ii]->ontop != 0 && 
            at->nbr[ii]->ontop->kind == pillbug && 
            at->nbr[ii]->ontop->color == whoseturn) nbr_is_pillbug = true;
         // Is my neighbor a mosquito that inherits pillbug behavior?
         if(   at->nbr[ii] != 0 &&
               at->nbr[ii]->ontop != 0 && 
               at->nbr[ii]->ontop->kind == mosquito && 
               at->nbr[ii]->ontop->color == whoseturn) {
            for(int kk=0; kk<6; kk++) {
               if(   at->nbr[ii]->nbr[kk] != 0 &&
                     at->nbr[ii]->nbr[kk]->ontop != 0 && 
                     at->nbr[ii]->nbr[kk]->ontop->kind == pillbug) {
                  nbr_is_pillbug = true;
               }
            }
         }

         if(nbr_is_pillbug) {
            // Check the neighbors of the pillbug. Pillbug is at at->nbr[ii]
            for(int jj=0; jj<6; jj++) {
               if(ii != (jj+3)%6) {
                  if(at->nbr[ii]->nbr[jj]->ontop == 0) {
                     if(  !(at->impass_high_lvl(ii, true)) &&
                          !(at->nbr[ii]->impass_high_lvl(jj, false))) {
                        turns.push_back(new turn(at, at->nbr[ii]->nbr[jj]));
                     }
                  }
               }
            }
         }
      }
   }
   
   if(rm_duplicates) remove_duplicate_moves(turns);
   
}


int piece::count_moves(board* just_moved, bool whoseturn) {
   
   // Check if we are blocked by a dungbeetle
   if(this->ontop != 0) return 0;
   
   if(at == just_moved) return 0;
   
   // Check if we can move without violating the one-hive-rule. The pillbug
   // has to check this move itself, since it can move other pieces around.
   if(!at->one_hive()) return 0;
   
   int n_moves = 0;
   
   // Find possible moves for our specific piece
   if(whoseturn == color) {
      switch(kind) {
         case queen:
            n_moves += count_moves_queen();
            break;
         case ant:
            n_moves += count_moves_ant();
            break;
         case spider:
            n_moves += count_moves_spider();
            break;
         case cricket:
            n_moves += count_moves_cricket();
            break;
         case beetle:
            n_moves += count_moves_beetle();
            break;
         case ladybug:
            n_moves += count_moves_ladybug();
            break;
         case mosquito:
            n_moves += count_moves_mosquito();
            break;
         case pillbug:
            n_moves += count_moves_pillbug();
            break;
         default:
            cout << "Cannot count the moves for an undefined piece." << endl;
            exit(-1);
      }
   }

   // Account for the possibility that the pillbug moves us around
   if(at->ontop == this) {
      for(int ii=0; ii<6; ii++) {
         
         bool nbr_is_pillbug = false;
         // Is my neighbor a pillbug?
         if(at->nbr[ii] != 0 &&
            at->nbr[ii]->ontop != 0 && 
            at->nbr[ii]->ontop->kind == pillbug && 
            at->nbr[ii]->ontop->color == whoseturn) nbr_is_pillbug = true;
         // Is my neighbor a mosquito that inherits pillbug behavior?
         if(   at->nbr[ii] != 0 &&
               at->nbr[ii]->ontop != 0 && 
               at->nbr[ii]->ontop->kind == mosquito && 
               at->nbr[ii]->ontop->color == whoseturn) {
            for(int kk=0; kk<6; kk++) {
               if(   at->nbr[ii]->nbr[kk] != 0 &&
                     at->nbr[ii]->nbr[kk]->ontop != 0 && 
                     at->nbr[ii]->nbr[kk]->ontop->kind == pillbug) {
                  nbr_is_pillbug = true;
               }
            }
         }

         if(nbr_is_pillbug) {
            // Check the neighbors of the pillbug. Pillbug is at at->nbr[ii]
            for(int jj=0; jj<6; jj++) {
               if(ii != (jj+3)%6) {
                  if(at->nbr[ii]->nbr[jj]->ontop == 0) {
                     if(  !(at->impass_high_lvl(ii, true)) &&
                          !(at->nbr[ii]->impass_high_lvl(jj, false))) {
                        n_moves++;
                     }
                  }
               }
            }
         }
      }
   }
   return n_moves;
}


// void piece::remove_duplicate_moves(vector<turn*> &turns) {
//    for(unsigned int ii=0; ii<turns.size(); ii++) {
//       for(unsigned int jj=ii+1; jj<turns.size(); jj++) {
//          if(  turns[ii]->from == turns[jj]->from &&
//               turns[ii]->to == turns[jj]->to &&
//               turns[ii]->kind == turns[jj]->kind ) {
//             turns.erase(turns.begin()+jj); jj--;
//          }
//       }
//    }
// }

bool duplicate_compare_function (turn* ii, turn* jj) { 
   return (ii->from < jj->from); 
}

void piece::remove_duplicate_moves(vector<turn*> &turns) {
   sort(turns.begin(), turns.end(), duplicate_compare_function);
   turns.erase(unique(turns.begin(), turns.end()), turns.end());
   
}

// ============
//  Find individual moves
// ============

void piece::find_moves_queen(vector<turn*> &turns) {
   for(int ii=0; ii<6; ii++) {
      if(at->bee_can_move_to(ii)) {
         turns.push_back(new turn(at, at->nbr[ii]));
      }
   }
}

void piece::find_moves_ant(vector<turn*> &turns) {
   at->set_not_visited();
   at->ontop = 0;
   at->ant_graph_search(turns, at);
   at->ontop = this;
}

void piece::find_moves_spider(vector<turn*> &turns) {
   at->ontop = 0;
   for(int ii=0; ii<6; ii++) {
      if(at->bee_can_move_to(ii)) {
         for(int jj=0; jj<6; jj++) {
            if(jj != (ii+3)%6 && // This would move me back to where I came from
                  at->nbr[ii]->bee_can_move_to(jj)) {
               for(int kk=0; kk<6; kk++) {
                  if(kk != (jj+3)%6 &&
                        at->nbr[ii]->nbr[jj]->bee_can_move_to(kk)) {
                     turns.push_back(new turn(at, 
                                       at->nbr[ii]->nbr[jj]->nbr[kk]));
                  }
               }
            }
         }
      }
   }
   at->ontop = this;
}

void piece::find_moves_cricket(vector<turn*> &turns) {
   for(int ii=0; ii<6; ii++) {
      board* it;
      if(at->nbr[ii]->ontop != 0 && !(at->impass_high_lvl(ii, true))) {
         it = at->nbr[ii];
         bool new_move = true;
         while(it->ontop != 0) {
            if(it->impass_high_lvl(ii, false)) new_move = false;
            it = it->nbr[ii];
         }
         if(new_move) turns.push_back(new turn(at, it));
      }
   }
}

void piece::find_moves_beetle(vector<turn*> &turns) {
   if(at->ontop == this) {
      // We're not on top of somebody else
      find_moves_queen(turns);
      for(int ii=0; ii<6; ii++) {
         if(at->nbr[ii]->ontop != 0 && !(at->impass_high_lvl(ii, true))) {
            turns.push_back(new turn(at, at->nbr[ii]));
         }
      }
   } else {
      // We're on top of somebody else
      for(int ii=0; ii<6; ii++) {
         if(!(at->impass_high_lvl(ii, true))) {
            turns.push_back(new turn(at, at->nbr[ii]));
         }
      }
   }
}

void piece::find_moves_ladybug(vector<turn*> &turns) {
   at->ontop = 0;
   at->set_not_visited();
   at->visited = true;
   for(int ii=0; ii<6; ii++) {
      if(at->nbr[ii]->ontop != 0 &&  // One move on top of someone else
       !(at->impass_high_lvl(ii, true))) {
         for(int jj=0; jj<6; jj++) {
            if(at->nbr[ii]->nbr[jj]->ontop != 0 && // Another one
             !(at->nbr[ii]->impass_high_lvl(jj, false))) {
               for(int kk=0; kk<6; kk++) {
                  if(!at->nbr[ii]->nbr[jj]->nbr[kk]->visited && // Prevent duplicate moves
                      at->nbr[ii]->nbr[jj]->nbr[kk]->ontop == 0 &&
                    !(at->nbr[ii]->nbr[jj]->impass_high_lvl(kk, false))) {
                     at->nbr[ii]->nbr[jj]->nbr[kk]->visited = true;
                     turns.push_back(new turn(at, 
                                              at->nbr[ii]->nbr[jj]->nbr[kk]));
                  }
               }
            }
         }
      }
   }
   at->ontop = this;
}

void piece::find_moves_mosquito(vector<turn*> &turns) {
   
   // Am I on top of somebody and thus a beetle?
   if(at->ontop != this) {
      find_moves_beetle(turns);
   } else {
      for(int ii=0; ii<6; ii++) {
         piece* it = at->nbr[ii]->ontop;
         if(it != 0) {
            while(it->ontop != 0) it = it->ontop;
            switch(it->kind) {
               case queen:
                  find_moves_queen(turns);
                  break;
               case ant:
                  find_moves_ant(turns);
                  break;
               case spider:
                  find_moves_spider(turns);
                  break;
               case cricket:
                  find_moves_cricket(turns);
                  break;
               case beetle:
                  find_moves_beetle(turns);
                  break;
               case ladybug:
                  find_moves_ladybug(turns);
                  break;
               case pillbug:
                  find_moves_pillbug(turns);
                  break;
               case mosquito:
                  break;
               default:
                  cerr << "There's a piece on the board that has no valid type." << endl;
                  exit(-1);
            }
         }
      }
   }
}

void piece::find_moves_pillbug(vector<turn*> &turns) {
   find_moves_queen(turns);
}

// ============
// Count individual moves
// ============

int piece::count_moves_queen() {
   int n_moves = 0;
   for(int ii=0; ii<6; ii++) {
      if(at->bee_can_move_to(ii)) {
         n_moves++;
      }
   }
   return n_moves;
}

int piece::count_moves_ant() {
   at->set_not_visited();
   at->ontop = 0;
   int n_moves = 0;
   at->ant_graph_search(at, n_moves);
   at->ontop = this;
   return n_moves;
}

int piece::count_moves_spider() {
   int n_moves = 0;
   at->ontop = 0;
   for(int ii=0; ii<6; ii++) {
      if(at->bee_can_move_to(ii)) {
         for(int jj=0; jj<6; jj++) {
            if(jj != (ii+3)%6 && // This would move me back to where I came from
                  at->nbr[ii]->bee_can_move_to(jj)) {
               for(int kk=0; kk<6; kk++) {
                  if(kk != (jj+3)%6 &&
                        at->nbr[ii]->nbr[jj]->bee_can_move_to(kk)) {
                     n_moves++;
                  }
               }
            }
         }
      }
   }
   at->ontop = this;
   return n_moves;
}

int piece::count_moves_cricket() {
   int n_moves = 0;
   for(int ii=0; ii<6; ii++) {
      board* it;
      if(at->nbr[ii]->ontop != 0 && !(at->impass_high_lvl(ii, true))) {
         it = at->nbr[ii];
         bool new_move = true;
         while(it->ontop != 0) {
            if(it->impass_high_lvl(ii, false)) new_move = false;
            it = it->nbr[ii];
         }
         if(new_move) n_moves++;
      }
   }
   return n_moves;
}

int piece::count_moves_beetle() {
   int n_moves = 0;
   if(at->ontop == this) {
      // We're not on top of somebody else
      n_moves += count_moves_queen();
      for(int ii=0; ii<6; ii++) {
         if(at->nbr[ii]->ontop != 0 && !(at->impass_high_lvl(ii, true))) {
            n_moves++;
         }
      }
   } else {
      // We're on top of somebody else
      for(int ii=0; ii<6; ii++) {
         if(!(at->impass_high_lvl(ii, true))) {
            n_moves++;
         }
      }
   }
   return n_moves;
}

int piece::count_moves_ladybug() {
   int n_moves = 0;
   at->ontop = 0;
   at->set_not_visited();
   at->visited = true;
   for(int ii=0; ii<6; ii++) {
      if(at->nbr[ii]->ontop != 0 &&  // One move on top of someone else
       !(at->impass_high_lvl(ii, true))) {
         for(int jj=0; jj<6; jj++) {
            if(at->nbr[ii]->nbr[jj]->ontop != 0 && // Another one
             !(at->nbr[ii]->impass_high_lvl(jj, false))) {
               for(int kk=0; kk<6; kk++) {
                  if(!at->nbr[ii]->nbr[jj]->nbr[kk]->visited && // Prevent duplicate moves
                      at->nbr[ii]->nbr[jj]->nbr[kk]->ontop == 0 &&
                    !(at->nbr[ii]->nbr[jj]->impass_high_lvl(kk, false))) {
                     at->nbr[ii]->nbr[jj]->nbr[kk]->visited = true;
                     n_moves++;
                  }
               }
            }
         }
      }
   }
   at->ontop = this;
   return n_moves;
}

int piece::count_moves_mosquito() {
   int n_moves = 0;
   // Am I on top of somebody and thus a beetle?
   if(at->ontop != this) {
      return count_moves_beetle();
   } else {
      for(int ii=0; ii<6; ii++) {
         piece* it = at->nbr[ii]->ontop;
         if(it != 0) {
            while(it->ontop != 0) it = it->ontop;
            switch(it->kind) {
               case queen:
                  n_moves += count_moves_queen();
                  break;
               case ant:
                  n_moves += count_moves_ant();
                  break;
               case spider:
                  n_moves += count_moves_spider();
                  break;
               case cricket:
                  n_moves += count_moves_cricket();
                  break;
               case beetle:
                  n_moves += count_moves_beetle();
                  break;
               case ladybug:
                  n_moves += count_moves_ladybug();
                  break;
               case pillbug:
                  n_moves += count_moves_pillbug();
                  break;
               case mosquito:
                  break;
               default:
                  cerr << "There's a piece on the board that has no valid type." << endl;
                  exit(-1);
            }
         }
      }
   }
   return n_moves;
}

int piece::count_moves_pillbug() {
   return count_moves_queen();
}


#endif 
 
