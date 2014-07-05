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

   if(me->button() != Qt::LeftButton) {
      emit click_abort();
   } else if(me->button() == Qt::LeftButton) {
      QPointF where = me->scenePos();
      emit clicked_piece(int(where.x()), int(where.y()));
   }
}


bool GameScene::clicked_is_outside(ai *game, double xx, double yy) {
   return false;
}

////////////////////////////////
// COORDINATE TRANSFORMATIONS //
////////////////////////////////

int GameScene::SceneToGameX(int xx, int yy) {
   int ii = xx - MAIN_SIZE/2 + WIDTH/2;
   if(ii<0) ii = (ii * 2) / (WIDTH + SPACING) - 1;
   else ii = (ii * 2) / (WIDTH + SPACING);
   return ii;
}


int GameScene::SceneToGameY(int xx, int yy) {
   int jj = yy - MAIN_SIZE/2 + WIDTH/2;
   int ii = SceneToGameX(xx, yy);
   jj = jj + ii*HEIGHT/2;
   if(jj>0) jj = jj / HEIGHT;
   else jj = jj/HEIGHT - 1;
   return jj;
}


int GameScene::gameToSceneX(int xx, int yy, int stackheight) {
   int sceneX = xx * (WIDTH - SPACING/2) - stackheight*XSHIFT;
   sceneX = sceneX + MAIN_SIZE/2 - WIDTH/2;
   return sceneX;
}


int GameScene::gameToSceneY(int xx, int yy, int stackheight) {
   int sceneY = yy * HEIGHT - stackheight*YSHIFT;
   sceneY = sceneY - xx*HEIGHT/2;
   sceneY = sceneY + MAIN_SIZE/2 - WIDTH/2;
   return sceneY;
}

////////////////////
// DRAW THE SCENE //
////////////////////

void GameScene::redraw(ai *game, uiMove *my_move) {
   
   this->clear();

   QGraphicsPixmapItem* item;
   QPixmap image;

   int xmin = game->x_min();
   int ymin = game->y_min();
   int xmax = game->x_max();
   int ymax = game->y_max();
   
   // Draw the TO markers on empty tiles
   vector<vector<int> > targets;
   if(my_move->origin_selected) {
      
      if(my_move->origin_type != empty) {
         if(!game->need_to_place_queen() || my_move->origin_type == queen)
            targets = game->can_place_at();
      } else {
         targets = game->can_move_to(my_move->origin_x, my_move->origin_y);
      }
   
      for(int kk=0; kk<int(targets.size()); kk++) {
         
         if(game->get_tile_type(targets[kk][0], targets[kk][1]) == empty) {
            int ii = gameToSceneX(targets[kk][0], targets[kk][1], 0);
            int jj = gameToSceneY(targets[kk][0], targets[kk][1], 0);
            image.load(":/images/to_shade.png");
            item = this->addPixmap(image);
            item->setPos(ii, jj);
         }
      }
   }

   // Draw the pieces on the board
   for(int ii=xmin; ii<=xmax; ii++) {
      for(int jj=ymin; jj<=ymax; jj++) {
         
         vector<type> kind;
         vector<bool> color;
         game->buried_tiles(ii, jj, kind, color);
         int kk;
         for(kk=0; kk<int(kind.size()); kk++) {
            draw_single_piece(gameToSceneX(ii, jj, kk), gameToSceneY(ii, jj, kk), 
                              kind[kk], color[kk]);
         }
         draw_single_piece(gameToSceneX(ii, jj, kk), gameToSceneY(ii, jj, kk), 
                     game->get_tile_type(ii, jj), game->get_tile_color(ii, jj));

      }
   }

   // Draw the TO markers on occupied tiles
   if(my_move->origin_selected) {
      
      for(int kk=0; kk<int(targets.size()); kk++) {
         
         // Figure out the height of the stack
         vector<type> kind;
         vector<bool> color;
         game->buried_tiles(targets[kk][0], targets[kk][1], kind, color);
         int stackheight = color.size();
         
         if(game->get_tile_type(targets[kk][0], targets[kk][1]) != empty) {
            int ii = gameToSceneX(targets[kk][0], targets[kk][1], stackheight);
            int jj = gameToSceneY(targets[kk][0], targets[kk][1], stackheight);
            image.load(":/images/to_shade.png");
            item = this->addPixmap(image);
            item->setPos(ii, jj);
         }
      }
   }
   
   // Draw the FROM marker
   if(  my_move->origin_selected &&
        my_move->origin_type == empty) {
      
      // Figure out the height of the stack
      vector<type> kind;
      vector<bool> color;
      game->buried_tiles(my_move->origin_x, my_move->origin_y, kind, color);
      int stackheight = color.size();
   
      int ii = gameToSceneX(my_move->origin_x, my_move->origin_y, stackheight);
      int jj = gameToSceneY(my_move->origin_x, my_move->origin_y, stackheight);
      
      image.load(":/images/from_shade.png");
      item = this->addPixmap(image);
      item->setPos(ii, jj);
   }
   
   // Draw the "Computer just moved" markers
   if(my_move->computer_just_moved) {
      
      // Figure out the height of the stack
      vector<type> kind;
      vector<bool> color;
      game->buried_tiles(my_move->dest_x, my_move->dest_y, kind, color);
      int stackheight = color.size();
      
      int ii = gameToSceneX(my_move->dest_x, my_move->dest_y, stackheight);
      int jj = gameToSceneY(my_move->dest_x, my_move->dest_y, stackheight);
      
      image.load(":/images/to_shade.png");
      item = this->addPixmap(image);
      item->setPos(ii, jj);
      
      // Did we move, i.e. do we have to draw a from marker?
      if(my_move->origin_type == empty) {
         
         // Figure out the height of the stack
         vector<type> kind;
         vector<bool> color;
         game->buried_tiles(my_move->origin_x, my_move->origin_y, kind, color);
         int stackheight = color.size();
      
         int ii = gameToSceneX(my_move->origin_x, my_move->origin_y, stackheight);
         int jj = gameToSceneY(my_move->origin_x, my_move->origin_y, stackheight);
         
         image.load(":/images/from_shade.png");
         item = this->addPixmap(image);
         item->setPos(ii, jj);
      }
   }
   
   game->clear_2d_vector(targets);
   this->update();
}


