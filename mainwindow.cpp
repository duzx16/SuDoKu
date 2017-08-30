#include "mainwindow.h"
#include <QVector>
#include <random>
#include <time.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    game_widget=new GameWidget(this);
    srand(time(NULL));
    generator.generate(0);
    game_widget->initBlocks(generator.result());
    game_widget->startTimer();
    setCentralWidget(game_widget);
    setWindowTitle("Sudoku Game[*]");
}

MainWindow::~MainWindow()
{

}
