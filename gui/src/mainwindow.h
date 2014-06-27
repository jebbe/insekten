#ifndef mainwindow_h 
#define mainwindow_h

#include <QMainWindow>
#include <QtGui>
#include <QLabel>

#include "gamescene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   MainWindow();

private slots:
//     void backgroundButtonGroupClicked(QAbstractButton *button);
//     void buttonGroupClicked(int id);
//     void deleteItem();
//     void pointerGroupClicked(int id);
//     void bringToFront();
//     void sendToBack();
//     void itemInserted(DiagramItem *item);
//     void textInserted(QGraphicsTextItem *item);
//     void currentFontChanged(const QFont &font);
//     void fontSizeChanged(const QString &size);
//     void sceneScaleChanged(const QString &scale);
//     void textColorChanged();
//     void itemColorChanged();
//     void lineColorChanged();
//     void textButtonTriggered();
//     void fillButtonTriggered();
//     void lineButtonTriggered();
//     void handleFontChange();
//     void itemSelected(QGraphicsItem *item);
   void newGame();
   void about();
   void undoMove();

private:

   void createToolBox();
   void createActions();
   void createMenus();
   void createToolbars();
   
//     QWidget *createBackgroundCellWidget(const QString &text,
//                                         const QString &image);
//     QWidget *createCellWidget(const QString &text,
//                               DiagramItem::DiagramType type);
//     QMenu *createColorMenu(const char *slot, QColor defaultColor);
//     QIcon createColorToolButtonIcon(const QString &image, QColor color);
//     QIcon createColorIcon(QColor color);

   GameScene *scene;
   QGraphicsView *view;

   QAction *exitAction;
   QAction *undoAction;
   QAction *aboutAction;
   QAction *newGameAction;

   QMenu *fileMenu;
   QMenu *editMenu;
   QMenu *aboutMenu;

//     QToolBar *textToolBar;
//     QToolBar *editToolBar;
//     QToolBar *colorToolBar;
//     QToolBar *pointerToolbar;
// 
//     QComboBox *sceneScaleCombo;
//     QComboBox *itemColorCombo;
//     QComboBox *textColorCombo;
//     QComboBox *fontSizeCombo;
//     QFontComboBox *fontCombo;
// 
//     QToolBox *toolBox;
//     QButtonGroup *buttonGroup;
//     QButtonGroup *pointerTypeGroup;
//     QButtonGroup *backgroundButtonGroup;
//     QToolButton *fontColorToolButton;
//     QToolButton *fillColorToolButton;
//     QToolButton *lineColorToolButton;
//     QAction *boldAction;
//     QAction *underlineAction;
//     QAction *italicAction;
//     QAction *textAction;
//     QAction *fillAction;
//     QAction *lineAction;
};

#endif
