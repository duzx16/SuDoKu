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

    begin_page=new BeginPage(this);

    complex_dialog=new ComplexityDialog(this);
    complex_dialog->hide();

    connect(begin_page,SIGNAL(GameBegin()),complex_dialog,SLOT(show()));
    connect(begin_page,SIGNAL(QuitGame()),this,SLOT(close()));
    connect(complex_dialog,SIGNAL(accepted()),this,SLOT(beginGame()));
    connect(game_widget,SIGNAL(GameOver()),this,SLOT(endGame()));

    pages=new QStackedWidget(this);

    pages->addWidget(begin_page);
    pages->addWidget(game_widget);

    setCentralWidget(pages);
    setWindowTitle("Sudoku Game[*]");


}

MainWindow::~MainWindow()
{

}

void MainWindow::beginGame()
{
    srand(time(NULL));
    generator.generate(complex_dialog->value());
    game_widget->initBlocks(generator.result(),generator.return_answer());
    game_widget->startTimer();
    pages->setCurrentIndex(1);
}

void MainWindow::endGame()
{
    QMessageBox dialog;
    dialog.setWindowTitle("恭喜");
    dialog.setText("您已获胜！");
    dialog.exec();
    pages->setCurrentIndex(0);
}
