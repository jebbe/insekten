#ifndef gamescene_h 
#define gamescene_h 

#include <QGraphicsScene>

#include "ai.h"
#include "common.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:

   GameScene(QObject *parent = 0);
   
   void redraw(ai *game, uiMove *my_move);

};


#endif
