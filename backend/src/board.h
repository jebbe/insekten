/*
 * This class represents an empty board. Each instance represents a single tile,
 * and all the tiles together form a bidirectional graph to represent the 
 * geometry as well as a bidirectional list to enable searching.
 */

#ifndef board_h
#define board_h

#include <iostream>
#include <vector>
#include <climits>

#include "piece.h"
#include "turn.h"

using namespace std;

class piece;
class turn;

class board {

private:
   
   const int nbrx[6] = {1, 1, 0,-1,-1, 0};
   const int nbry[6] = {0, 1, 1, 0,-1,-1};

   // Iteratively find a path to *to. Sterted by has_path_to().
   bool path_search(board* to);

public:
   
   // Position on the board
   int xx, yy;
   
   // Bidirectional graph:
   board** nbr;
   
   // Flag to help search the graph.
   bool visited;
   
   // Bidirectional circular list:
   board* next;
   board* prev;
   
   // pieces on the tile
   piece* ontop;
   
   // Create and delete tiles from the board
   board(board* some_board, int xx=SHRT_MAX, int yy=SHRT_MAX);
   ~board();
   void delete_all();
   
   // Add and remove pieces. This usually implies creating or deleting tiles.
   // Will always operate on the uppermost layer of pieces.
   void place_piece(piece* bug);
   void remove_piece();

   // Figure out if the one hive rule is fulfilled when THIS is removed
   bool one_hive();
   
   // A graph search: figure out if we have a path to the *to-tile using only 
   // tiles with pieces on them and without passing across the *without-tile.
   bool has_path_to(board* to, board* without);
   
   // Set all visited flags to false using the list
   void set_not_visited();

   // This is the basic movement on which many other bugs base theirs
   bool bee_can_move_to(int nbr_no);

   // A beetle or ladybug cannot pass when two beetles left and right on top
   // of other pieces block the passage
   bool impass_high_lvl(int nbr_no, bool remove_current_piece);
   
   // Find all the tiles an ant can move to
   void ant_graph_search(vector<turn*> &turns, board* origin);

};

#endif 
