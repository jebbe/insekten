#ifndef gamescene_h 
#define gamescene_h 

#include <QGraphicsScene>

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
//     enum Mode { InsertItem, InsertLine, InsertText, MoveItem };
// 
//     DiagramScene(QMenu *itemMenu, QObject *parent = 0);
   GameScene(QObject *parent = 0);
//     QFont font() const
//         { return myFont; }
//     QColor textColor() const
//         { return myTextColor; }
//     QColor itemColor() const
//         { return myItemColor; }
//     QColor lineColor() const
//         { return myLineColor; }
//     void setLineColor(const QColor &color);
//     void setTextColor(const QColor &color);
//     void setItemColor(const QColor &color);
//     void setFont(const QFont &font);

public slots:
//     void setMode(Mode mode);
//     void setItemType(DiagramItem::DiagramType type);
//     void editorLostFocus(DiagramTextItem *item);

signals:
//     void itemInserted(DiagramItem *item);
//     void textInserted(QGraphicsTextItem *item);
//     void itemSelected(QGraphicsItem *item);

protected:
//     void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
//     void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
//     void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
//     bool isItemChange(int type);
// 
//     DiagramItem::DiagramType myItemType;
//     QMenu *myItemMenu;
//     Mode myMode;
//     bool leftButtonDown;
//     QPointF startPoint;
//     QGraphicsLineItem *line;
//     QFont myFont;
//     DiagramTextItem *textItem;
//     QColor myTextColor;
//     QColor myItemColor;
//     QColor myLineColor;
};


#endif
