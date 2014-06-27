/*
 * This class implements the actual pieces of the game. Each piece needs to have
 * either a tile on which it lives, or another piece on top of which it lives.
 */
#ifndef piece_h
#define piece_h

#include <iostream>
#include <vector>

#include "turn.h"
#include "board.h"

using namespace std;

class turn;
class board;

enum type : unsigned char {
   queen=0, 
   ant=1, 
   spider=2, 
   cricket=3, 
   beetle=4, 
   ladybug=5, 
   mosquito=6, 
   pillbug=7, 
   NUM_TYPE=8, 
   empty=9
}; 

class piece {

private:
   
   void find_moves_queen(vector<turn*> &turns);
   void find_moves_ant(vector<turn*> &turns);
   void find_moves_spider(vector<turn*> &turns);
   void find_moves_cricket(vector<turn*> &turns);
   void find_moves_beetle(vector<turn*> &turns);
   void find_moves_ladybug(vector<turn*> &turns);
   void find_moves_mosquito(vector<turn*> &turns, board* just_moved);
   void find_moves_pillbug(vector<turn*> &turns, board* just_moved);

public:
   
   // I am associated with a player and I have an image printed on me.
   bool color; // true: black; false: white
   type kind;
   
   // Where on the board am I? And do I have somebody sitting on me?
   board* at;
   piece* ontop;
   
   // Don't call these directly, use the methods provided by board instead!
   piece(type kind, bool color, board* place_at);
   ~piece();
   
   // Find all the tiles this piece can go to and add them into the vector.
   void list_moves(vector<turn*> &turns, board* just_moved);
   
   // This is an expensive function, try to avoid if possible!
   void remove_duplicate_moves(vector<turn*> &turns);
   
};

#endif 
 
