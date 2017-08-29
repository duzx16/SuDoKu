#include "gamewidget.h"
#include <QtWidgets>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    //设置游戏主界面
    blocks=new QButtonGroup(this);
    QGridLayout *block_layout=new QGridLayout;
    createBlocks(block_layout);

    //设置杂七杂八的成员
    undo_stack=new QUndoStack(this);
    createAction();
    createButtonBar();

    //设置上方工具栏与时间布局
    QHBoxLayout *up_layout=new QHBoxLayout();
    up_layout->addWidget(gameBar);
    up_layout->addWidget(lcd_time);

    //设置数字按钮布局
    QHBoxLayout *numberLayout=new QHBoxLayout;
    createNumberButtons(numberLayout);


    //设置总布局
    QVBoxLayout *v_layout=new QVBoxLayout(this);
    v_layout->addLayout(up_layout);
    v_layout->addLayout(block_layout);
    v_layout->addLayout(numberLayout);
    v_layout->addSpacing(30);

    //设置计时器
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(addASecond()));

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
    //删除所选方格内的所有数字
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
    //在所选方格内增加数字
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
    //将与所选方格数字相同的方格设置为高亮
    int aim_num=0;
    auto cur_block=blocks->checkedButton();
    if(cur_block)
    {
        QList<int> cur_nums=dynamic_cast<Block *>(cur_block)->allNumbers();
        if(cur_nums.size()==1)
        {
            aim_num=cur_nums[0];
        }
    }
    foreach(auto block,blocks->buttons())
    {
        bool set_high_light=dynamic_cast<Block *>(block)->containNumber(aim_num);
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
    deleteAction->setIcon(QIcon(":/icon/remove"));
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
    //创建“游戏”工具栏（游戏界面上方）
    gameBar=new QToolBar(this);
    gameBar->addAction(pauseAction);
    gameBar->addAction(restartAction);
    gameBar->addAction(undoAction);
    gameBar->addAction(redoAction);
    gameBar->addAction(markAction);
    gameBar->addAction(deleteAction);
    //创建LCD时间显示
    lcd_time=new QLCDNumber(this);
    lcd_time->setStyleSheet("QLCDNumber{background-color:#DDF0ED;}");

}

void GameWidget::createBlocks(QGridLayout *block_layout)
{
    //创建数独游戏界面的方格
    QGridLayout **small_block_layout=new QGridLayout* [9];
    for(int i=0;i<9;++i)
    {
        small_block_layout[i]=new QGridLayout;
        block_layout->addLayout(small_block_layout[i],i/3,i%3);
        small_block_layout[i]->setMargin(2);
        small_block_layout[i]->setSpacing(1);
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
    //创建数字按钮
    numberButtons=new QButtonGroup(this);
    QSignalMapper *m=new QSignalMapper(this);
    for(int i=0;i<9;++i)
    {
        QPushButton *button=new QPushButton(this);
        button->setText(QString::number(i+1));
        button->setMinimumSize(40,40);
        button->setStyleSheet("QPushButton{background-color:#F3D7B5;border:0px;font-weight:bold;color:#2E68AA;} QPushButton:hover{background-color:#EFCEE8;} QPushButton:pressed{background-color:#C7B3E5}");
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
    //（重新）开始计时
    seconds=0;
    updateLCDTime();
    timer->start(1000);
}

void GameWidget::updateLCDTime()
{
    //将LCD的显示时间与second成员同步
    int mins=seconds/60,secs=seconds%60;
    lcd_time->display(QString("%1:%2").arg(mins,2,10,QLatin1Char('0')).arg(secs,2,10,QLatin1Char('0')));
}

bool checkBlocks(Block *aim1,Block *aim2)
{
    //检查两个方格是否有重复数字
    bool wrong=false;
    foreach(int num,aim1->allNumbers())
    {
        if(aim2->containNumber(num))
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
    //暂停计时并弹出模态对话框
    timer->stop();
    QMessageBox dialog;
    dialog.setText("The game has been paused.");
    dialog.exec();
    timer->start();
}

void GameWidget::restartGame()
{
    //重新开始游戏（即重新载入）
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


