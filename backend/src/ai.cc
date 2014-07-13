#ifndef ai_cc
#define ai_cc

#include "ai.h"

ai::ai(ruleset rules) : game(rules) {
   has_stored_move = false;
}


ai::~ai() {
   if(has_stored_move) delete stored_move;
}


float ai::eval(bool evalcolor, bool print) {
   
#ifdef DEBUG
   ncalls++;
#endif

   bool draw = is_draw();
   bool blackw = black_wins();
   bool whitew = white_wins();
   
   if(draw) return draw_score;
   if(blackw && evalcolor) return victory_score;
   if(blackw && !evalcolor) return -victory_score;
   if(whitew && evalcolor) return -victory_score;
   if(whitew && !evalcolor) return victory_score;
   
   float index[5] = {0., 0., 0., 0., 0.};
   for(int sign=-1; sign<=1; sign+=2) {
      
      // We evaluate for both players and add up their score with different
      // signs.
      bool color = ((sign == -1) ? !evalcolor : evalcolor);
     
      vector<turn*> turns;
      
      // Find the queen and the pillbug

      board* my_queen = my_board;
      if(stock[int(color)][queen] == 0) {
         while(my_queen->ontop == 0 || 
               my_queen->ontop->kind != queen || 
               my_queen->ontop->color != color) my_queen = my_queen->next;
      }
      
      board* my_pillbug = my_board;
      if((rules & p) == p && stock[int(color)][pillbug] == 0) {
         while(my_pillbug->ontop == 0 || 
               my_pillbug->ontop->kind != pillbug ||
               my_pillbug->ontop->color != color) my_pillbug = my_pillbug->next;
      }

      // Count number of pieces (not including any stacked ones - for simplicity
      int ii = 0;
      board* it = my_board;
      board* begin = it;
      do {
         if(it->ontop!=0) ii++;
         it = it->next;
      } while(it != begin);
      float npieces = float(ii);
      
      // Free neighboring tiles of the queen bee
      if(stock[int(color)][queen] == 0) {
         for(int ii=0; ii<6; ii++) {
            if(my_queen->nbr[ii]->ontop == 0) {
               index[0] += float(sign) * score_per_bee_freedom;
            }
         }
      } else {
         index[0] += float(sign) * score_no_queen;
      }
      
      // Number of moves around the board
      find_all_movement_moves(color, turns);
      int kind[NUM_TYPE] = {0,0,0,0,0,0,0,0};
      for(int ii=0; ii<int(turns.size()); ii++) {
         kind[int(turns[ii]->from->ontop->kind)]++;
      }
      for(unsigned char ii=0; ii<NUM_TYPE; ii++) {
         index[1] += float(sign) * movements_weight * float(kind[int(ii)])
                          * (score[ii][0] + score[ii][1])
                          / (npieces + score[ii][1]);
      }
      delete_moves(turns);
      turns.clear();
      
      // Number of placements
      vector<board*> tiles;
      find_placeable_tiles(color, tiles);
      index[2] += float(sign) * placement_weight * float(tiles.size());
      delete_moves(turns);
      turns.clear();
      
      // Number of pieces in stock
      for(unsigned char ii=0; ii<NUM_TYPE; ii++) {
         index[3] += float(sign) * stock_weight * float(stock[color][int(ii)])
                          * (score[ii][2] + score[ii][3])
                          / (float(stock[color][int(ii)]) + score[ii][3]);
      }
      
      // Extra points for beetles on pillbugs / queens
      if((rules & p) == p && stock[int(color)][pillbug] == 0) {
         if(my_pillbug->ontop->ontop != 0) {
            index[4] += - float(sign) * beetle_on_pillbug;
         }
      }
      if(stock[int(color)][queen] == 0) {
         if(my_queen->ontop->ontop != 0) {
            index[4] += - float(sign) * beetle_on_queen;
         }
      }
   }

#ifdef DEBUG
   if(print) {
      cout << "Total score: " << index[0] + index[1] + index[2] 
                               + index[3] + index[4]
           << ";  bee free neighbors: " << index[0] 
           << ";  moves around: " << index[1] 
           << ";  placements: " << index[2] 
           << ";  reserve: " << index[3]
           << ";  extra: " << index[4] << endl;
   }
#endif
   
   return index[0] + index[1] + index[2] + index[3] + index[4]; 
   
}

// I want to sort the moves with highest values to the first places, so I should
// use (ii->val > jj->val). However, we have performed a move and have thus 
// flipped whose_turn(), so we actually sort the smallest moves first, (which
// are the highest ones for myself).
bool compare_function(move_sorter *ii, move_sorter *jj) { 
   return (ii->val < jj->val); 
}

