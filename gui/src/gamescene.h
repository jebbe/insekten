#ifndef gamescene_h 
#define gamescene_h 

#include <QtGui>
#include <QtWidgets>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include <vector>

#include "ai.h"
#include "common.h"

class GameScene : public QGraphicsScene {
    Q_OBJECT

signals:
   
   void clicked_piece(int xx, int yy);
   void click_abort();

public slots:

   void mouseReleaseEvent(QGraphicsSceneMouseEvent *me);
    
public:

   GameScene(QObject *parent = 0);
   void setView(QGraphicsView *embeddedIn);
   
   void redraw(ai *game, uiMove *my_move, bool waiting_message = false);

   int SceneToGameX(int xx, int yy);
   int SceneToGameY(int xx, int yy);

private:
   
   QGraphicsView *myView;
   
   void draw_single_piece(int xx, int yy, type kind, bool color);
   int gameToSceneX(int xx, int yy, int stackheight);
   int gameToSceneY(int xx, int yy, int stackheight);

};


#endif
