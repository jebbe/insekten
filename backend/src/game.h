/*
 * This class provides the methods to manage a game of hive. It provides the
 * rules, taking advantage of the board, pieces and turns classes, and it
 * provides an interface for access by a UI.
 */

#ifndef game_h
#define game_h

#include <iostream>
#include <vector>

#include "board.h"
#include "turn.h"
#include "piece.h"

using namespace std;

enum ruleset : int {
   m = 1, // mosquito
   l = 2, // ladybug
   p = 4, // pillbug
   nqf = 8 // Cannot place queen first
};

inline ruleset operator|(ruleset a, ruleset b) {
   return static_cast<ruleset>(static_cast<int>(a) | static_cast<int>(b));
}

class game {

private:
   
   // Keep a history of turns
   int half_turns;
   vector<turn*> history;
   vector<board*> just_moved_history;
   
   // Remember which piece has just moved and thus cannot be moved right again
   board* just_moved;
   
   // print the content of a single tile
   void print_tile(board* it);
   
   // Where can I place new pieces?
   void find_placeable_tiles(bool color, vector<board*> &tiles);
   
   // Help figuring out whether somebody won
   bool queen_surrounded(bool color);
   //bool no_legal_move(bool color);

// These routines use the internal data structures and are meant to be used by
// the AI
protected:

   board* my_board;
   ruleset rules;

   // Keep track of the not-yet-placed pieces
   int stock[2][NUM_TYPE];

   // List all the possible moves of all pieces
   void find_all_moves(bool color, vector<turn*> &turns);
   void find_all_placement_moves(bool color, vector<turn*> &turns);
   void find_all_movement_moves(bool color, vector<turn*> &turns);
   void delete_moves(vector<turn*> &turns);
   
   // Carry out a turn or undo it. Will create a copy of the turn object, so
   // that it can be deleted after calling perform_move().
   void perform_move(turn* go);
   void undo_move();

   // Debugging routines: Print the board to stdout, optionally with a list of 
   // moves to mark. Marking moves only makes sense for moves of one single 
   // piece.
   void print();
   void print(vector<turn*> &turns);

// These routines hide the internal data structures and are meant for access
// by some UI. Access is by an x/y grid, the internal graph and list structures
// are hidden.
public:
   
   game(ruleset rules);
   ~game();

   // Except for the buried_tiles routine every routine operates on the topmost
   // piece of a stack.

   // Query game properties. For outside acces purposes, type is an enum that
   // provides the following members: {bee, ant, spider, cricket, dungbeetle, 
   //                                  ladybug, mosquito, none}
   // Where none represents an empty tile.
   int x_min();
   int x_max();
   int y_min();
   int y_max();
   type get_tile_type(int xx, int yy);
   bool get_tile_color(int xx, int yy);
   int in_stock(type kind, bool color);
   bool need_to_place_queen();
   bool whose_turn();
   bool buried_tiles(int xx, int yy, vector<type> &kind, vector<bool> &color);
   bool buried_tiles(int xx, int yy);
   bool game_over();
   bool white_wins();
   bool black_wins();
   bool is_draw();
   ruleset our_rules();
   int turn_count() { return half_turns; }
   
   // Figure out where a certain pieces can move and where we can place new
   // pieces. Returns a vector containing x- and y-coordinates of the tiles.
   vector<vector<int> > can_move_to(int xx, int yy);
   vector<vector<int> > can_place_at();
   void clear_2d_vector(vector<vector<int> > to_clear);
   
   // Manipulate the game by playing.
   bool move(int x_from, int y_from, int x_to, int y_to);
   bool place(type kind, int x_to, int y_to);
   bool undo();
   bool pass();
   
};

#endif 
