#ifndef shell_cc
#define shell_cc

#include "shell.h"

shell::shell(bool ai_plays_white, bool ai_plays_black, 
             int difficulty, ruleset rules) {
   ai_white = ai_plays_white;
   ai_black = ai_plays_black;
   search_depth = difficulty;
   this->rules = rules;
   game = new ai(rules);
}

shell::~shell() {
   delete game;
}

void shell::play() {
   
//    // Test the no-impass rule
//    game->place(queen, 0, 0);
//    game->place(queen, 0, 1);
//    game->place(ant, 0, -1);
//    game->place(ant, 0, 2);
//    game->move(0, -1, -1, 0);
//    game->move(0, 2, 1, 1);
//    game->place(beetle, -1, -1);
//    game->place(beetle, 1, 2);
//    game->move(-1, -1, -1, 0);
//    game->move(1, 2, 1, 1);
//    game->place(beetle, 0, -1);
//    game->place(cricket, 0, 2);
//    game->move(0, -1, 0, 0);
//   game->place(cricket, 0, 3);

// Test the pillbug
//    cout << game->place(queen, 0, 0) << endl;
//    cout << game->place(queen, 0, 1) << endl;
//    cout << game->place(cricket, 0, -1) << endl;
//    cout << game->place(pillbug, 0, 2) << endl;
//    cout << game->move(0, -1, 0, 3) << endl;
//    cout << "ZERO: " << game->move(0, 3, 1, 2) << endl;
//   cout << "ZERO: " << game->move(1, 2, -1, 0) << endl;
//    cout << game->place(spider, 0, -1) << endl;
//    cout << game->place(ant, -1, 1) << endl;
//    cout << game->move(1, 2, -1, 2) << endl;
//    cout << "ZERO: " << game->move(-1, 2, 0, 3) << endl;

   vector<int> coords;
   
   bool white = true;
   
   while(true) {
      
      if(white) {
         print();
         if(ai_white) {
            ai_move();
            white = false;
         } else {
            if(manual_move()) {
               white = false;
            } else {
               // Undo
               if(game->undo()) {
                  white = false;
               } else {
                  white = true;
               }
            }
         }
         if(game->game_over()) { print(); cout << "Game over" << endl; return; }
      }

      if(!white) {
         print();
         if(ai_black) {
            ai_move();
            white = true;
         } else {
            if(manual_move()) {
               white = true;
            } else {
               // Undo
               if(game->undo()) {
                  white = true;
               } else {
                  white = false;
               }
            }
         }
         if(game->game_over()) { print(); cout << "Game over" << endl; return; }
      }
   }
}

bool shell::manual_move() {

   char in = ' ', what = ' ';
   int x_from, y_from, x_to, y_to;
   bool success;
   
   cin.clear();
   do {
      
      while(in!='n' && in!='m' && in!='q' && in!='u') {
         if(game->whose_turn()) {
            cout << "BLACK: ";
         } else {
            cout << "white: ";
         }
         cout << "(n/m/u/q) ((type x y)/(x y x y)): ";
         cin >> in;
      }
      
      if(in=='q') exit(0);

      // Undo
      if(in=='u') return false;
      
      // Place new tile
      if(in=='n') {
         cin >> what >> x_to >> y_to;
         type kind;
         switch(what) {
            case 'q': kind = queen; break;
            case 'a': kind = ant; break;
            case 's': kind = spider; break;
            case 'c': kind = cricket; break;
            case 'b': kind = beetle; break;
            case 'l': kind = ladybug; break;
            case 'm': kind = mosquito; break;
            case 'p': kind = pillbug; break;
            default: kind = empty; break;
         }
         success = game->place(kind, x_to, y_to);
         if(!success) cout << "Couldn't place new piece." << endl;
      }
      
      // Move existing tile
      if(in=='m') {
         cin >> x_from >> y_from >> x_to >> y_to;
         success = game->move(x_from, y_from, x_to, y_to);
         if(!success) cout << "Couldn't move the piece." << endl;
      }
      
      in=' ';
      what = ' ';
      
   } while(!success);
   return true;
}

void shell::ai_move() {
   if(!game->generate_move(search_depth)) {
      cout << "Cannot move legally." << endl;
      exit(-1);
   }
   if(!game->perform_ai_move()) {
      cout << "You found a bug in the hive." << endl;
      exit(-1);
   }
   cout << "Computer has moved." << endl;
}

void shell::print_tile(int xx, int yy) {
   
   switch(game->get_tile_type(xx, yy)) {
      case queen:
         if(game->get_tile_color(xx, yy)) cout << "  Q ";
         else cout << "  q ";
         break;
      case ant:
         if(game->get_tile_color(xx, yy)) cout << "  A ";
         else cout << "  a ";
         break;
      case spider:
         if(game->get_tile_color(xx, yy)) cout << "  S ";
         else cout << "  s ";
         break;
      case cricket:
         if(game->get_tile_color(xx, yy)) cout << "  C ";
         else cout << "  c ";
         break;
      case beetle:
         if(game->buried_tiles(xx, yy)) {
            if(game->get_tile_color(xx, yy)) cout << "  B*";
            else cout << "  b*";
         } else {
            if(game->get_tile_color(xx, yy)) cout << "  B ";
            else cout << "  b ";
         }
         break;
      case ladybug:
         if(game->get_tile_color(xx, yy)) cout << "  L ";
         else cout << "  l ";
         break;
      case mosquito:
         if(game->buried_tiles(xx, yy)) {
            if(game->get_tile_color(xx, yy)) cout << "  M*";
            else cout << "  m*";
         } else {
            if(game->get_tile_color(xx, yy)) cout << "  M ";
            else cout << "  m ";
         }
         break;
      case pillbug:
         if(game->get_tile_color(xx, yy)) cout << "  P ";
         else cout << "  p ";
         break;
      case empty:
         cout << "  . ";
         break;
      default:
         cerr << "NUM_TYPE is not a valid piece." << endl;
         exit(-1);
   }
}

void shell::print() {
   int xmin = game->x_min();
   int xmax = game->x_max();
   int ymin = game->y_min();
   int ymax = game->y_max();
   
   for(int xx=xmin; xx<=xmax; xx++) {
      cout << setw(4) << xx;
      for(int ii=xx; ii<xmax; ii++) cout << "  ";
      for(int yy=ymin; yy<=ymax; yy++) {
         print_tile(xx, yy);
      }
      cout << endl;
   }
   cout << "  ";
   for(int yy=ymin; yy<=ymax; yy++) cout << setw(4) << yy;
   cout << endl;
}

#endif 
 
