#include "mainwindow.h"
#include <QVector>
#include <random>
#include <time.h>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    game_widget=new GameWidget(this);
    //添加“求助”动作
    askHelpAction=new QAction("求助",this);
    connect(askHelpAction,SIGNAL(triggered(bool)),game_widget,SLOT(askHelp()));
    controlMenu=menuBar()->addMenu("控制");
    controlMenu->addAction(askHelpAction);

    srand(time(NULL));
    generator.generate(9);
    game_widget->initBlocks(generator.result(),generator.return_answer());
    game_widget->startTimer();
    setCentralWidget(game_widget);
    setWindowTitle("Sudoku Game[*]");


}

MainWindow::~MainWindow()
{

}
