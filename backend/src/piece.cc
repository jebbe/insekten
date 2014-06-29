#ifndef piece_cc
#define piece_cc

#include "piece.h"

piece::piece(type kind, bool color, board* place_at) {
   
   this->kind = kind;
   this->color = color;
   at = place_at;
   ontop = 0;
  
   at->place_piece(this);
   
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

void piece::list_moves(vector<turn*> &turns, board* just_moved) {
   
   // Check if we are blocked by a dungbeetle
   if(this->ontop != 0) return;
   
   if(at == just_moved) return;
   
   // Check if we can move without violating the one-hive-rule. The pillbug
   // has to check this move itself, since it can move other pieces around.
   if(kind != pillbug && !at->one_hive()) return;

   // Find possible moves for our specific piece
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
         find_moves_mosquito(turns, just_moved);
         break;
      case pillbug: // Make sure pillbug comes last so that pieces move by 
                    // themselves rather than being moved by a pillbug if both
                    // is possible
         find_moves_pillbug(turns, just_moved);
         break;
      default:
         cout << "Cannot find a move for an undefined piece." << endl;
         exit(-1);
   }
}

void piece::remove_duplicate_moves(vector<turn*> &turns) {
   vector<uint> duplicates;
   at->set_not_visited();
   for(uint ii=0; ii<turns.size(); ii++) {
      for(uint jj=ii+1; jj<turns.size(); jj++) {
         if( turns[ii]->from == turns[jj]->from &&
             turns[ii]->to == turns[jj]->to &&
             turns[ii]->kind == turns[jj]->kind ) {
            duplicates.push_back(ii);
         }
      }
   }
   for(int ii=duplicates.size()-1; ii>=0; ii--) {
      turns.erase(turns.begin()+duplicates[ii]);
   }
}

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
            if(it->impass_high_lvl(ii, true)) new_move = false;
            it = it->nbr[ii];
         }
         if(new_move) turns.push_back(new turn(at, it));
      }
   }
}

void piece::find_moves_beetle(vector<turn*> &turns) {
   find_moves_queen(turns);
   for(int ii=0; ii<6; ii++) {
      if(at->nbr[ii]->ontop != 0 && !(at->impass_high_lvl(ii, true))) {
         turns.push_back(new turn(at, at->nbr[ii]));
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

void piece::find_moves_mosquito(vector<turn*> &turns, board* just_moved) {
   
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
                  find_moves_pillbug(turns, just_moved);
                  break;
               case mosquito:
                  break;
               default:
                  cerr << "There's apiece on the board that has no valid type." << endl;
                  exit(-1);
            }
         }
      }
      remove_duplicate_moves(turns);
   }
}

void piece::find_moves_pillbug(vector<turn*> &turns, board* just_moved) {
   
   // Move myself
   if(at->one_hive()) find_moves_queen(turns);

   // Move others around
   for(int ii=0; ii<6; ii++) {
      if(at->nbr[ii]->ontop != 0 && at->nbr[ii]->ontop->ontop == 0) {
         for(int jj=0; jj<6; jj++) {
            if(at->nbr[jj]->ontop == 0) {
               // Check one hive rule
               if(  at->nbr[ii]->one_hive() &&
                    !(at->nbr[ii]->impass_high_lvl((ii+3)%6, true)) &&
                    !(at->impass_high_lvl(jj, false)) &&
                    (at->nbr[ii] != just_moved)) {
                  turns.push_back(new turn(at->nbr[ii], at->nbr[jj]));
               }
            }
         }
      }
   }
   remove_duplicate_moves(turns);
}

#endif 
 
