#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "block.h"
#include "solver.h"
#include <QWidget>
#include <QVector>
#include <QList>
#include <QUndoCommand>

QT_BEGIN_NAMESPACE
class QUndoStack;
class QAction;
class QTimer;
class QButtonGroup;
class QUndoCommand;
class QToolBar;
class QLCDNumber;
class QGridLayout;
class QHBoxLayout;
class QMenu;
class QTextStream;
QT_END_NAMESPACE

class AddNumberCommand:public QUndoCommand
{
public:
    AddNumberCommand(Block *block,int n);
    virtual void undo();
    virtual void redo();
private:
    Block *aim;
    int number;
};

class DeleteNumbersCommand:public QUndoCommand
{
public:
    DeleteNumbersCommand(Block *block);
    virtual void undo();
    virtual void redo();

private:
    Block *aim;
    QList<int> numbers;
};

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);

    //用于开始游戏的接口
    void initBlocks(const Sudoku_array &data,const Sudoku_array &answer);
    void startTimer();

    void saveGame(QTextStream &in);
    void loadGame(QTextStream &out);

signals:
    void GameOver();

public slots:
    //用于向方格中增删数字
    void deleteNumbers();
    void addNumber(int n);

    //用于设置方格的格式
    void setBlockHighLight();
    void setRACBorder();
    void setMarked(bool marked);

    //用于计时的槽函数
    void addASecond();
    void pauseTimer();

    //重启游戏的槽函数
    void restartGame();

    //用于更新“标记”按钮的状态
    void updateMarkAction();

    void askHelp();

    //检测错误数字，每次对数字进行更改时应该调用这个函数
    void checkAnswer();

    //音效
    void blockSound();
    void numberSound();


private:
    //初始化界面的函数
    void createAction();
    void createMenu();
    void createButtonBar();
    void createBlocks(QGridLayout *block_layout);
    void createNumberButtons(QHBoxLayout *numberLayout);

    //用于计时的函数
    void updateLCDTime();

    //检验游戏是否结束，由checkAnswer调用
    void checkGameOver();

    //用于实现撤销
    QUndoStack *undo_stack;
    //游戏方块
    QButtonGroup *blocks;
    //下方的数字按钮
    QButtonGroup *numberButtons;

    //用于计时
    QTimer *timer;
    QLCDNumber *lcd_time;
    int seconds;

    //本局游戏的初始状态
    Sudoku_array init_data;
    Sudoku_array answer_data;


    //游戏中的六个按钮
    QAction *undoAction;
    QAction *redoAction;
    QAction *deleteAction;
    QAction *markAction;
    QAction *restartAction;
    QAction *pauseAction;


    //界面上方的工具栏
    QToolBar *gameBar;

};

#endif // GAMEWIDGET_H
