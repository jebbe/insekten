#ifndef gamescene_cc
#define gamescene_cc

#include "gamescene.h"

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent) {
   
   QGraphicsPixmapItem* item;
   QPixmap image;

   image.load(":/images/welcome.png");
   item = this->addPixmap(image);
   item->setPos(MAIN_SIZE/2-image.width()/2, MAIN_SIZE/2-image.height()/2);
   
   this->update();
}


void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *me) {

   if(me->button() == Qt::RightButton) {
      emit click_abort();
   } else if(me->button() == Qt::LeftButton) {
      QPointF where = me->scenePos();
      emit clicked_piece(int(where.x()), int(where.y()));
   }
}


void GameScene::redraw(ai *game, uiMove *my_move) {

   this->clear();
   
   
   
   this->update();
}

#endif
 
