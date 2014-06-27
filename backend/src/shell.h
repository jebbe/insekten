/*
 * 
 */
#ifndef shell_h
#define shell_h

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#include "ai.h"

using namespace std;

class shell {

private:
   
   bool ai_white, ai_black;
   ruleset rules;
   int search_depth;
   
   ai* game;

   void print_tile(int xx, int yy);
   void print();
   
   // A return false means an undo
   bool manual_move();
   void ai_move();
   
public:
   
   shell(bool ai_plays_white, bool ai_plays_black, 
         int difficulty, ruleset rules);
   ~shell();
   
   void play();
   
};

#endif 
  
