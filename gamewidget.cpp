#include "gamewidget.h"
#include <QtWidgets>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    blocks=new QButtonGroup(this);
    QGridLayout *block_layout=new QGridLayout(this);
    QGridLayout **small_block_layout=new QGridLayout* [9];
    for(int i=0;i<9;++i)
    {
        small_block_layout[i]=new QGridLayout;
        block_layout->addLayout(small_block_layout[i],i/3,i%3);
        small_block_layout[i]->setMargin(3);
        small_block_layout[i]->setSpacing(1);
    }
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            Block *block=new Block(this);
            block->setMinimumSize(40,40);
            if(j%3==2||i%3==2)
            {
                block->setMinimumSize(34,34);
            }
            blocks->addButton(block,9*i+j);
            small_block_layout[(i/3)*3+j/3]->addWidget(block,i%3,j%3);
        }
    }
    block_layout->setMargin(50);
    block_layout->setSpacing(1);

}
