#include "windows/mainwindow.h"
#include "game/entities/maze.h"
#include "windows/boardwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //BoardWindow boardWindow;
    //boardWindow.show();

    return a.exec();
}
