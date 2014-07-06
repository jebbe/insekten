#ifndef game_cc
#define game_cc

#include "game.h"

game::game(ruleset rules) {
   
   my_board = new board(0);
   this->rules = rules;
   half_turns = 0;
   
   just_moved = 0;
   
   for(int ii=0; ii<=1; ii++) {
      stock[ii][queen] = 1;
      stock[ii][ant] = 3;
      stock[ii][spider] = 2;
      stock[ii][cricket] = 3;
      stock[ii][beetle] = 2;
      if((rules & l) == l) {
         stock[ii][ladybug] = 1;
      } else {
         stock[ii][ladybug] = 0;
      }
      if((rules & m) == m) {
         stock[ii][mosquito] = 1;
      } else {
         stock[ii][mosquito] = 0;
      }
      if((rules & p) == p) {
         stock[ii][pillbug] = 1;
      } else {
         stock[ii][pillbug] = 0;
      }
   }
}

game::~game() {
   for(unsigned int ii=0; ii<history.size(); ii++) delete history[ii];
   my_board->delete_all();
}

/**************
 * GAME RULES *
 **************/

void game::find_placeable_tiles(bool color, vector<board*> &tiles) {
   
   // Is this the very first move?
   if(half_turns == 0) {
      if(color == whose_turn()) tiles.push_back(my_board);
      return;
   }
   
   // Is this the second move?
   if(half_turns == 1) {
      if(color == whose_turn()) {
         for(int ii=0; ii<6; ii++) {
            tiles.push_back(my_board->nbr[ii]);
         }
      }
      return;
   }
   
   // Loop over the whole list to find empty tiles with only correctly-colored
   // neighbors
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->ontop == 0 && it->connected == true) {
         bool can_place = true;
         for(int ii=0; ii<6; ii++) {
            if(it->nbr[ii] != 0 && it->nbr[ii]->ontop !=0) {
               piece *mp = it->nbr[ii]->ontop;
               while(mp->ontop != 0) mp = mp->ontop;
               if(mp->color != color) {
                  can_place = false;
                  break;
               }
            }
         }
         if(can_place) {
            tiles.push_back(it);
         }
      }
      it = it->next;
   } while(it != started);
}

void game::find_all_placement_moves(bool color, vector<turn*> &turns) {

   // Where can I place pieces?
   vector<board*> tiles;
   find_placeable_tiles(color, tiles);
   
   // Do we have to place the queen bee?
   if(half_turns > 5 && stock[int(color)][queen] == 1) {
      for(unsigned int ii=0; ii<tiles.size(); ii++) {
         turns.push_back(new turn(queen, color, tiles[ii]));
      }
      return;
   }
   
   // Find all possible placements
   for(int ii=0; ii<tiles.size(); ii++) {
      for(unsigned char kind=0; kind<NUM_TYPE; kind++) {
         if(stock[int(color)][int(kind)] > 0) {
            turns.push_back(new turn((type)kind, color, tiles[ii]));
         }
      }
   }
      
   if(half_turns > 0) {
      board* it = my_board;
      while(it->ontop == 0) it = it->next;
      it->ontop->remove_duplicate_moves(turns);
   }
   
}


void game::find_all_movement_moves(bool color, vector<turn*> &turns) {
   
   // Where can I place pieces?
   vector<board*> tiles;
   find_placeable_tiles(color, tiles);
   
   // Do we have to place the queen bee?
   if(half_turns > 5 && stock[int(color)][queen] == 1) {
      for(unsigned int ii=0; ii<tiles.size(); ii++) {
         turns.push_back(new turn(queen, color, tiles[ii]));
      }
      return;
   }
   
   // Where can I move pieces?
   if(stock[int(color)][queen] == 0) { // Is the bee already placed?
      board* it = my_board;
      board* started = my_board;
      do {
         if(it->ontop != 0) {
            it->ontop->list_moves(turns, just_moved, whose_turn(), false);
         }
         it = it->next;
      } while(it != started);
   }

   if(half_turns > 0) {
      board* it = my_board;
      while(it->ontop == 0) it = it->next;
      it->ontop->remove_duplicate_moves(turns);
   }
   
}