void GameScene::draw_single_piece(int xx, int yy, type kind, bool color) {
   QGraphicsPixmapItem* item;
   QPixmap image;
   
   switch(kind) {
      case queen:
         if(color) image.load(":/images/black_queen.png");
         else image.load(":/images/white_queen.png");
         item = this->addPixmap(image);
         item->setPos(xx, yy);
         break;
      case ant:
         if(color) image.load(":/images/black_ant.png");
         else image.load(":/images/white_ant.png");
         item = this->addPixmap(image);
         item->setPos(xx, yy);
         break;
      case spider:
         if(color) image.load(":/images/black_spider.png");
         else image.load(":/images/white_spider.png");
         item = this->addPixmap(image);
         item->setPos(xx, yy);
         break;
      case cricket:
         if(color) image.load(":/images/black_cricket.png");
         else image.load(":/images/white_cricket.png");
         item = this->addPixmap(image);
         item->setPos(xx, yy);
         break;
      case beetle:
         if(color) image.load(":/images/black_beetle.png");
         else image.load(":/images/white_beetle.png");
         item = this->addPixmap(image);
         item->setPos(xx, yy);
         break;
      case ladybug:
         if(color) image.load(":/images/black_ladybug.png");
         else image.load(":/images/white_ladybug.png");
         item = this->addPixmap(image);
         item->setPos(xx, yy);
         break;
      case mosquito:
         if(color) image.load(":/images/black_mosquito.png");
         else image.load(":/images/white_mosquito.png");
         item = this->addPixmap(image);
         item->setPos(xx, yy);
         break;
      case pillbug:
         if(color) image.load(":/images/black_pillbug.png");
         else image.load(":/images/white_pillbug.png");
         item = this->addPixmap(image);
         item->setPos(xx, yy);
         break;
      default:
         break;
   }

}

#endif
 
