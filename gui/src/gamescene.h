#ifndef gamescene_h 
#define gamescene_h 

#include <QtGui>
#include <vector>

#include "ai.h"
#include "common.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT

signals:
   
   void clicked_piece(int xx, int yy);
   void click_abort();

public slots:

   void mouseReleaseEvent(QGraphicsSceneMouseEvent *me);
    
public:

   GameScene(QObject *parent = 0);

   void redraw(ai *game, uiMove *my_move);

private:
   void draw_single_piece(int xx, int yy, type kind, bool color);
   int SceneToGameX(int xx, int yy);
   int SceneToGameY(int xx, int yy);
   int gameToSceneX(int xx, int yy, int stackheight);
   int gameToSceneY(int xx, int yy, int stackheight);

};


#endif
