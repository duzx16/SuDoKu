#include "mainwindow.h"
#include <QVector>
#include <random>
#include <time.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    game_widget=new GameWidget(this);
    srand(time(NULL));
    QVector<QVector<int>> data=QVector<QVector<int>>(9,QVector<int>(9));
    for(int i=0;i<3;++i)
    {
        for(int j=0;j<3;++j)
        {
            for(int k=0;k<9;++k)
            {
                if(rand()%10<6)
                {
                    data[j*3+i][k]=(j+i*3+k)%9+1;
                }
                else
                {
                    data[i][j]=0;
                }
            }
        }
    }
    game_widget->initBlocks(data);
    game_widget->startTimer();
    setCentralWidget(game_widget);
    setWindowTitle("Sudoku Game[*]");
}

MainWindow::~MainWindow()
{

}
