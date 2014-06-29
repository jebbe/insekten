#ifndef inventory_cc
#define inventory_cc

#include "inventory.h"

// true: black / false: white
InventoryScene::InventoryScene(bool color, QObject *parent) 
         : QGraphicsScene(parent) {
   my_color = color;
}


type InventoryScene::clicked_what(ai* game, double xx, double yy) {
   
   if(xx<0 || xx>WIDTH) return empty;
   
   int jj = int(yy+YSHIFT)/(HEIGHT+2*YSHIFT);
   if(jj == 0) return queen;
   if(jj == 1) return ant;
   if(jj == 2) return spider;
   if(jj == 3) return cricket;
   if(jj == 4) return beetle;
   if(jj == 5) {
      if((game->our_rules() & l) == l) return ladybug;
      if((game->our_rules() & m) == m) return mosquito;
      if((game->our_rules() & p) == p) return pillbug;
   }
   if(jj == 6) {
      if((game->our_rules() & l) == l &&
         (game->our_rules() & m) == m) return mosquito;
      if((game->our_rules() & l) != l &&
         (game->our_rules() & m) == m &&
         (game->our_rules() & p) == p) return pillbug;
      if((game->our_rules() & l) == l &&
         (game->our_rules() & m) != m &&
         (game->our_rules() & p) == p) return pillbug;
   }
   if(jj == 7) {
      if((game->our_rules() & l) == l &&
         (game->our_rules() & m) == m &&
         (game->our_rules() & p) == p) return pillbug;
   }

   return empty;
   
}


void InventoryScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *me) {

   if(me->button() == Qt::RightButton) {
      emit click_abort();
   } else if(me->button() == Qt::LeftButton) {
      QPointF where = me->scenePos();
      emit clicked_piece(int(where.x()), int(where.y()));
   }
}


void InventoryScene::redraw(ai* game, uiMove *my_move) {
   
   this->clear();
   
   QGraphicsPixmapItem* item;
   QPixmap image;

   int jj = 0;
   int stock;
   
   stock = game->in_stock(queen, my_color);
   for(int ii=0; ii<stock; ii++) {
      if(my_color) image.load(":/images/black_queen.png");
      else image.load(":/images/white_queen.png");
      item = this->addPixmap(image);
      item->setPos(-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
   }
   jj++;
   
   stock = game->in_stock(ant, my_color);
   for(int ii=0; ii<stock; ii++) {
      if(my_color) image.load(":/images/black_ant.png");
      else image.load(":/images/white_ant.png");
      item = this->addPixmap(image);
      item->setPos(-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
   }
   jj++;

   stock = game->in_stock(spider, my_color);
   for(int ii=0; ii<stock; ii++) {
      if(my_color) image.load(":/images/black_spider.png");
      else image.load(":/images/white_spider.png");
      item = this->addPixmap(image);
      item->setPos(-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
   }
   jj++;

   stock = game->in_stock(cricket, my_color);
   for(int ii=0; ii<stock; ii++) {
      if(my_color) image.load(":/images/black_cricket.png");
      else image.load(":/images/white_cricket.png");
      item = this->addPixmap(image);
      item->setPos(-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
   }
   jj++;

   stock = game->in_stock(beetle, my_color);
   for(int ii=0; ii<stock; ii++) {
      if(my_color) image.load(":/images/black_beetle.png");
      else image.load(":/images/white_beetle.png");
      item = this->addPixmap(image);
      item->setPos(-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
   }
   jj++;

   if((game->our_rules() & l) == l) {
      stock = game->in_stock(ladybug, my_color);
      for(int ii=0; ii<stock; ii++) {
         if(my_color) image.load(":/images/black_ladybug.png");
         else image.load(":/images/white_ladybug.png");
         item = this->addPixmap(image);
         item->setPos(-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
      }
      jj++;
   }

   if((game->our_rules() & m) == m) {
      stock = game->in_stock(mosquito, my_color);
      for(int ii=0; ii<stock; ii++) {
         if(my_color) image.load(":/images/black_mosquito.png");
         else image.load(":/images/white_mosquito.png");
         item = this->addPixmap(image);
         item->setPos(-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
      }
      jj++;
   }
      
   if((game->our_rules() & p) == p) {
      stock = game->in_stock(pillbug, my_color);
      for(int ii=0; ii<stock; ii++) {
         if(my_color) image.load(":/images/black_pillbug.png");
         else image.load(":/images/white_pillbug.png");
         item = this->addPixmap(image);
         item->setPos(-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
      }
   }
   
   if(  my_move->origin_selected &&
        my_move->origin_color == my_color &&
        my_move->origin_type != empty) {
      
      if(my_move->origin_type == queen) jj = 0;
      if(my_move->origin_type == ant) jj = 1;
      if(my_move->origin_type == spider) jj = 2;
      if(my_move->origin_type == cricket) jj = 3;
      if(my_move->origin_type == beetle) jj = 4;
      if(my_move->origin_type == ladybug) jj = 5;
      if(  my_move->origin_type == mosquito &&
           (game->our_rules() & l) == l) jj = 6;
      if(  my_move->origin_type == mosquito &&
           (game->our_rules() & l) != l) jj = 5;
      if(  my_move->origin_type == pillbug &&
           (game->our_rules() & l) == l &&
           (game->our_rules() & m) == m) jj = 7;
      if(  my_move->origin_type == pillbug &&
           (game->our_rules() & l) == l &&
           (game->our_rules() & m) != m) jj = 6;
      if(  my_move->origin_type == pillbug &&
           (game->our_rules() & l) != l &&
           (game->our_rules() & m) == m) jj = 6;
      if(  my_move->origin_type == pillbug &&
           (game->our_rules() & l) != l &&
           (game->our_rules() & m) != m) jj = 5;
      
      int ii = game->in_stock(my_move->origin_type, my_color) - 1;
      
      image.load(":/images/from_shade.png");
      item = this->addPixmap(image);
      item->setPos(-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
      
   }
   
   this->update();
   
}

#endif 
