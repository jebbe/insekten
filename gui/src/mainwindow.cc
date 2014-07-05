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
   my_move->computer_just_moved = false;
   
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
           this, SLOT(mainPieceSelected(int, int)));
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
   
   // Could potentially make this work at some point - right now just cancel
   if(   ui->white_computer->isChecked() &&
         ui->black_computer->isChecked()) {
      abortBeginGame();
      QMessageBox::about(this, tr("Error"),
                         tr("Cannot have the computer play itself." ));
      return;
   }
   
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
   
   // If the computer plays white, we have to make it move now
   if(!white_human) computerMove(true);

   // Set up the main display and inventories
   sceneWhite->redraw(game, my_move);
   sceneBlack->redraw(game, my_move);
   sceneMain->redraw(game, my_move);

   my_move->computer_just_moved = false;
   
}

/////////////////////////////
// PLAYING THE ACTUAL GAME //
/////////////////////////////

void MainWindow::computerMove(bool color) {
   
   // Could potentially put this into a new thread and display a waiting message
   
   if(color) game->generate_move(black_level);
   else game->generate_move(white_level);
   
   game->perform_ai_move();

   my_move->computer_just_moved = true;
   my_move->origin_selected = false;
   //my_move->origin_color;
   my_move->origin_type = game->ai_move_kind();
   if(my_move->origin_type == empty) {
      my_move->origin_x = game->ai_move_x_from();
      my_move->origin_y = game->ai_move_y_from();
   }
   my_move->dest_selected = false;
   //my_move->dest_color;
   my_move->dest_type = game->ai_move_kind();
   my_move->dest_x = game->ai_move_x_to();
   my_move->dest_y = game->ai_move_y_to();
   
   game->delete_ai_move();

}


void MainWindow::whiteInventoryOriginSelected(int xx, int yy) {
   
   if(!game_active) return;
   
   if(game->whose_turn()) {
      resetClicked();
      return;
   }
   
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
   
   if(!game->whose_turn()) {
      resetClicked();
      return;
   }

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


void MainWindow::initiateMove(int ii, int jj, bool color) {
   if(game->can_move_to(ii, jj).size() == 0) {
      resetClicked();
      return;
   }
   
   my_move->origin_selected = true;
   my_move->origin_type = empty;
   my_move->origin_color = color;
   my_move->origin_x = ii;
   my_move->origin_y = jj;
   my_move->dest_selected = false;
}


void MainWindow::mainPieceSelected(int xx, int yy) {
   
   if(!game_active) return;
   
   // Figure out what color (or empty tile) we clicked on
   int ii = sceneMain->SceneToGameX(xx, yy);
   int jj = sceneMain->SceneToGameY(xx, yy);
   
   type kind = game->get_tile_type(ii, jj);
   bool color = game->get_tile_color(ii, jj);
   
   if(my_move->origin_selected == true) {
      
      // We are finishing up a move
      
      if(my_move->origin_type == empty) {
         
         // We're moving a tile
         
         if(!game->move(my_move->origin_x, my_move->origin_y, ii, jj)) {
            
            // Couldn't move; initiate another move instead
            initiateMove(ii, jj, color);
         } else {
            resetClicked();
 
            // We just moved. Make the computer move now
             if(game->whose_turn() && !black_human) {
               computerMove(true);
            } else if(!game->whose_turn() && !white_human) {
               computerMove(false);
            }

         }
         
     } else {
        
         // We're placing a new tile
         game->place(my_move->origin_type, ii, jj);
         resetClicked();
         
         // We just moved. Make the computer move now
          if(game->whose_turn() && !black_human) {
            computerMove(true);
         } else if(!game->whose_turn() && !white_human) {
            computerMove(false);
         }
      }
      
      // Are we done?
      if(game->game_over()) {
         if(game->white_wins()) {
            QMessageBox::about(this, tr("Game over"),
                         tr("White wins!" ));
         } else if(game->black_wins()) {
            QMessageBox::about(this, tr("Game over"),
                         tr("Black wins!" ));
         } else if(game->is_draw()) {
            QMessageBox::about(this, tr("Game over"),
                         tr("Draw." ));
         } else {
            QMessageBox::about(this, tr("Game over"),
                         tr("Game over, but nobody wins, and no draw. Probably a bug." ));
         }
         game_active = false;
      }
      
   } else {
      // We're initiating a move
      initiateMove(ii, jj, color);
   }
   
   sceneWhite->redraw(game, my_move);
   sceneBlack->redraw(game, my_move);
   sceneMain->redraw(game, my_move);
   
   my_move->computer_just_moved = false;
   
}


#endif
 