void game::find_all_moves(bool color, vector<turn*> &turns) {
   
   // Where can I place pieces?
   vector<board*> tiles;
   find_placeable_tiles(color, tiles);
   
   // Do we have to place the queen bee?
   if(half_turns > 5 && stock[int(color)][queen] == 1) {
      for(unsigned int ii=0; ii<tiles.size(); ii++) {
         turns.push_back(new turn(queen, color, tiles[ii]));
      }
      return;
   }
   
   // Find all possible placements
   for(int ii=0; ii<tiles.size(); ii++) {
      for(unsigned char kind=0; kind<NUM_TYPE; kind++) {
         if(stock[int(color)][int(kind)] > 0) {
            turns.push_back(new turn((type)kind, color, tiles[ii]));
         }
      }
   }
      
   // Where can I move pieces?
   if(stock[int(color)][queen] == 0) { // Is the bee already placed?
      board* it = my_board;
      board* started = my_board;
      do {
         if(it->ontop != 0) {
            it->ontop->list_moves(turns, just_moved, whose_turn(), false);
         }
         it = it->next;
      } while(it != started);
   }

   if(half_turns > 0) {
      board* it = my_board;
      while(it->ontop == 0) it = it->next;
      it->ontop->remove_duplicate_moves(turns);
   }
   
}

void game::delete_moves(vector<turn*> &turns) {
   for(int ii=0; ii<turns.size(); ii++) {
      delete turns[ii];
   }
   turns.clear();
}
   
void game::perform_move(turn* go) {
   if(go->kind != empty)
      stock[whose_turn()][int(go->kind)]--;
   just_moved = go->to;
   just_moved_history.push_back(just_moved);
   go->perform();
   half_turns++;
   history.push_back(new turn(*go));
}

void game::undo_move() {
   if(history.back()->kind != empty) 
      stock[int(!whose_turn())][history.back()->kind]++;
   just_moved = just_moved_history.back();
   just_moved_history.pop_back();
   history.back()->undo();
   delete history.back();
   history.pop_back();
      half_turns--;
}

bool game::queen_surrounded(bool color) {
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->ontop != 0 && it->ontop->kind == queen
                        && it->ontop->color == !color) {
         for(int ii=0; ii<6; ii++) {
            if(it->nbr[ii]->ontop == 0) return false;
         }
         return true;
      }
      it = it->next;
   } while(it != started);
   return false;
}

bool game::no_legal_move(bool color) {
   if(whose_turn() != color) return false;
   vector<turn*> turns;
   find_all_moves(color, turns);
   int size = turns.size();
   // delete the moves
   if(size == 0) return true;
   return false;
}


/******************
 * OUTSIDE ACCESS *
 ******************/

int game::x_min() {
   int xmin=0;
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->xx < xmin) xmin = it->xx;
      it = it->next;
   } while(it != started);
   return xmin;
}

int game::x_max() {
   int xmax=0;
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->xx > xmax) xmax = it->xx;
      it = it->next;
   } while(it != started);
   return xmax;
}

int game::y_min() {
   int ymin=0;
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->yy < ymin) ymin = it->yy;
      it = it->next;
   } while(it != started);
   return ymin;
}

int game::y_max() {
   int ymax=0;
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->yy > ymax) ymax = it->yy;
      it = it->next;
   } while(it != started);
   return ymax;
}

type game::get_tile_type(int xx, int yy) {
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->xx == xx && it->yy == yy) {
         if(it->ontop == 0) return empty;
         piece* my_piece = it->ontop;
         while(my_piece->ontop != 0) my_piece = my_piece->ontop;
         return my_piece->kind;
      }
      it = it->next;
   } while(it != started);
   return empty;
}

bool game::get_tile_color(int xx, int yy) {
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->xx == xx && it->yy == yy) {
         if(it->ontop == 0) return empty;
         piece* my_piece = it->ontop;
         while(my_piece->ontop != 0) my_piece = my_piece->ontop;
         return my_piece->color;
      }
      it = it->next;
   } while(it != started);
   return empty;
}

int game::in_stock(type kind, bool color) {
   return stock[int(color)][int(kind)];
}

bool game::whose_turn() {
   return bool(half_turns%2);
}

bool game::buried_tiles(int xx, int yy, 
                        vector<type> &kind, vector<bool> &color) {
   kind.clear();
   color.clear();
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->xx == xx && it->yy == yy) {
         if(it->ontop == 0) return false;
         if(it->ontop->ontop == 0) return false;
         piece* my_piece = it->ontop;
         while(my_piece->ontop != 0) {
            kind.push_back(my_piece->kind);
            color.push_back(my_piece->color);
            my_piece = my_piece->ontop;
         }
         return true;
      }
      it = it->next;
   } while(it != started);
   return false;
}

