#ifndef board_cc
#define board_cc

#include "board.h"

/*******************
 * Memory handling *
 *******************/

board::board(board* some_board, int xx, int yy) {
   
   connected = true;
   
   // Put myself within the ring list
   if(some_board == 0) {
      next = this;
      prev = this;
      this->xx = 0;
      this->yy = 0;
   } else {
      if(xx == SHRT_MAX && yy == SHRT_MAX) {
         cerr << "You have to give coordinates when placing a new" 
              << " tile that isn't the first one." << endl;
         exit(-1);
      }
      this->xx = xx;
      this->yy = yy;
      this->next = some_board->next;
      some_board->next = this;
      this->prev = some_board;
      this->next->prev = this;
   }
   
   // Allocating the neighbor array, not the neighbors themselves. Neighbors
   // will be set by place_piece().
   nbr = new board*[6];
   for(int ii=0; ii<6; ii++) nbr[ii] = 0;
   
   ontop = 0;
}

board::~board() {
   
   // Making sure nobody sits on top of me
   if(ontop != 0) {
      cerr << "Invalid delete, there's a piece on a tile "
           << "that's supposed to be empty." << endl;
      exit(-1);
   }
   
   // Removing myself from the linked list
   next->prev = prev;
   prev->next = next;
   
   // Removing myself from the graph
   for(int ii=0; ii<6; ii++) {
      if(nbr[ii]!=0) {
         if(nbr[ii]->nbr[(ii+3)%6] == 0) {
            cerr << "Something is wrong in the graph, "
                 << "it's not bidirectional." << endl;
            exit(-1);
         }
         nbr[ii]->nbr[(ii+3)%6] = 0;
      }
   }
   
   // Deleting the neighbor array, not the neighbors themselves
   delete[] nbr;
}

void board::delete_all() {
   board* it = this;
   board* started = this;
   board* nxt = this->next;
   do {
      it = nxt;
      if(it->ontop != 0) delete it->ontop;
      it->ontop = 0;
      nxt = it->next;
      delete it;
   } while(it != started);
}

/**********************
 * Graph manipulation *
 **********************/

void board::place_piece(piece* bug) {
   
   if(ontop == 0) {
      ontop = bug;
      // Making sure I have free tiles all around me, extending the graph
      board* new_board;
      for(int ii=0; ii<6; ii++) {
         if(nbr[ii] == 0) {
            new_board = new board(this, xx+nbrx[ii], yy+nbry[ii]);
            // Loop over the whole list to set all the new links in the graph
            board* it = this;
            board* started = this;
            do {
               it = it->next;
               for(int jj=0; jj<6; jj++) {
                  if(it->xx+nbrx[jj] == new_board->xx &&
                     it->yy+nbry[jj] == new_board->yy) {
                     it->nbr[jj] = new_board;
                     new_board->nbr[(jj+3)%6] = it;
                  }
               }
            } while(it != started);
         } else {
            nbr[ii]->connected = true;
         }
      }
   } else {
      // No need to manipulate the graph when moving on top of another piece
      piece* it = ontop;
      while(it->ontop != 0) it = it->ontop;
      it->ontop = bug;
   }
}

void board::remove_piece() {
   
   if(ontop == 0) {
      cerr << "Cannot remove piece, there is none on this tile." << endl;
      exit(-1);
   }
   
   // Delete the uppermost piece
   piece* it = this->ontop;
   piece* it_prev = 0;
   while(it->ontop != 0) {
      it_prev = it;
      it = it->ontop;
   }
   delete it;
   
   // Remove unnecessary pointer upwards and tiles that 
   // have no occupied neighbor any more
   if(it_prev != 0) {
      it_prev->ontop = 0;
   } else {
      this->ontop = 0;
      // Check for every neighbor if we can remove it
      for(int ii=0; ii<6; ii++) {
         bool removable = true;
         if(this->nbr[ii]->ontop != 0) {
            removable = false;
         } else {
            for(int jj=0; jj<6; jj++) {
               if(this->nbr[ii]->nbr[jj] != 0 &&
                  this->nbr[ii]->nbr[jj]->ontop != 0) {
                     removable = false;
               }
            }
         }
         if(removable) {
            this->nbr[ii]->connected = false;
            //delete this->nbr[ii];
         }
      }
   }
   
}

