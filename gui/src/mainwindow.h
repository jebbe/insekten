#ifndef mainwindow_h 
#define mainwindow_h

#include <QtGui>
#include <QMainWindow>
#include <QGraphicsView>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>

#include "ai.h"
#include "gamescene.h"
#include "inventory.h"
#include "ui_newgame.h"
#include "common.h"

class MainWindow : public QMainWindow {
   
    Q_OBJECT

public:
   MainWindow();
   ~MainWindow();

private slots:
   
   void newGame();
   void about();
   void undoMove();
   
   void beginGame();
   void abortBeginGame();
   
   void whiteInventoryOriginSelected(int xx, int yy);
   void blackInventoryOriginSelected(int xx, int yy);
   void resetClicked();
   void mainPieceSelected(int xx, int yy);
   
private:

   void initiateMove(int ii, int jj, bool color);

   bool game_active;
   ai* game;
   bool white_human;
   bool black_human;
   int white_level;
   int black_level;
   
   void createActions();
   void createMenu();
   void createToolbar();
   
   void computerMove(bool color);
   
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
   bool newgamedialog_open;
   
   uiMove *my_move;
};

#endif
