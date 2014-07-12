/*
 * This class provides an AI for the game of hive. It inherits from game,
 * so all funtionality of a game of hive can be accessed.
 */
#ifndef ai_h
#define ai_h

#define DEBUG

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#ifdef DEBUG
#include <time.h>
#endif

#include "game.h"


using namespace std;

// A data structure to help sort our moves by their eval-value. Needed for
// efficient alpha-beta pruning
struct move_sorter {
   turn *move;
   float val;
};

// And a function to define an order on the moves for sorting.
bool compare_function(move_sorter *ii, move_sorter *jj);


class ai : public game {

private:

#ifdef DEBUG
   int ncalls;
#endif
   
   bool has_stored_move;
   turn* stored_move;
   
   float eval(bool evalcolor, bool print);
   
   // score[kind][term]
   // kind: What insect? Refers to the type enum.
   // term: 0: Movements
   //       1: Mvmnts dgr
   //       2: Stock
   //       3: Stock degrading
   float score[8][4];
   
   // weight[term]
   // term: 0: Movements
   //       1: Stock
   float weight[2];
   
   // How many points for not setting the queen, how many for every free
   // tile around the bee?
   float score_no_queen, score_per_bee_freedom;
   
   // How bad do we want to win / avoid a draw?
   float victory_score, draw_score;
   
   // How important is being able to place new pieces?
   float placement_weight;
   
   // Extra points
   float beetle_on_pillbug, beetle_on_queen;
   
   // Minimax with alpha-beta-pruning
   float alphabeta(int depth, float alpha, float beta,
                   int initial_depth, turn* &best_move);
   
   // To increase the chance of cutoffs, we presort the moves by evaluating
   // them with the evaluation function. As of yet, this is just a bubble sort.
   void sort_moves(vector<turn*> &turns);
   
public:
   
   ai(ruleset rules);
   ~ai();
   
   // Invoke the AI. First we have to generate a move that will then be stored
   // inside the AI object and can be executed by perform_ai_move(). In between
   // you can query information about the move: If a piece is newly placed,
   // the type will give the kind of piece we place. If the type is "empty",
   // a piece is moved around.
   bool generate_move(int max_depth);
   bool perform_ai_move();
   bool delete_ai_move();
   
   bool ai_move_is_pass();
   type ai_move_kind();
   int ai_move_x_from();
   int ai_move_y_from();
   int ai_move_x_to();
   int ai_move_y_to();

};

#endif  
