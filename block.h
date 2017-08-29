#ifndef BLOCK_H
#define BLOCK_H

#include <QPushButton>
#include <QSet>

class Block:public QPushButton
{
public:
    Block(QWidget *parent=0);

    void addNumber(int n);
    void deleteNumber(int n);
    void deleteNumbers();
    bool existNumber(int n);
    bool isFixed(){return fixed;}
    bool isMarked(){return marked;}
    QList<int> allNumbers();

    void setHighLight(bool);//用背景不同表示
    void setMarked(bool);//用下划线表示
    void setWrong(bool);//用红色字体表示
    void setFixed(bool);//用黑色字体表示

    //对数字更改后应该调用这个函数
    void updateText();
    //对显示格式更改后应该调用则个函数
    void updateStyle();

private:
    QSet<int> numbers;
    bool marked;
    bool wrong;
    bool fixed;
    bool highlight;
};

#endif // BLOCK_H
