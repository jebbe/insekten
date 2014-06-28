#ifndef inventory_h 
#define inventory_h 

#include <QtGui>

#include "common.h"

class InventoryScene : public QGraphicsScene
{
    Q_OBJECT

public:
   
   InventoryScene(bool color, QObject *parent = 0);
   
private:
   
   
   bool my_color;

};
#endif
 
