#ifndef gamescene_h 
#define gamescene_h 

#include <QtGui>

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

};


#endif
