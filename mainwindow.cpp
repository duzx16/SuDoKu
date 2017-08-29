#include "mainwindow.h"
#include <QVector>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    game_widget=new GameWidget(this);
    QVector<QVector<int>> data=QVector<QVector<int>>(9,QVector<int>(9));
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            if(rand()%10<5)
            {
                data[i][j]=(i+j+1)%9+1;
            }
            else
            {
                data[i][j]=0;
            }
        }
    }
    game_widget->initBlocks(data);
    setCentralWidget(game_widget);
}

MainWindow::~MainWindow()
{

}
