#include "gamewidget.h"
#include <QtWidgets>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    blocks=new QButtonGroup(this);
    QGridLayout *block_layout=new QGridLayout;
    QGridLayout **small_block_layout=new QGridLayout* [9];
    for(int i=0;i<9;++i)
    {
        small_block_layout[i]=new QGridLayout;
        block_layout->addLayout(small_block_layout[i],i/3,i%3);
        small_block_layout[i]->setMargin(3);
        small_block_layout[i]->setSpacing(3);
    }
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            Block *block=new Block(this);
            block->addNumber(1);
            block->setMinimumSize(40,40);

            blocks->addButton(block,9*i+j);
            small_block_layout[(i/3)*3+j/3]->addWidget(block,i%3,j%3);
        }
    }
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<3;++j)
        {
            small_block_layout[i]->setColumnStretch(j,1);
            small_block_layout[i]->setRowStretch(j,1);
        }
    }
    for(int i=0;i<3;++i)
    {
        block_layout->setColumnStretch(i,1);
        block_layout->setRowStretch(i,1);
    }
    block_layout->setMargin(20);
    block_layout->setSpacing(1);

    connect(blocks,SIGNAL(buttonClicked(int)),this,SLOT(setBlockHighLight()));

    undo_stack=new QUndoStack(this);
    createAction();
    createButtonBar();

    numberButtons=new QButtonGroup(this);
    QHBoxLayout *numberLayout=new QHBoxLayout;
    QSignalMapper *m=new QSignalMapper(this);
    for(int i=0;i<9;++i)
    {
        QPushButton *button=new QPushButton(this);
        button->setText(QString::number(i+1));
        button->setMinimumSize(50,50);
        connect(button,SIGNAL(clicked(bool)),m,SLOT(map()));
        m->setMapping(button,i+1);
        button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        numberButtons->addButton(button,i+1);
        numberLayout->addWidget(button);
    }
    connect(m,SIGNAL(mapped(int)),this,SLOT(addNumber(int)));
    QVBoxLayout *v_layout=new QVBoxLayout(this);
    v_layout->addLayout(block_layout);
    v_layout->addWidget(controlBar);
    v_layout->addLayout(numberLayout);


}

void GameWidget::setBlocks(int **matrix)
{
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {

        }
    }
}

void GameWidget::deleteNumbers()
{
    if(blocks->checkedButton())
    {
        Block *aim=dynamic_cast<Block *>(blocks->checkedButton());
        if(!aim->isFixed())
            undo_stack->push(new DeleteNumbersCommand(aim));
    }
}

void GameWidget::addNumber(int n)
{
    if(blocks->checkedButton())
    {
        Block *aim=dynamic_cast<Block *>(blocks->checkedButton());
        undo_stack->push(new AddNumberCommand(aim,n));
    }

}

void GameWidget::setBlockHighLight()
{
    int aim_num=0;
    auto cur_block=blocks->checkedButton();
    if(blocks->checkedButton())
    {
        QList<int> cur_nums=dynamic_cast<Block *>(cur_block)->allNumbers();
        if(cur_nums.size()==1)
        {
            aim_num=cur_nums[0];
        }
    }
    foreach(auto block,blocks->buttons())
    {
        bool set_high_light=dynamic_cast<Block *>(block)->existNumber(aim_num);
        dynamic_cast<Block *>(block)->setHighLight(set_high_light);
    }

}

void GameWidget::setMarked(bool marked)
{
    if(blocks->checkedButton())
    {
        dynamic_cast<Block *>(blocks->checkedButton())->setMarked(marked);
    }
}

void GameWidget::updateMarkAction()
{
    markAction->setChecked(dynamic_cast<Block *>(blocks->checkedButton())->isMarked());
}

void GameWidget::createAction()
{
    //添加“撤销”动作
    undoAction=undo_stack->createUndoAction(this);
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setIcon(QIcon(":/icon/undo"));

    //添加“重做”动作
    redoAction=undo_stack->createRedoAction(this);
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setIcon(QIcon(":/icon/redo"));

    //添加“删除”动作
    deleteAction=new QAction(this);
    deleteAction->setIcon(QIcon(":/icon/delete"));
    connect(deleteAction,SIGNAL(triggered(bool)),this,SLOT(deleteNumbers()));

    //添加“标记”动作
    markAction=new QAction(this);
    markAction->setIcon(QIcon(":/icon/mark"));
    markAction->setCheckable(true);
    connect(markAction,SIGNAL(triggered(bool)),this,SLOT(setMarked(bool)));
    connect(blocks,SIGNAL(buttonClicked(int)),this,SLOT(updateMarkAction()));
}

void GameWidget::createButtonBar()
{
    controlBar=new QToolBar(this);
    controlBar->addAction(undoAction);
    controlBar->addAction(redoAction);
    controlBar->addAction(deleteAction);
    controlBar->addAction(markAction);
}



AddNumberCommand::AddNumberCommand(Block *block, int n):aim(block),number(n)
{
    setText("add number "+QString::number(n));
}

void AddNumberCommand::undo()
{
    aim->deleteNumber(number);
}

void AddNumberCommand::redo()
{
    aim->addNumber(number);
}



DeleteNumbersCommand::DeleteNumbersCommand(Block *block):aim(block)
{
    setText("delete all numbers");
}

void DeleteNumbersCommand::undo()
{
    foreach(int num,numbers)
    {
        aim->addNumber(num);
    }
}

void DeleteNumbersCommand::redo()
{
    numbers=aim->allNumbers();
    aim->deleteNumbers();
}


