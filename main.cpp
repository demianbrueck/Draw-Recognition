#include "mainwindow.h"
#include <QApplication>




int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.setMouseTracking(true);
    //mainWindow.drawnLine.posX.push_back(1);
    //mainWindow.drawnLine.posY.push_back(1);

    mainWindow.setWindowTitle("Draw Recognition");
    mainWindow.show();




    return a.exec();
}

