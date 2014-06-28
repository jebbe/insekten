#ifndef mainwindow_h 
#define mainwindow_h

#include <QMainWindow>
#include <QtGui>
//#include <QLabel>

#include "ai.h"
#include "gamescene.h"
#include "inventory.h"
#include "ui_newgame.h"
#include "common.h"

class MainWindow : public QMainWindow {
   
    Q_OBJECT

public:
   MainWindow();

private slots:
   
   void newGame();
   void about();
   void undoMove();
   
   void beginGame();
   void abortBeginGame();

private:
   
   bool game_active;
   ai* game;
   bool white_human;
   bool black_human;
   int white_level;
   int black_level;
   
   void createActions();
   void createMenu();
   void createToolbar();

   GameScene *sceneMain;
   QGraphicsView *viewMain;

   InventoryScene *sceneWhite;
   QGraphicsView *viewWhite;
   
   InventoryScene *sceneBlack;
   QGraphicsView *viewBlack;

   QAction *exitAction;
   QAction *undoAction;
   QAction *aboutAction;
   QAction *newGameAction;

   QMenu *fileMenu;
   QMenu *editMenu;
   QMenu *aboutMenu;

   QToolBar *myToolBar;

   QDialog *newgamedialog;
   Ui::NewGame *ui;
   
};

#endif
