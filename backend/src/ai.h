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

   // --------------------------------------------------- //
   // Defining the parameters for the evaluation function //
   // --------------------------------------------------- //

   // We evaluate the current board position based on these terms:
   // -1) Winning/losing the game or playing draw.
   //  0) The number of free tiles adjacent to the queen bee
   //  1) The number of possible movements around the board, where each 
   //     combination of origin/destiniation is considered one movement.
   //  2) The number of possible placements of new pieces.
   //  3) The number of pieces that have not been placed.
   //  4) Extra terms
   
   // -1) Game ends
   const float victory_score = 1.e12;
   const float draw_score = -30.;

   // 0) If the queen has been placed: 
   //    score_per_bee_freedom*[free tiles around the queen bee]
   const float score_per_bee_freedom = 60.;
   
   // 1) This term is calculated as 
   //       Movements * sum_over_pieces( (#moves * Movements_piece * mvmnts_dgr / (N + mvmnts_dgr) ) )
   //    where N is the move counter. mvmnts_dgr gives you the opportunity to make 
   //    flexibility less valuable later in the game.
   //    The higher movements_degrading, the slower the movability of a piece 
   //    becomes less valuable during the game.
   // 
   // 2) This term is calculated as 
   //       Stock * sum_over_pieces( (#pieces * Stock_piece * stock_dgr / (#pieces + stock_dgr) ) )
   //    Here, the number of pieces of this kind can be made less significant by
   //    increasing stock_dgr, such that e.g. two ants in stock aren't twice as valuable
   //    as one ant in stock.
   //    The higher stock_degrading, the more valuable additional unplaced pieces.
   //
   //    Warning: Dgr values smaller than one can screw up the evaluation function.
   const float movements_weight = 4;
   const float stock_weight = 40;
   
   const float score[8][4] = {
   // Movements_piece | Move dgr | Stock_piece | Stock dgr            
    { 10,               10000,      1,            1 },      // Queen       
    { 1,                 5,         1,            1.5 },    // Ant         
    { 3,                10,         1,            1 },      // Spider      
    { 3,                10,         1,            1 },      // Cricket     
    { 2,                10,         1,            1 },      // Beetle      
    { 1,                10,         1,            1 },      // Ladybug     
    { 1,                10,         1,            1 },      // Mosquito    
    { 3,                10,         1,            1 }};     // Pillbug     

   // 3) This term just adds up all the number of tiles we can place new pieces 
   //    on and weighs them according to the placement_weight.
   const float placement_weight = 10;

   // 4) Extra points for:
   //    - Beetles on top the pillbug
   //    - Beetles on top of the queen
   const float beetle_on_pillbug = 40;
   const float beetle_on_queen = 100;
   
   // Enforcing certain behaviour outside the evaluation function
   // draw_score comes into play here as well
   const float ant_first = -50.;
   const float queen_first = -100.;

   // TODO: 
   // (- Make moavble own pieces ok next to own queen) - maybe improve search depth for those?
   // - Pillbug extra score for next to own queen
   
#ifdef DEBUG
   int ncalls;
#endif
   
   bool has_stored_move;
   turn* stored_move;
   
   float eval(bool evalcolor, bool print);
   
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
