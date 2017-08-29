#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "block.h"
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
    void setBlocks(int **matrix);

signals:

public slots:

    void deleteNumbers();
    void addNumber(int n);
    void setBlockHighLight();
    void setMarked(bool marked);
    void updateMarkAction();

private:
    void createAction();
    void createButtonBar();
    //每次对数字进行更改时应该调用这个函数
    void checkAnswer();

    QUndoStack *undo_stack;
    QButtonGroup *blocks;
    QButtonGroup *numberButtons;
    QTimer *timer;



    QAction *undoAction;
    QAction *redoAction;
    QAction *deleteAction;
    QAction *markAction;

    QAction *restartAction;
    QAction *pauseAction;

    QToolBar *controlBar;
    QToolBar *gameBar;
};

#endif // GAMEWIDGET_H
