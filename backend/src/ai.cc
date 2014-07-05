#ifndef ai_cc
#define ai_cc

#include "ai.h"

ai::ai(ruleset rules) : game(rules) {
   has_stored_move = false;
}

ai::~ai() {
   if(has_stored_move) delete stored_move;
}

bool ai::generate_move(int max_depth) {
   vector<turn*> turns;
   find_all_moves(whose_turn(), turns);
   if(turns.size() > 0) {
      has_stored_move = true;
      stored_move = new turn();
      // The smartest AI ever: Choose the first move you find:
      stored_move = turns[0];
      return true;
   } else {
      return false;
   }
}

bool ai::perform_ai_move() {
   if(has_stored_move) {
      turn *move = new turn(*stored_move);
      perform_move(move);
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
 