void ai::sort_moves(vector<turn*> &turns) {
   vector<move_sorter*> all_turns;
   for(int ii=0; ii<int(turns.size()); ii++) {
      move_sorter *my_move = new move_sorter;
      my_move->move = new turn(*turns[ii]);
      perform_move(turns[ii]);
      my_move->val = eval(whose_turn(), false);
      undo_move();
      all_turns.push_back(my_move);
   }
   delete_moves(turns);
   turns.clear();
   sort (all_turns.begin(), all_turns.end(), compare_function);
   for(int ii=0; ii<int(all_turns.size()); ii++) {
      turns.push_back(new turn(*all_turns[ii]->move));
      delete all_turns[ii];
   }
}


float ai::alphabeta(int depth, float alpha, float beta,
                    int initial_depth, turn* &best_move) {

   if (depth == 0 || game_over()) {
      return eval(whose_turn(), false);
   }
   
   float maxValue = alpha;
   vector<turn*> turns;
   find_all_moves(whose_turn(), turns);
   
   if(depth != 1) sort_moves(turns);
   
   for(int ii=0; ii<int(turns.size()); ii++) {
      perform_move(turns[ii]);

      // Selective deepening
      float value;
      if(   (initial_depth == 6 && depth == 4 && ii>int(turns.size())*2/3) ||
            (initial_depth == 6 && depth == 5 && ii>int(turns.size())/3)) {
         value = -eval(whose_turn(), false);
      } else {
         value = -alphabeta(depth-1, -beta, -maxValue, 
                            initial_depth, best_move);
      }
      undo_move();
      
      // Do we have a new best move?
      if(value > maxValue) {
         maxValue = value;
         if (maxValue >= beta)             
            break;          
         if (depth == initial_depth) {
            delete best_move;
            best_move = new turn (*turns[ii]);
         }
      }
   }
   delete_moves(turns);
   return maxValue;
}


bool ai::generate_move(int max_depth) {
   
#ifdef DEBUG
   ncalls = 0;
   clock_t t_init, t_final;
   t_init=clock();
#endif
   stored_move = new turn;
   float my_score = alphabeta(max_depth, 
             -std::numeric_limits<float>::max(), 
             std::numeric_limits<float>::max(),
             max_depth, stored_move);
   if(stored_move->from == 0 &&
      stored_move->to == 0 &&
      stored_move->kind == empty &&
      stored_move->color == false) {
         // We cannot move, we have to pass
         stored_move->pass = true;
   }
   has_stored_move = true;
#ifdef DEBUG
   t_final = clock() - t_init;
   cout << "Eval got involved " << ncalls << " times;  "
        << "total depth: " << max_depth << ".  "
        << "Max score: " << my_score << ";  "
        << "Time to create move: " 
        << (double)t_final / ((double)CLOCKS_PER_SEC) << endl;
#endif

   return true;
}


   
bool ai::perform_ai_move() {
   if(has_stored_move) {
      perform_move(stored_move);
      eval(!whose_turn(), true); // Print the current score
      return true;
   } else {
      return false;
   }
}


bool ai::delete_ai_move() {
   if(has_stored_move) {
      delete stored_move;
      has_stored_move = false;
      return true;
   } else {
      return false;
   }
}


type ai::ai_move_kind() {
   if(has_stored_move) {
      return stored_move->kind;
   } else {
      cout << "Cannot examine a non-existent move!" << endl;
      exit(-1);
   }
}


int ai::ai_move_x_from() {
   if(has_stored_move) {
      return stored_move->from->xx;
   } else {
      cout << "Cannot examine a non-existent move!" << endl;
      exit(-1);
   }
}


int ai::ai_move_y_from() {
   if(has_stored_move) {
      return stored_move->from->yy;
   } else {
      cout << "Cannot examine a non-existent move!" << endl;
      exit(-1);
   }
}


int ai::ai_move_x_to() {
   if(has_stored_move) {
      return stored_move->to->xx;
   } else {
      cout << "Cannot examine a non-existent move!" << endl;
      exit(-1);
   }
}


int ai::ai_move_y_to() {
   if(has_stored_move) {
      return stored_move->to->yy;
   } else {
      cout << "Cannot examine a non-existent move!" << endl;
      exit(-1);
   }
}


bool ai::ai_move_is_pass() {
   if(has_stored_move) {
      return stored_move->pass;
   } else {
      cout << "Cannot examine a non-existent move!" << endl;
      exit(-1);
   }
}


#endif
 