bool game::buried_tiles(int xx, int yy) {
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->xx == xx && it->yy == yy) {
         if(it->ontop == 0) return false;
         if(it->ontop->ontop == 0) return false;
         return true;
      }
      it = it->next;
   } while(it != started);
   return false;
}

// white: false / black: true
bool game::game_over() {
   return (queen_surrounded(true) || queen_surrounded(false) ||
           no_legal_move(true) || no_legal_move(false));
}

bool game::white_wins() {
   return ((queen_surrounded(false) && !queen_surrounded(true)) ||
           (no_legal_move(true) && !no_legal_move(false)));
}

bool game::black_wins() {
   return ((queen_surrounded(true) && !queen_surrounded(false)) ||
           (no_legal_move(false) && !no_legal_move(true)));
}

bool game::is_draw() {
   return ((queen_surrounded(true) && queen_surrounded(false)) ||
           (no_legal_move(false) && no_legal_move(true)));
}

ruleset game::our_rules() {
   return rules;
}

bool game::need_to_place_queen() {
   return (half_turns > 5 && stock[whose_turn()][queen] == 1);

}

vector<vector<int>> game::can_move_to(int xx, int yy) {
   
   vector<vector<int>> targets;
   
   // Have we already queen, ie can we move at all?
   if(stock[whose_turn()][queen] == 1) return targets;
   
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->xx == xx && it->yy == yy) {
         if(it->ontop == 0) return targets;
         piece* mp = it->ontop;
         while(mp->ontop != 0) mp = mp->ontop;
         vector<turn*> turns;
         mp->list_moves(turns, just_moved, whose_turn(), true);

         for(unsigned int ii=0; ii<turns.size(); ii++) {
            vector<int> coords = {turns[ii]->to->xx, 
                                  turns[ii]->to->yy};
            targets.push_back(coords);
            delete turns[ii];
         }
         return targets;
      }
      it = it->next;
   } while(it != started);
   return targets;
}

vector<vector<int> > game::can_place_at() {

   vector<vector<int> > targets;
   vector<board*> tiles;
   find_placeable_tiles(whose_turn(), tiles);

   for(unsigned int ii=0; ii<tiles.size(); ii++) {
      vector<int> coords = {tiles[ii]->xx, tiles[ii]->yy};
      targets.push_back(coords);
   }
   return targets;
}

void game::clear_2d_vector(vector<vector<int>> to_clear) {
   for(unsigned int ii=0; ii<to_clear.size(); ii++) {
      to_clear[ii].clear();
   }
   to_clear.clear();
}

bool game::move(int x_from, int y_from, int x_to, int y_to) {
   
   if(game_over()) return false;
   if(stock[whose_turn()][queen] == 1) return false;
   
   // Figure out what tile we start from
   board* it = my_board;
   board* started = my_board;
   do {
      if(it->xx == x_from && it->yy == y_from) {
         vector<turn*> turns;
         piece *mp = it->ontop;
         while(mp->ontop != 0) mp = mp->ontop;
         mp->list_moves(turns, just_moved, whose_turn(), true);

         // Figure out what tile we move to
         for(unsigned int ii=0; ii<turns.size(); ii++) {
            if(   turns[ii]->from->xx == x_from && 
                  turns[ii]->from->yy == y_from && 
                  turns[ii]->to->xx == x_to && 
                  turns[ii]->to->yy == y_to) {
               perform_move(turns[ii]);
               delete turns[ii];
               return true;
            } else {
               delete turns[ii];
            }
         }
         return false;
      }
      it = it->next;
   } while(it != started);
   return false;
}

bool game::place(type kind, int x_to, int y_to) {

   if(stock[whose_turn()][int(kind)] == 0) return false;
   if(kind == empty) return false;
   if(game_over()) return false;

   // Do we have to place the queen bee?
   if(half_turns > 5 && stock[whose_turn()][queen] == 1 && kind != queen) {
      return false;
   }

   // Figure out which tile we want to place the piece at
   vector<board*> tiles;
   find_placeable_tiles(whose_turn(), tiles);
   for(unsigned int ii=0; ii<tiles.size(); ii++) {
      if(tiles[ii]->xx == x_to && tiles[ii]->yy == y_to) {
         turn* my_turn = new turn(kind, whose_turn(), tiles[ii]);
         perform_move(my_turn);
         delete my_turn;
         return true;
      }
   }
   return false;
}

bool game::undo() {
   if(half_turns == 0) return false;
   undo_move();
   return true;
}

#endif 

