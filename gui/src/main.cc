#include <QtGui>

#include "mainwindow.h"

int main(int argv, char *args[])
{
    //Q_INIT_RESOURCE(diagramscene);

    QApplication app(argv, args);
    MainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 1000, 700);
    mainWindow.show();

    return app.exec();
} 
