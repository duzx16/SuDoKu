#include "mainwindow.h"
#include "solver.h"
#include <QApplication>

#include <iostream>

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
    g.generate(0);
}
*/
/*int main()
{
    int data[][9]={
            {8, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 3, 6, 0, 0, 0, 0, 0},
            {0, 7, 0, 0, 9, 0, 2, 0, 0},
            {0, 5, 0, 0, 0, 7, 0, 0, 0},
            {0, 0, 0, 0, 4, 5, 7, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 3, 0},
            {0, 0, 1, 0, 0, 0, 0, 6, 8},
            {0, 0, 8, 5, 0, 0, 0, 1, 0},
            {0, 9, 0, 0, 0, 0, 4, 0, 0}};
    int data1[][9]={
            {6,5,0,2,4,0,0,0,0},
            {0,0,0,0,0,0,8,0,2},
            {0,0,0,0,0,8,3,0,0},
            {5,0,0,4,0,0,0,8,1},
            {8,0,0,0,0,0,2,0,0},
            {2,4,7,0,0,0,0,3,0},
            {0,0,0,6,7,0,4,0,0},
            {0,9,6,0,0,0,0,5,0},
            {0,0,0,3,0,1,0,0,0}
    };
    clock_t start_time;
    start_time=clock();
    Sudoku_solver solver(data);
    solver.solve();
    solver.print_result();
    std::cout<<(double)(clock()-start_time)/CLOCKS_PER_SEC<<'\n';
    return 0;
}*/
