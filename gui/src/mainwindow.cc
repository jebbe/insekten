#ifndef mainwindow_cc
#define mainwindow_cc

#include "mainwindow.h"

MainWindow::MainWindow()
{
   createActions();
   createToolBox();
   createMenus();
// 
//     scene = new DiagramScene(itemMenu, this);
//     scene->setSceneRect(QRectF(0, 0, 5000, 5000));
//     connect(scene, SIGNAL(itemInserted(DiagramItem*)),
//             this, SLOT(itemInserted(DiagramItem*)));
//     connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
//         this, SLOT(textInserted(QGraphicsTextItem*)));
//     connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
//         this, SLOT(itemSelected(QGraphicsItem*)));

   createToolbars();

//     QHBoxLayout *layout = new QHBoxLayout;
//     layout->addWidget(toolBox);
//     view = new QGraphicsView(scene);
//     layout->addWidget(view);
// 
//     QWidget *widget = new QWidget;
//     widget->setLayout(layout);
// 
//     setCentralWidget(widget);
     setWindowTitle(tr("Hive"));
     setUnifiedTitleAndToolBarOnMac(true);
}


void MainWindow::createToolBox() {
   
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


void MainWindow::createMenus() {
   fileMenu = menuBar()->addMenu(tr("&File"));
   fileMenu->addAction(newGameAction);
   fileMenu->addAction(exitAction);

   editMenu = menuBar()->addMenu(tr("&Edit"));
   editMenu->addAction(undoAction);

   aboutMenu = menuBar()->addMenu(tr("&Help"));
   aboutMenu->addAction(aboutAction);
}


void MainWindow::createToolbars() {
   
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
 
