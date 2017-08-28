#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "block.h"
#include <QWidget>
#include <QVector>

QT_BEGIN_NAMESPACE
class QUndoStack;
class QAction;
class QTimer;
class QButtonGroup;
QT_END_NAMESPACE

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);
    //todo
    void setBlocks();

signals:

public slots:
private:
    //每次对数字进行更改时应该调用这个函数
    void checkAnswer();

    QUndoStack *undo_stack;
    QButtonGroup *blocks;
    Block *current;
    QTimer *timer;



    QAction *undoAction;
    QAction *redoAction;
    QAction *deleteAction;
    QAction *markAction;
    QAction *restartAction;
    QAction *pauseAction;
};

#endif // GAMEWIDGET_H