/****************
 * Graph search *
 ****************/

bool board::bee_can_move_to(int nbr_no) {
   
   if(nbr[nbr_no] == 0) return false; // Neighboring tile doesn't exist
   if(!nbr[nbr_no]->connected) return false; // Neighboring tile doesn't exist
   if(nbr[nbr_no]->ontop != 0) return false; // Neighboring tile occupied

   // We can move when one and only one of the two common neighbors have a
   // piece sitting on it.
   unsigned int side = 0;
   if(nbr[(nbr_no+1)%6] != 0 && nbr[(nbr_no+1)%6]->ontop != 0) side++;
   if(nbr[(nbr_no+5)%6] != 0 && nbr[(nbr_no+5)%6]->ontop != 0) side++;
   if(side==1) return true;
   return false;
}

bool board::impass_high_lvl(int nbr_no, bool remove_current_piece) {
   
   // Count the heights of the relevant stacks of pieces
   int my_height = 0;
   int nbr_height = 0;
   int right_height = 0;
   int left_height = 0;
   
   piece* it;

   it = this->ontop;
   while(it != 0) {
      it = it->ontop;
      my_height++;
   }
   if(remove_current_piece) my_height--; // Because my own piece doesn't count 
                                         // towards the stack I'm sitting on
   
   it = nbr[nbr_no]->ontop;
   while(it != 0) {
      it = it->ontop;
      nbr_height++;
   }
   
   it = nbr[(nbr_no+1)%6]->ontop;
   while(it != 0) {
      it = it->ontop;
      left_height++;
   }
   
   it = nbr[(nbr_no+5)%6]->ontop;
   while(it != 0) {
      it = it->ontop;
      right_height++;
   }

   if(left_height > my_height && right_height > my_height &&
      left_height > nbr_height && right_height > nbr_height) {
       // We have an impass
      return true;
   }
   
   return false;
}

bool board::one_hive() {
   int ii = 0;
   while(this->nbr[ii]->ontop == 0) {
      ii++;
      if(ii == 6) return false; // We are the only piece on the board
   }
   if(this->ontop->ontop == 0) { // We don't have to pieces sitting on top of us
      for(int jj=ii+1; jj<6; jj++) {
         if(this->nbr[jj]->ontop != 0 && 
           !this->nbr[ii]->has_path_to(this->nbr[jj], this)) {
            // tile ii and jj would be separated
            return false;
         }
      }
   }
   return true;
}

void board::ant_graph_search(vector<turn*> &turns, board* origin) {
   if(!visited) {
      if(this != origin) turns.push_back(new turn(origin, this));
      visited = true;
      for(int ii=0; ii<6; ii++) {
         if(bee_can_move_to(ii)) {
            nbr[ii]->ant_graph_search(turns, origin);
         }
      }
   }
}

bool board::path_search(board* to) {
   if(this == to) return true;
   visited = true;
   for(int ii=0; ii<6; ii++) {
      if(nbr[ii] != 0 && !nbr[ii]->visited && nbr[ii]->ontop != 0) {
         if(nbr[ii]->path_search(to)) return true;
      }
   }
   return false;
}

bool board::has_path_to(board* to, board* without) {
   
   if(this->ontop == 0 || to->ontop == 0) {
      cerr << "One of the tiles is empty. Cannot start route search." << endl;
      exit(-1);
   }
   
   set_not_visited();
   without->visited = true;
   return this->path_search(to);
   
}

void board::set_not_visited() {
   // Loop over the whole list to set all the flags
   board* it = this;
   board* started = this;
   do {
      it->visited = false;
      it = it->next;
   } while(it != started);
}


#endif 




















