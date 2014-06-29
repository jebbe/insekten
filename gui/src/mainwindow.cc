#ifndef mainwindow_cc
#define mainwindow_cc

#include "mainwindow.h"

//////////////////////////////
// CREATING THE MAIN WINDOW //
//////////////////////////////

MainWindow::MainWindow() {
   
   game_active = false;
   newgamedialog_open = false;
   my_move = new uiMove;
   my_move->origin_selected = false;
   my_move->dest_selected = false;
   
   createActions();
   createMenu();
   createToolbar();

   sceneWhite = new InventoryScene(false, this);
   sceneWhite->setSceneRect(QRectF(-XSHIFT, 0, WIDTH, NO_PIECES*(HEIGHT+2*YSHIFT)));
   viewWhite = new QGraphicsView(sceneWhite);
   viewWhite->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   viewWhite->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   connect(sceneWhite, SIGNAL(clicked_piece(int, int)), 
           this, SLOT(whiteInventoryOriginSelected(int, int)));
   connect(sceneWhite, SIGNAL(click_abort()), 
           this, SLOT(resetClicked()));
   
   sceneBlack = new InventoryScene(true, this);
   sceneBlack->setSceneRect(QRectF(-XSHIFT, 0, WIDTH, NO_PIECES*(HEIGHT+2*YSHIFT)));
   viewBlack = new QGraphicsView(sceneBlack);
   viewBlack->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   viewBlack->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   connect(sceneBlack, SIGNAL(clicked_piece(int, int)), 
           this, SLOT(blackInventoryOriginSelected(int, int)));
   connect(sceneBlack, SIGNAL(click_abort()), 
           this, SLOT(resetClicked()));
   
   sceneMain = new GameScene(this);
   sceneMain->setSceneRect(QRectF(0, 0, MAIN_SIZE, MAIN_SIZE));
   viewMain = new QGraphicsView(sceneMain);
   connect(sceneMain, SIGNAL(clicked_piece(int, int)), 
           this, SLOT(mainOriginSelected(int, int)));
   connect(sceneMain, SIGNAL(click_abort()), 
           this, SLOT(resetClicked()));
   
   QHBoxLayout *layout = new QHBoxLayout;
   viewWhite->setMinimumSize(WIDTH+8+2*XSHIFT, 0);
   viewBlack->setMinimumSize(WIDTH+8+2*XSHIFT, 0);
   layout->addWidget(viewWhite);
   layout->addWidget(viewMain);
   layout->addWidget(viewBlack);

   QWidget *widget = new QWidget;
   widget->setLayout(layout);

   setCentralWidget(widget);
   setWindowTitle(tr("Hive"));
   setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow() {
   delete my_move;
}

// void MainWindow::createInventories() {
//    
//    
//    QGraphicsPixmapItem* item;
//    QPixmap queenWhite(":/images/white_queen.png");
//    item = sceneWhite->addPixmap(queenWhite);
//    item->setPos(0, 0);   
//    
//    QPixmap queenBlack(":/images/black_queen.png");
//    item = sceneBlack->addPixmap(queenBlack);
//    item->setPos(0, 0);   
// }


void MainWindow::createActions() {
 
    newGameAction = new QAction(QIcon(":/images/newgame.png"),
                               tr("&New game"), this);
    newGameAction->setStatusTip(tr("Start a new game"));
    connect(newGameAction, SIGNAL(triggered()),
        this, SLOT(newGame()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setStatusTip(tr("Quit Hive"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    undoAction = new QAction(QIcon(":/images/undo.png"),
                               tr("&Undo"), this);
    undoAction->setStatusTip(tr("Undo the last move"));
    connect(undoAction, SIGNAL(triggered()),
        this, SLOT(undoMove()));

    aboutAction = new QAction(tr("A&bout"), this);
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));
}


void MainWindow::createMenu() {
   fileMenu = menuBar()->addMenu(tr("&File"));
   fileMenu->addAction(newGameAction);
   fileMenu->addAction(exitAction);

   editMenu = menuBar()->addMenu(tr("&Edit"));
   editMenu->addAction(undoAction);

   aboutMenu = menuBar()->addMenu(tr("&Help"));
   aboutMenu->addAction(aboutAction);
}


void MainWindow::createToolbar() {
   myToolBar = addToolBar(tr("Tools"));
   myToolBar->addAction(newGameAction);
   myToolBar->addAction(undoAction);
}

//////////////////////////////////
// ACTIONS FROM THE MAIN WINDOW //
//////////////////////////////////

void MainWindow::about() {
   QMessageBox::about(this, tr("About Hive"),
                      tr("This is <i>Hive</i>. "
                         "Imagine some witty info text here." ));
}


void MainWindow::newGame() {
   if(newgamedialog_open) return;
   newgamedialog_open = true;
   newgamedialog = new QDialog(this);
   ui = new Ui::NewGame;
   ui->setupUi(newgamedialog);
   newgamedialog->setFixedSize(270,308); 
   newgamedialog->show();
   connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(beginGame()));
   connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(abortBeginGame()));
}


void MainWindow::undoMove() {

   if(!game_active) return;

   if(newgamedialog_open) return;
   QMessageBox::about(this, tr("Undo"),
                      tr("Undo placeholder." ));
}

/////////////////////////
// CREATING A NEW GAME //
/////////////////////////

void MainWindow::abortBeginGame() {
   newgamedialog->close();
   newgamedialog_open = false;
   delete newgamedialog;
   delete ui;
}


void MainWindow::beginGame() {
   // Clear the running game
   if(game_active) {
      delete game;
   }
   my_move->origin_selected = false;
   my_move->dest_selected = false;

   // Set up a new game
   game_active = true;
   int my_rules = 0;
   my_rules += (int(ui->mosquito_check->isChecked()));
   my_rules += (2*int(ui->ladybug_check->isChecked()));
   my_rules += (4*int(ui->pillbug_check->isChecked()));
   game = new ai(ruleset(my_rules));
   
   white_human = ui->white_human->isChecked();
   black_human = ui->black_human->isChecked();
   white_level = ui->white_level->value();
   black_level = ui->black_level->value();
   
   // Clear the new game dialog
   newgamedialog->close();
   newgamedialog_open = false;
   delete newgamedialog;
   delete ui;
   
   // Set up the main display and inventories
   sceneWhite->redraw(game, my_move);
   sceneBlack->redraw(game, my_move);
   sceneMain->redraw(game, my_move);
   
}

/////////////////////////////
// PLAYING THE ACTUAL GAME //
/////////////////////////////

void MainWindow::whiteInventoryOriginSelected(int xx, int yy) {
   
   if(!game_active) return;
   
   type kind = sceneWhite->clicked_what(game, xx, yy);
   if(kind == empty) {
      resetClicked();
      return;
   }
   
   my_move->origin_selected = true;
   my_move->origin_type = kind;
   my_move->origin_color = false;
   my_move->dest_selected = false;

   sceneWhite->redraw(game, my_move);
   sceneBlack->redraw(game, my_move);
   sceneMain->redraw(game, my_move);
}


void MainWindow::blackInventoryOriginSelected(int xx, int yy) {
   
   if(!game_active) return;

   type kind = sceneBlack->clicked_what(game, xx, yy);
   if(kind == empty) {
      resetClicked();
      return;
   }

   my_move->origin_selected = true;
   my_move->origin_type = kind;
   my_move->origin_color = true;
   my_move->dest_selected = false;

   sceneWhite->redraw(game, my_move);
   sceneBlack->redraw(game, my_move);
   sceneMain->redraw(game, my_move);
}


void MainWindow::resetClicked() {

   if(!game_active) return;
   
   my_move->origin_selected = false;
   my_move->dest_selected = false;
   
   sceneWhite->redraw(game, my_move);
   sceneBlack->redraw(game, my_move);
   sceneMain->redraw(game, my_move);
}


void MainWindow::mainOriginSelected(int xx, int yy) {
   
   if(!game_active) return;
   
   // TODO
   
   sceneWhite->redraw(game, my_move);
   sceneBlack->redraw(game, my_move);
   sceneMain->redraw(game, my_move);
}


void MainWindow::mainDestSelected(int xx, int yy) {
   
   if(!game_active) return;
   
   // TODO
   
   sceneWhite->redraw(game, my_move);
   sceneBlack->redraw(game, my_move);
   sceneMain->redraw(game, my_move);
}


#endif
 
