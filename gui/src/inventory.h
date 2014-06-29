#ifndef inventory_h 
#define inventory_h 

#include <QtGui>

#include "ai.h"
#include "common.h"

class InventoryScene : public QGraphicsScene {
   
   Q_OBJECT

// signals:
//    
//    void clicked();

signals:
   
   void clicked_piece(int xx, int yy);
   void click_abort();

public slots:

   void mouseReleaseEvent(QGraphicsSceneMouseEvent *me);

public:
   
   InventoryScene(bool color, QObject *parent = 0);

   type clicked_what(ai *game, double xx, double yy);

   void redraw(ai* game, uiMove *my_move);
   
private:
   
   bool my_color;
   
};
#endif
 
