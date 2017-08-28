#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    game_widget=new GameWidget(this);
    setCentralWidget(game_widget);
}

MainWindow::~MainWindow()
{

}
