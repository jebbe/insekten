#ifndef ai_cc
#define ai_cc

#include "ai.h"

ai::ai(ruleset rules) : game(rules) {
   has_stored_move = false;

   ifstream file;
   file.open("eval.txt");
   string junk;
   getline(file, junk);
   file >> weight[0] >> weight[1] >> weight[2] >> junk;
   for(int ii=0; ii<8; ii++) {
      file >> score[ii][0] >> score[ii][1] >> score[ii][2] >> score[ii][3]
           >> score[ii][4] >> junk;
   }
   file >> score_per_bee_freedom >> junk;
   file >> score_no_queen >> junk;
   file >> victory_score >> junk;
   file >> draw_score >> junk;
   file.close();
}


ai::~ai() {
   if(has_stored_move) delete stored_move;
}


float ai::eval(bool evalcolor, bool print) {
   
   bool draw = is_draw();
   bool blackw = black_wins();
   bool whitew = white_wins();
   
   if(draw) return draw_score;
   if(blackw && evalcolor) return victory_score;
   if(blackw && !evalcolor) return -victory_score;
   if(whitew && evalcolor) return -victory_score;
   if(whitew && !evalcolor) return victory_score;
   
   float index[4] = {0., 0., 0., 0.};
   int sign;
   for(int sign=-1; sign<=1; sign+=2) {
      
      bool color = ((sign == -1) ? !evalcolor : evalcolor);
     
      vector<turn*> turns;

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
         board* it = my_board;
         while(it->ontop == 0 || 
               it->ontop->kind != queen || 
               it->ontop->color != color) it = it->next;
         for(int ii=0; ii<6; ii++) {
            if(it->nbr[ii]->ontop == 0) {
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
         index[1] += float(sign) * weight[0] * float(kind[int(ii)])
                          * (score[ii][0] + score[ii][1])
                          / (npieces + score[ii][1]);
      }
      delete_moves(turns);
      turns.clear();
      
      // Number of placements
      find_all_placement_moves(color, turns);
      int kind_[NUM_TYPE] = {0,0,0,0,0,0,0,0};
      for(int ii=0; ii<int(turns.size()); ii++) {
         kind_[int(turns[ii]->kind)]++;
      }
      for(unsigned char ii=0; ii<NUM_TYPE; ii++) {
         index[2] += float(sign) * weight[1] 
                     * float(kind_[int(ii)]) * score[ii][2];
      }
      delete_moves(turns);
      turns.clear();
      
      // Number of pieces in stock
      for(unsigned char ii=0; ii<NUM_TYPE; ii++) {
         index[3] += float(sign) * weight[2] * float(stock[color][int(ii)])
                          * (score[ii][3] + score[ii][4])
                          / (float(stock[color][int(ii)]) + score[ii][4]);
      }
      
   }
   if(print) {
      cout << "Total score: " << index[0] + index[1] + index[2] + index[3] 
           << " | bee free neighbors: " << index[0] 
           << " | # moves around: " << index[1] 
           << " | # placements: " << index[2] 
           << " | # reserve: " << index[3] << endl;
   }
   return index[0] + index[1] + index[2] + index[3]; 
   
}


bool ai::generate_move(int max_depth) {
   vector<turn*> turns;
   find_all_moves(whose_turn(), turns);
   if(turns.size() > 0) {
      
      // Choose the move with the highest score
      float highest = - std::numeric_limits<float>::max();
      float points;
      int chosen = 0;
      for(int ii=0; ii<int(turns.size()); ii++) {
         perform_move(turns[ii]);
         points = eval(!whose_turn(), false); // !whose_turn() is me here!
         if(points > highest) {
            chosen = ii;
            highest = points;
         }
         undo_move();
      }
      if(has_stored_move) {
         cerr << "Error: Cannot generate another move." << endl;
         exit(-1);
      }
      has_stored_move = true;
      stored_move = new turn(*turns[chosen]);
      delete_moves(turns);
      return true;
   } else {
      delete_moves(turns);
      return false;
   }
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


#endif
 
