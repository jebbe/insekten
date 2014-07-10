/*
 * This class provides an AI for the game of hive. It inherits from game,
 * so all funtionality of a game of hive can be accessed.
 */
#ifndef ai_h
#define ai_h

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>

#include "game.h"

using namespace std;

class ai : public game {

private:
   
   bool has_stored_move;
   turn* stored_move;
   
   float eval(bool evalcolor, bool print);
   
   // score[kind][term]
   // kind: What insect? Refers to the type enum.
   // term: 0: Movements
   //       1: Mvmnts dgr
   //       2: Placements
   //       3: Stock
   //       4: Stock degrading
   float score[8][5];
   
   // weight[term]
   // term: 0: Movements
   //       1: Placements
   //       2: Stock
   float weight[3];
   
   // How many points for not setting the queen, how many for every free
   // tile around the bee?
   float score_no_queen, score_per_bee_freedom;
   float victory_score, draw_score;
   
   float alphabeta(bool player, int depth, float alpha, float beta,
                   int initial_depth, turn* &best_move);
   
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
   
   type ai_move_kind();
   int ai_move_x_from();
   int ai_move_y_from();
   int ai_move_x_to();
   int ai_move_y_to();

};

#endif  
