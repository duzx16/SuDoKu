#include "mainwindow.h"
#include <QVector>
#include <random>
#include <time.h>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    game_widget=new GameWidget(this);

    createActions();

    begin_page=new BeginPage(this);

    complex_dialog=new ComplexityDialog(this);
    complex_dialog->hide();

    connect(begin_page,SIGNAL(GameBegin()),complex_dialog,SLOT(show()));
    connect(begin_page,SIGNAL(QuitGame()),this,SLOT(close()));
    connect(complex_dialog,SIGNAL(accepted()),this,SLOT(beginGame()));
    connect(game_widget,SIGNAL(GameOver()),this,SLOT(returnBegin()));

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
    //开始一局新游戏
    srand(time(NULL));
    generator.generate(complex_dialog->value());
    game_widget->initBlocks(generator.result(),generator.return_answer());
    game_widget->startTimer();
    pages->setCurrentIndex(1);
}

void MainWindow::returnBegin()
{
    //回到开始菜单
    pages->setCurrentIndex(0);
}

void MainWindow::showGame()
{
    //显示新游戏
    pages->setCurrentIndex(1);
}

void MainWindow::saveGame()
{
    //选择路径并保存游戏
    QString path = QFileDialog::getSaveFileName(this,tr("打开文件"),".");

    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("写入文件"),tr("无法打开文件:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
        game_widget->saveGame(in);
        file.close();
    } else {
        QMessageBox::warning(this, tr("路径"),tr("你没有选择任何文件"));
    }
}

void MainWindow::loadGame()
{
    //选择存档并载入
    QString path = QFileDialog::getOpenFileName(this,tr("打开文件"),".");

    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("读入文件"),tr("无法打开文件:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
        game_widget->loadGame(in);
        file.close();
        showGame();
    } else {
        QMessageBox::warning(this, tr("路径"),tr("你没有选择任何文件"));
    }
}

void MainWindow::createActions()
{
    //添加“求助”动作
    askHelpAction=new QAction("求助",this);
    connect(askHelpAction,SIGNAL(triggered(bool)),game_widget,SLOT(askHelp()));

    //添加“回到主界面”动作
    returnBeginAction=new QAction("回到主界面",this);
    connect(returnBeginAction,SIGNAL(triggered(bool)),this,SLOT(returnBegin()));

    //添加“保存”动作
    saveGameAction=new QAction("保存当前游戏",this);
    connect(saveGameAction,SIGNAL(triggered(bool)),this,SLOT(saveGame()));

    //添加“载入”动作
    loadGameAction=new QAction("载入游戏",this);
    connect(loadGameAction,SIGNAL(triggered(bool)),this,SLOT(loadGame()));

    controlMenu=menuBar()->addMenu("控制");
    controlMenu->addAction(askHelpAction);
    controlMenu->addAction(returnBeginAction);
    controlMenu->addAction(saveGameAction);
    controlMenu->addAction(loadGameAction);
}
