/*
 * This class implements a move. It can either be a newly placed tile or a
 * movement of an existing tile to a different location.
 */
#ifndef turn_h
#define turn_h

#include <iostream>
#include <vector>

#include "board.h"
#include "piece.h"

using namespace std;

class board;
enum type : unsigned char;

class turn {

private:
   
public:

   // Where do I move?
   board* from;
   board* to;
   
   // Do I have to pass?
   bool pass;
   
   // I need a type when *from is zero, ie when we place a new piece
   type kind;
   bool color;
   
   turn(board* from, board* to);
   turn(type kind, bool color, board* to);
   turn();
   turn(turn& other);
   turn(bool pass);
   
   // This moves the pieces around on the board
   void perform();
   void undo();
   
};

#endif 
 
