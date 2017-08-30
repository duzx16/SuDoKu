#include "mainwindow.h"
#include "solver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
/*
int main()
{
    Generator g;
    g.generate(9);
}
*/
