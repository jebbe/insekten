#ifndef inventory_cc
#define inventory_cc

#include "inventory.h"

// true: black / false: white
InventoryScene::InventoryScene(bool color, QObject *parent) 
         : QGraphicsScene(parent) {
   
   my_color = color;
   
   QGraphicsPixmapItem* item;
   QPixmap image;

   int jj = 0;
   
   if(my_color) image.load(":/images/black_queen.png");
   else image.load(":/images/white_queen.png");
   item = this->addPixmap(image);
   item->setPos(0, jj*(HEIGHT+2*YSHIFT));
   jj++;
   
   for(int ii=0; ii<3; ii++) {
      if(my_color) image.load(":/images/black_ant.png");
      else image.load(":/images/white_ant.png");
      item = this->addPixmap(image);
      item->setPos(0-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
   }
   jj++;

   for(int ii=0; ii<2; ii++) {
      if(my_color) image.load(":/images/black_spider.png");
      else image.load(":/images/white_spider.png");
      item = this->addPixmap(image);
      item->setPos(0-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
   }
   jj++;

   for(int ii=0; ii<3; ii++) {
      if(my_color) image.load(":/images/black_cricket.png");
      else image.load(":/images/white_cricket.png");
      item = this->addPixmap(image);
      item->setPos(0-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
   }
   jj++;

   for(int ii=0; ii<2; ii++) {
      if(my_color) image.load(":/images/black_beetle.png");
      else image.load(":/images/white_beetle.png");
      item = this->addPixmap(image);
      item->setPos(0-ii*XSHIFT, jj*(HEIGHT+2*YSHIFT)-ii*YSHIFT);
   }
   jj++;

   if(my_color) image.load(":/images/black_ladybug.png");
   else image.load(":/images/white_ladybug.png");
   item = this->addPixmap(image);
   item->setPos(0, jj*(HEIGHT+2*YSHIFT));
   jj++;

   if(my_color) image.load(":/images/black_mosquito.png");
   else image.load(":/images/white_mosquito.png");
   item = this->addPixmap(image);
   item->setPos(0, jj*(HEIGHT+2*YSHIFT));
   jj++;

   if(my_color) image.load(":/images/black_pillbug.png");
   else image.load(":/images/white_pillbug.png");
   item = this->addPixmap(image);
   item->setPos(0, jj*(HEIGHT+2*YSHIFT));
   jj++;

}

#endif 
