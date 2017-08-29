#include "gamewidget.h"
#include <QtWidgets>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    blocks=new QButtonGroup(this);
    QGridLayout *block_layout=new QGridLayout;
    createBlocks(block_layout);

    undo_stack=new QUndoStack(this);
    createAction();
    createButtonBar();


    QHBoxLayout *numberLayout=new QHBoxLayout;
    createNumberButtons(numberLayout);

    QVBoxLayout *v_layout=new QVBoxLayout(this);
    QHBoxLayout *up_layout=new QHBoxLayout();
    up_layout->addWidget(gameBar);
    up_layout->addWidget(lcd_time);
    v_layout->addLayout(up_layout);
    v_layout->addLayout(block_layout);
    v_layout->addWidget(controlBar);
    v_layout->addLayout(numberLayout);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(addASecond()));
    startTimer();

}

void GameWidget::initBlocks(QVector<QVector<int> > data)
{
    //用于将方格初始化
    init_data=data;
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            Block *cur=dynamic_cast<Block *>(blocks->button(i*9+j));
            cur->deleteNumbers();
            if(data[i][j])
            {
                cur->addNumber(data[i][j]);
                cur->setFixed(true);
            }
            else
            {
                cur->setFixed(false);
            }
            cur->setWrong(false);
            cur->setMarked(false);
            cur->setHighLight(false);
        }
    }
}

void GameWidget::deleteNumbers()
{
    if(blocks->checkedButton())
    {
        Block *aim=dynamic_cast<Block *>(blocks->checkedButton());
        if(!aim->isFixed())
        {
            undo_stack->push(new DeleteNumbersCommand(aim));
            checkAnswer();
        }
    }
}

void GameWidget::addNumber(int n)
{
    if(blocks->checkedButton())
    {
        Block *aim=dynamic_cast<Block *>(blocks->checkedButton());
        if(!aim->isFixed())
        {
            undo_stack->push(new AddNumberCommand(aim,n));
            checkAnswer();
        }
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

void GameWidget::addASecond()
{
    seconds+=1;
    updateLCDTime();
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

    //添加“暂停”动作
    pauseAction=new QAction(this);
    pauseAction->setIcon(QIcon(":/icon/pause"));
    connect(pauseAction,SIGNAL(triggered(bool)),this,SLOT(pauseTimer()));

    //添加“重来”动作
    restartAction=new QAction(this);
    restartAction->setIcon(QIcon(":/icon/restart"));
    connect(restartAction,SIGNAL(triggered(bool)),this,SLOT(restartGame()));
}

void GameWidget::createButtonBar()
{
    //创建“控制”工具栏（游戏界面下方）
    controlBar=new QToolBar(this);
    controlBar->addAction(undoAction);
    controlBar->addAction(redoAction);
    controlBar->addAction(deleteAction);
    controlBar->addAction(markAction);

    //创建“游戏”工具栏（游戏界面上方）
    gameBar=new QToolBar(this);
    gameBar->addAction(pauseAction);
    gameBar->addAction(restartAction);
    lcd_time=new QLCDNumber(this);

}

void GameWidget::createBlocks(QGridLayout *block_layout)
{
    //创建数独游戏界面的方格
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

}

void GameWidget::createNumberButtons(QHBoxLayout *numberLayout)
{
    numberButtons=new QButtonGroup(this);
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
}

void GameWidget::startTimer()
{
    seconds=0;
    updateLCDTime();
    timer->start(1000);
}

void GameWidget::updateLCDTime()
{
    int mins=seconds/60,secs=seconds%60;
    lcd_time->display(QString("%1:%2").arg(mins,2,10,QLatin1Char('0')).arg(secs,2,10,QLatin1Char('0')));
}

bool checkBlocks(Block *aim1,Block *aim2)
{
    bool wrong=false;
    foreach(int num,aim1->allNumbers())
    {
        if(aim2->existNumber(num))
        {
            wrong=true;
            break;
        }
    }
    return wrong;
}


void GameWidget::checkAnswer()
{
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            bool wrong=false;
            Block *cur_block=dynamic_cast<Block *>(blocks->button(i*9+j));
            for(int k=0;k<9;++k)
            {
                if(k!=i)
                {
                    if(checkBlocks(cur_block,dynamic_cast<Block *>(blocks->button(k*9+j))))
                    {
                        wrong=true;
                        break;
                    }
                }
                if(k!=j)
                {
                    if(checkBlocks(cur_block,dynamic_cast<Block *>(blocks->button(i*9+k))))
                    {
                        wrong=true;
                        break;
                    }
                }
            }
            for(int k=0;k<3;++k)
                for(int l=0;l<3;++l)
                {
                    if(k!=i%3||l!=j%3)
                    {
                        if(checkBlocks(cur_block,dynamic_cast<Block *>(blocks->button(i/3*27+j/3*3+k*3+l))))
                        {
                            wrong=true;
                            break;
                        }
                    }
                }
            cur_block->setWrong(wrong);
        }
    }
}

void GameWidget::pauseTimer()
{
    timer->stop();
    QMessageBox dialog;
    dialog.setText("The game has been stopped.");
    dialog.exec();
    timer->start();
}

void GameWidget::restartGame()
{
    initBlocks(init_data);
    startTimer();
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


