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
   
   float eval();
   float score[5][8];
   float weight[3];
   float score_no_queen, score_per_bee_freedom;
   
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
