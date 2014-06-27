#ifndef mainwindow_cc
#define mainwindow_cc

#include "mainwindow.h"

#define HEIGHT 87
#define INNER_HEIGHT 70
#define WIDTH 100
#define NO_PIECES 6 // Should be determined automatically depending on the rules

MainWindow::MainWindow()
{
   createActions();
   createInventories();
   createMenu();

   scene = new GameScene(this);
   scene->setSceneRect(QRectF(0, 0, 6000, 6000));
   view = new QGraphicsView(scene);
//     connect(scene, SIGNAL(itemInserted(DiagramItem*)),
//             this, SLOT(itemInserted(DiagramItem*)));
//     connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
//         this, SLOT(textInserted(QGraphicsTextItem*)));
//     connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
//         this, SLOT(itemSelected(QGraphicsItem*)));

   createToolbar();

   QHBoxLayout *layout = new QHBoxLayout;
   viewWhite->setMinimumSize(WIDTH+8, 0);
   viewBlack->setMinimumSize(WIDTH+8, 0);
   layout->addWidget(viewWhite);
   layout->addWidget(view);
   layout->addWidget(viewBlack);

   QWidget *widget = new QWidget;
   widget->setLayout(layout);

   setCentralWidget(widget);
   setWindowTitle(tr("Hive"));
   setUnifiedTitleAndToolBarOnMac(true);
}


void MainWindow::createInventories() {
   
   sceneWhite = new QGraphicsScene(this);
   sceneWhite->setSceneRect(QRectF(0, 0, WIDTH, NO_PIECES*HEIGHT));
   viewWhite = new QGraphicsView(sceneWhite);
   viewWhite->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   viewWhite->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

   sceneBlack = new QGraphicsScene(this);
   sceneBlack->setSceneRect(QRectF(0, 0, WIDTH, NO_PIECES*HEIGHT));
   viewBlack = new QGraphicsView(sceneBlack);
   viewBlack->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   viewBlack->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   
   QGraphicsPixmapItem* item;
   QPixmap queenWhite(":/images/white_queen.png");
   item = sceneWhite->addPixmap(queenWhite);
   item->setPos(0, 0);   
   
   QPixmap queenBlack(":/images/black_queen.png");
   item = sceneBlack->addPixmap(queenBlack);
   item->setPos(0, 0);   
}


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


void MainWindow::about() {
    QMessageBox::about(this, tr("About Hive"),
                       tr("This is <i>Hive</i>. "
                          "Imagine some witty info text here." ));
}


void MainWindow::newGame() {
    QMessageBox::about(this, tr("New game"),
                       tr("New Game dialog placeholder." ));
}


void MainWindow::undoMove() {
    QMessageBox::about(this, tr("Undo"),
                       tr("Undo placeholder." ));
}

#endif
 
