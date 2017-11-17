#include "mainwindow.h"
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    // All the main really does is create the application, the main window, and the first child of the main windows, then shows them.
    QApplication a(argc, argv);
    FileOpen* fo = new FileOpen();
    MainWindow* w = new MainWindow(0,fo);
    w->show();
    // This starts the execution and interactive part of the program.
    return a.exec();
}
